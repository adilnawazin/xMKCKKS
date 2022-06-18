#include <iostream>
#include <sstream>
#include <string>  
#include "../src/HEAAN.h"
#include "NTL/ZZ.h"

#define IPADD "192.168.3.2"
using namespace std;


int main(int argc, char **argv) {
	long logq = 800;                // (Ciphertext Modulus) should be less than logQ in params.h
	long logp = 40;                 // (Scaling) message will be quantized by multiplying 2^logp (larger value --> more accuracy)
	long logn = 15;                 // (The number of slots) should be less than logN in params.h
    int clientsock;
	if(string (argv[1])=="--h"){
		cout<< "Please specify number of trials and number of rounds per trial in following format to run this test:"<<endl;
		cout<< "./TestMKScheme Test x y z (x = round, y = trial , z = length of vector passed to test"<<endl;
	}
	if(string(argv[1]) == "Test"){
	string arg1(argv[2]);
	string arg2(argv[3]);
	string arg3(argv[4]);
    string round = arg1; 
	string trial = arg2;
    string stport = arg3;
    int port = stoi(stport);

    cout << "Connecting" << endl;
	client client;
	clientsock = client.connect_serv(port, IPADD);
	int sum = 0;
	double x;
	ifstream inFile;
	long n= (1 << logn);
	ostringstream temp;
	temp << round;

//  Data Import from File (Users n)
	TimeUtils timeutils;
	timeutils.start("Data Import");
	double* mvec = EvaluatorUtils::randomRealArray(n);
	// inFile.open("/home/adel/heaan_test/doc_T1_"+round+"_"+trial+".txt");
	inFile.open("/home/nano1/xMKCKKS/test_docs/doc_T1_"+round+"_"+trial+".txt");
	// inFile.open("/home/nano2/xMKCKKS/test_docs/doc_T1_"+round+"_"+trial+".txt");	
    long i=0;
	if (!inFile) {
	cout << "Unable to open file doc_T1_"+round+"_"+trial<<endl;
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
//  Encode Data (msg(vector) --> plaintext)
	timeutils.start("Encode");
	SetNumThreads(8);
	Ring ring;
	SecretKey secretKey(ring);
	Plaintext plain;
	MKScheme_client scheme(secretKey, ring);
	scheme.encode(plain, mvec, n, logp, logq);
    timeutils.stop("Encode");
//	Publickey Gen (Each User generates its PK i.e. (b = -s.a+e))
    timeutils.start("Receive AXP from Server");
	ZZ* axP1  = new ZZ[N];
	scheme.ZZ_Receive(axP1,clientsock,"AXP ");
    ZZ* pkey1 = scheme.PublicKeyGeneration(secretKey, axP1);
//	Joint Public Key Generation at Server (b' = b1+b2+...+bn, e)
    timeutils.start("Send Key to server");
    scheme.ZZ_Send(pkey1, clientsock, "Public Key ");
    timeutils.stop("Send Key to server");
    timeutils.start("Receive Joint Key from server");
    ZZ* jkeyrecv = new ZZ[N];
    scheme.ZZ_Receive(jkeyrecv, clientsock, "Joint Key ");
    // uint64_t* rbx = new uint64_t[Nnprimes];
	// for (i=0; i<Nnprimes; i++)
    // {
    // memset(buf,0, sizeof(buf));
    // client.client_receive(clientsock, buf);
    // rbx[i] = stoull(buf);
    // }
    // jkey->rax = rax;
    // jkey->rbx = rbx;
    timeutils.stop("Receive Joint Key from server");
    Key* jkey;
    jkey = scheme.JointKeyGeneration(axP1, jkeyrecv);
//	Encryption at Device D(cipher = (v.b' + m + e) , (v.a + e))
//  Encrypt message 1 ---> n
	timeutils.start("Encryption");
    Ciphertext cipher;
	scheme.EncryptMsg(cipher, plain, jkey);
    timeutils.stop("Encryption");
    delete[] axP1; delete[] jkeyrecv;
    timeutils.start("Send Ciphertext to Server");
    scheme.ZZ_Send(cipher.ax , clientsock, "Ciphertext Co ");
    scheme.ZZ_Send(cipher.bx , clientsock, "Ciphertext C1 ");
    timeutils.stop("Send Ciphertext to Server");    
    timeutils.start("Receive CipherAdd from server");
    ZZ* cadd_ax  = new ZZ[N];
    scheme.ZZ_Receive(cadd_ax, clientsock, "CipherADD ");
    Ciphertext cipherAdd;
    cipherAdd.ax = cadd_ax;
    timeutils.stop("Receive CipherAdd from server");
    Plaintext plain_t;
	scheme.DecryptionShare(plain_t, cipher, secretKey, cipherAdd.ax);
	timeutils.start("Send Partial Decryption Share to server");
	scheme.ZZ_Send(plain_t.mx, clientsock, "Decryption Share ");
    timeutils.stop("Send Partial Decryption Share to server");
    // delete[] cadd_ax;
     //Receive Decryption from server
    // Plaintext decrypted_pt;
    // ZZ* decryptedmx  = new ZZ[N];
    // for(i=0; i<N; i++)
    // {
    // memset(buf,0, sizeof(buf));
    // client.client_receive(clientsock, buf);
    // decryptedmx[i] = conv<ZZ>(buf);
    // // cout<< "decrypted plaintext = " << decryptedmx[i]<< endl;
    // }
    // decrypted_pt.mx = decryptedmx;
    // complex<double>* res=scheme.decode(decrypted_pt);
    //Compare Results
    // double* madd = new double[n];
    // for(long i = 0; i < n; i++) {
    // 	madd[i] = mvec[i] + mvec1[i];
    // }
    // cout<< "========================="<<endl;
    // cout<< "G Truth : ";
    // StringUtils::showVec(madd,19);
    // cout << "========================"<<endl;
    // cout<<"Output  : ";
    // StringUtils::showVec_RP(res,19);
    // StringUtils::compare(madd,res,n,"Add");
} 
	close(clientsock);
    return 0;
}