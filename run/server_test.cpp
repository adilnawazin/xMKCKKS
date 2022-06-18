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
	server server1;
	server server2;
	int port1 = 8080;
	int port2 = 8081;
	int sock1 = 0;
	int sock2 = 0;
	sock1 = server1.estab_conn(port1);
	sock2 = server2.estab_conn(port2);
	do{
		sleep(3);
	} while (sock1 == 0 || sock2== 0);

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
	scheme.ZZ_Send(axP, sock1, "AXP ");
	scheme.ZZ_Send(axP, sock2, "AXP ");
	timeutils.stop("Send AXP to Users");
	ZZ* pkey = scheme.PublicKeyGeneration(secretKey, axP);
//	Joint Public Key Generation at Server (b' = b1+b2+...+bn, e)
	ZZ* pkey1  = new ZZ[N];
	ZZ* pkey2 = new ZZ[N];
	timeutils.start("Receive Key from Users");
	scheme.ZZ_Receive(pkey1, sock1, "Public Key ");
	scheme.ZZ_Receive(pkey2, sock2, "Public Key ");
    
	ZZ* jkey_send= scheme.Jkeysend(axP ,pkey, pkey1, pkey2);
	delete[] pkey1, delete[] pkey2; delete[] pkey;
	timeutils.start("Send Joint Key to Users");
	scheme.ZZ_Send(jkey_send, sock1, "Joint Key ");
	scheme.ZZ_Send(jkey_send, sock2, "Joint Key ");
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
	ZZ* ctax1 = new ZZ[N];
	ZZ* ctbx1 = new ZZ[N];
	ZZ* ctax2 = new ZZ[N];
	ZZ* ctbx2 = new ZZ[N];
	
	scheme.ZZ_Receive(ctax1, sock1, "Cipher Text (Co) ");
	scheme.ZZ_Receive(ctbx1, sock1, "Cipher Text (C1) ");
	scheme.ZZ_Receive(ctax2, sock2, "Cipher Text (Co) ");
	scheme.ZZ_Receive(ctbx2, sock2, "Cipher Text (C1) ");
	cout << "ctax1: " << ctax1 <<"\n";
	cout << "ctbx1: " << ctbx1 <<"\n";	
	Ciphertext cipher1;
	Ciphertext cipher2;
	cipher1.ax = ctax1;
	cipher1.bx = ctbx1;
	cipher2.ax = ctax2;
	cipher2.bx = ctbx2;
	cout << "cipher1.ax: " << cipher1.ax << "\n";
	cout << "cipher1.bx: " << cipher1.bx << "\n";
	timeutils.stop("Receive CipherText from the Users");
	Ciphertext cipherAdd;
	scheme.AddCipherText(cipherAdd, cipher, cipher1, cipher2);
	timeutils.start("Send CipherAdd to devices");
	scheme.ZZ_Send(cipherAdd.ax, sock1, "CiphertextADD (c1) ");
	scheme.ZZ_Send(cipherAdd.ax, sock2, "CiphertextADD (c1) ");
	timeutils.stop("CipherAdd to devices"); 
	Plaintext plain_t;
	scheme.DecryptionShare(plain_t, cipher, secretKey, cipherAdd.ax);
	timeutils.start("Receive Partial Decryption Share from Users");
	ZZ* partial_pt1 = new ZZ[N];
	ZZ* partial_pt2 = new ZZ[N];
	scheme.ZZ_Receive(partial_pt1, sock1, "Partial Decryption Share ");
	scheme.ZZ_Receive(partial_pt2, sock2, "Partial Decryption Share ");
	Plaintext plain_t1;
	Plaintext plain_t2;
	plain_t1.mx = partial_pt1;
	plain_t2.mx = partial_pt2;
	timeutils.stop("Receive Partial Decryption Share from Users");
//	Merge Decryption (m = C_sum0 + SUM(D_i) + e) and decode
	scheme.Decryption(plain_t, cipherAdd, plain_t1, plain_t2);	
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
	// return 0;
}