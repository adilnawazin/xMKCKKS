#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string>  
#include "../src/HEAAN.h"
#include "NTL/ZZ.h"

using namespace std;


int main(int argc, char **argv) {

	long logq = 800; ///< (Ciphertext Modulus) should be less than logQ in params.h
	long logp = 40; ///< (Scaling) message will be quantized by multiplying 2^logp (larger value --> more accuracy)
	long logn = 15; ///< (The number of slots) should be less than logN in params.h


	if(string (argv[1])=="--h"){
		cout<< "Please specify number of trials and number of rounds per trial in following format to run this test:"<<endl;
		cout<< "./TestMKScheme Test x y z (x = round, y = trial , z = length of vector passed to test"<<endl;
	}
	if(string(argv[1]) == "Test"){
	string arg1(argv[2]);
	string arg2(argv[3]);

	string round = arg1;
	string trial = arg2;

			//Data import
	int sum = 0;
	double x;
	ifstream inFile;
	long n= (1 << logn);
	ostringstream temp;
	temp << round;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//						Data Import from File (Users n)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	
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
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//					Encode Data (msg(vector) --> plaintext)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	timeutils.start("Encode");
	SetNumThreads(8);
	Ring ring;
	SecretKey secretKey(ring);
	Plaintext plain;
	MKScheme scheme(secretKey, ring);
	scheme.encode(plain, mvec, n, logp, logq);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Publickey Gen (Each User generates its PK i.e. (b = -s.a+e))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	ZZ* axP = new ZZ[N];
	ring.sampleUniform2(axP, logQQ);
    // send axP to all parties
    //
    //
    //
    // 
    // 
    // 
	ZZ* pkey = scheme.PublicKeyGeneration(secretKey, axP);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Joint Public Key Generation at Server (b' = b1+b2+...+bn, e)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
    // Receive Key from Nodes 
    //pkey1
    //pkey2
    //
    //
    //
    //
    Key* jkey= scheme.JointKeyGeneration(axP ,pkey, pkey1, pkey2);

    //Send Joint Key to the Nodes
    //
    //
    //
    //
    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Encryption at Device D(cipher = (v.b' + m + e) , (v.a + e))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Encrypt message 1 ---> n
	timeutils.start("Encryption");
	Ciphertext cipher;
	scheme.EncryptMsg(cipher, plain, jkey);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		Cipher Add(C_Sum0, C_sum1 = SUM(v.b' + m + e), SUM(v.a + e))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Receive CipherText from the devices
    //cipher1
    //cipher2
    //
    //
    Ciphertext cipherAdd;
	scheme.AddCipherText(cipherAdd, cipher, cipher1, cipher2);

    //Send CipherAdd to devices
    //
    //
    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		Part Dec (D_i = s.C_sum1)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Plaintext plain_t;
	scheme.DecryptionShare(plain_t, cipher, secretKey, cipherAdd.ax);
    // Receive Partial Decryption Shares from other devices
    // plain_t1 = recv(pt1);
    // plain_t2 = recv(pt2);
    //
    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		 Merge Decryption (m = C_sum0 + SUM(D_i) + e)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	scheme.Decryption(plain_t, cipherAdd, plain_t1, plain_t2);
	complex<double>* res=scheme.decode(plain_t);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Compare Results
// 	double* madd = new double[n];
// 	for(long i = 0; i < n; i++) {
// 		madd[i] = mvec[i] + mvec1[i] + mvec2[i];
// 	}
// 	cout<< "========================="<<endl;
// 	cout<< "G Truth : ";
// 	StringUtils::showVec(madd,19);
// 	cout << "========================"<<endl;
// 	cout<<"Output  : ";
// 	StringUtils::showVec_RP(res,19);
// 	StringUtils::compare(madd,res,n,"Add");
}
	return 0;

}