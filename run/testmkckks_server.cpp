#include <iostream>
#include <sstream>
#include <string>  
#include "../src/HEAAN.h"
#include "NTL/ZZ.h"

using namespace std;


int main(int argc, char **argv) {
	long logq = 800;						// (Ciphertext Modulus) should be less than logQ in params.h
	long logp = 40; 						// (Scaling) message will be quantized by multiplying 2^logp (larger value --> more accuracy)
	long logn = 15; 						// (The number of slots) should be less than logN in params.h
	cout << "Start listening" << endl;
	server server;
	int sock = server.estab_conn();

	if(string (argv[1])=="--h"){
		cout<< "Please specify number of trials and number of rounds per trial in following format to run this test:"<<endl;
		cout<< "./TestMKScheme Test x y z (x = round, y = trial , z = length of vector passed to test"<<endl;
	}
	if(string(argv[1]) == "Test"){
	string arg1(argv[2]); string arg2(argv[3]); string round = arg1; string trial = arg2;
	int sum = 0;
	double x;
	ifstream inFile;
	long n= (1 << logn);
	ostringstream temp;
	temp << round;
//	Data Import from File (Users n)
	TimeUtils timeutils;
	timeutils.start("Data Import");
	double* mvec = EvaluatorUtils::randomRealArray(n);
	inFile.open("/home/adel/heaan_test/doc_T0_"+round+"_"+trial+".txt");
	long i=0;
	if (!inFile) {
	cout << "Unable to open file doc_T0_"+round+"_"+trial<<endl;
	exit(1);
	}
	while (inFile >> x) {
	double tmp;
	tmp = x;
	mvec[i] = tmp;
	i++;
	}
	inFile.close();
	timeutils.stop("Data Import");
//	Encode Data (msg(vector) --> plaintext)
	timeutils.start("Encode");
	SetNumThreads(8);
	StringUtils stringutils;
	Ring ring;
	SecretKey secretKey(ring);
	Plaintext plain;
	MKScheme_server scheme(secretKey, ring);
	scheme.encode(plain, mvec, n, logp, logq);
	timeutils.stop("Encode");
//	Publickey Gen (Each User generates its PK i.e. (b = -s.a+e))
	timeutils.start("Send AXP to Users");
	ZZ* axP = new ZZ[N];
	ring.sampleUniform2(axP, logQQ);
    stringstream stream;							// buffer for data (recv, send)
	char buffer[512];
	float progress = 0.0;
	for (i= 0; i<N; i++)
	{
	stream = stringstream();						// clear buffer
	stream << axP[i];
	int bytesent=sizeof(buffer);
    memset(buffer, 0 , sizeof(buffer));
	strcpy(buffer, stream.str().c_str());			// copy a[i] to buffer
	if(bytesent != sizeof(buffer))
	{
		i--;
		// cout <<"i " << i<<endl;
		// cout << "b " <<bytesent << endl;
		continue;
	}
	bytesent = server::tcp_send(sock, buffer);					// send to user-a
	progress = (float(i)/float(N))*100;
	cout << "Sending Common Reference String to Devices ... " << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
	timeutils.stop("Send AXP to Users");
	ZZ* pkey = scheme.PublicKeyGeneration(secretKey, axP);
//	Joint Public Key Generation at Server (b' = b1+b2+...+bn, e)
	ZZ* pkey1  = new ZZ[N];
	timeutils.start("Receive Key from Users");
	progress = 0.0;
	for (i=0; i<N;i++)
	{
	memset (buffer,0,sizeof(buffer));
	server.tcp_recv(sock, buffer);
	// cout<< "pkey["<<i<<"]: "<< buffer <<endl;
	pkey1[i] = conv<ZZ>(buffer);
	progress = (float(i)/float(N))*100;
	cout << "Receiving Key from Devices ... " << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
	timeutils.stop("Receive Key from Users");
    
	ZZ* jkey_send= scheme.Jkeysend(axP ,pkey, pkey1);
	timeutils.start("Send Joint Key to Users");
	progress = 0.0;
	for (i=0;i<N;i++)
	{
	memset(buffer,0, sizeof(buffer));
	stream = stringstream();
	stream << jkey_send[i];
	strcpy(buffer, stream.str().c_str());
	server.tcp_send(sock, buffer);
	progress = (float(i)/float(N))*100;
	cout << "Sending Joint Key to Devices... " << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
	timeutils.stop("Send Joint Key to Users");
	Key* jkey = scheme.JointKeyGeneration(axP, jkey_send);
//	Encryption at Device D(cipher = (v.b' + m + e) , (v.a + e))
// 	Encrypt message 1 ---> n
	timeutils.start("Encryption");
	Ciphertext cipher;
	scheme.EncryptMsg(cipher, plain, jkey);
    timeutils.stop("Encryption");
// 	Cipher Add(C_Sum0, C_sum1 = SUM(v.b' + m + e), SUM(v.a + e))
	timeutils.start("Receive CipherText from the Users");
	ZZ* ctax = new ZZ[N];
	ZZ* ctbx = new ZZ[N];
	progress = 0.0;
	for (i=0; i<N;i++)
	{
	memset (buffer,0,sizeof(buffer));
	server.tcp_recv(sock, buffer);
	ctax[i] = conv<ZZ>(buffer);
	progress = (float(i)/float(N))*100;
	cout << "Receive Ciphertext (c0)....................." << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
	progress = 0.0;
	for (i=0; i<N;i++)
	{
	memset (buffer,0,sizeof(buffer));
	server.tcp_recv(sock, buffer);
	ctbx[i] = conv<ZZ>(buffer);
	progress = (float(i)/float(N))*100;
	cout << "Receive Ciphertext (c1)....................." << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
	Ciphertext cipher1;
	cipher1.ax = ctax;
	cipher1.bx = ctbx;
	timeutils.stop("Receive CipherText from the Users");
    Ciphertext cipherAdd;
	scheme.AddCipherText(cipherAdd, cipher, cipher1);
	timeutils.start("CipherAdd to devices");
	for (i= 0; i<N; i++)
	{
	stream = stringstream();						// clear buffer
    memset(buffer, 0 , sizeof(buffer));
	stream << cipherAdd.ax[i];
	strcpy(buffer, stream.str().c_str());			// copy a[i] to buffer
    server::tcp_send(sock, buffer);					// send to user-a
	progress = (float(i)/float(N))*100;
	cout << "Sending CiphertextAdd (c1)....................." << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
	timeutils.stop("CipherAdd to devices");    
	Plaintext plain_t;
	scheme.DecryptionShare(plain_t, cipher, secretKey, cipherAdd.ax);
	timeutils.start("Receive Partial Decryption Share from Users");
	ZZ* partial_pt = new ZZ[N];
	progress = 0.;
	for (i=0; i<N;i++)
	{
	memset (buffer,0,sizeof(buffer));
	server.tcp_recv(sock, buffer);
	partial_pt[i] = conv<ZZ>(buffer);
	progress = (float(i)/float(N))*100;
	cout << "Receive Decryption share (Di)....................." << progress << "%"<<"\t\r" <<flush;
	}
	cout << "\n" ;
    Plaintext plain_t1;
	plain_t1.mx = partial_pt;
	timeutils.stop("Receive Partial Decryption Share from Users");
//	Merge Decryption (m = C_sum0 + SUM(D_i) + e) and decode
	scheme.Decryption(plain_t, cipherAdd, plain_t1);

	// timeutils.start("Send Decryption to client");
    // for(i=0; i<N; i++)
    // {
    // memset(buffer, 0, sizeof(buffer));
    // stream = stringstream();
	// stream << plain_t.mx[i];
	// strcpy(buffer, stream.str().c_str());
	// server.tcp_send(sock, buffer);
	// // cout<< "decrypted plaintext = " << plain_t.mx[i]<< endl;
    // }
	// timeutils.stop("Send Decryption to client");
	
	complex<double>* res=scheme.decode(plain_t);
	StringUtils::showVec_RP(res,19);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Compare Results
	// double* madd = new double[n];
	// for(long i = 0; i < n; i++) {
	// 	madd[i] = mvec[i] + mvec1[i] + mvec2[i];
	// }
	// cout<< "========================="<<endl;
	// cout<< "G Truth : ";
	// StringUtils::showVec(madd,19);
	// cout << "========================"<<endl;
	// cout<<"Output  : ";
	// StringUtils::compare(madd,res,n,"Add");
} 
	return 0;
}