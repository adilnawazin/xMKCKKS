#include<string>  
#include<cstring>
#include "../src/HEAAN.h"
#include "NTL/ZZ.h"
#include <sstream>

using namespace std;

// string numberToString(ZZ* axP)
// {
//     char str[];
// }


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
	// cout << "Value of vector = " << *mvec << endl;
	// cout<< "successfully imported data from party 0" << endl;

	double* mvec1 = EvaluatorUtils::randomRealArray(n);
	inFile.open("/home/adel/heaan_test/doc_T1_"+round+"_"+trial+".txt");
	i=0;
	if (!inFile) {
	cout << "Unable to open file doc_T1_"+round+"_"+trial<<endl;
	exit(1);
	}
	while (inFile >> x) {
	double tmp;
	tmp = x;
	mvec1[i] = tmp;
	i++;
	}
	inFile.close();
	// cout << "Value of vector = " << *mvec << endl;
	// cout<< "successfully imported data from party 0" << endl;

	double* mvec2 = EvaluatorUtils::randomRealArray(n);
	inFile.open("/home/adel/heaan_test/doc_T2_"+round+"_"+trial+".txt");
	i=0;
	if (!inFile) {
	cout << "Unable to open file doc_T2_"+round+"_"+trial<<endl;
	exit(1);
	}
	while (inFile >> x) {
	double tmp;
	tmp = x;
	mvec2[i] = tmp;
	i++;
	}
	inFile.close();
	// cout << "Value of vector = " << *mvec << endl;
	// cout<< "successfully imported data from party 0" << endl;
	timeutils.stop("Data Import Complete");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//					Encode Data (msg(vector) --> plaintext)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	timeutils.start("Encode");
	// SetNumThreads(8);
	Ring ring;
	SecretKey secretKey(ring);
	Plaintext plain;
	MKScheme scheme(secretKey, ring);
	scheme.encode(plain, mvec, n, logp, logq);

	SecretKey secretKey1(ring);
	Plaintext plain1;
	MKScheme scheme1(secretKey1, ring);
	scheme1.encode(plain1, mvec1, n, logp, logq);

	SecretKey secretKey2(ring);
	Plaintext plain2;
	MKScheme scheme2(secretKey2, ring);
	scheme2.encode(plain2, mvec2, n, logp, logq);
	timeutils.stop("Encoding");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Publickey Gen (Each User generates its PK i.e. (b = -s.a+e))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	timeutils.start("Public Key Generation");
	ZZ* axP = new ZZ[N];										// Pointer to a
	ring.sampleUniform2(axP, logQQ);							//Generate value for a
	// cout << "orgnl axP: "<< axP[1] << endl;						// print a
	// cout << "\n" << endl;
	ZZ* axP1  = new ZZ[N];										// initialize a pointer on remote machine for a
	stringstream stream;										// buffer for data (recv, send)
	for (i= 0; i<N; i++)
	{
	stream = stringstream();									// clear buffer
    stream << axP[i];											// copy a[i] to buffer
	axP1[i] = conv<ZZ>(stream.str().c_str()); 					// convert and assign received a[i]	
	}	
	ZZ* pkey = scheme.PublicKeyGeneration(secretKey, axP);		// Generate key on server using a
	// cout <<"Encryption Key for server: " <<*pkey << endl;		// Print server's encryption key
	// cout << "\n" << endl;
	ZZ* pkey1 = scheme.PublicKeyGeneration(secretKey1, axP1);	// Generate Key on user using a (that was converted to char and back)
	// cout <<"Encryption Key for User-A: " << *pkey1<< endl;		// Print user-A encryption key
	// cout << "\n" << endl;
	ZZ* pkey2 = scheme.PublicKeyGeneration(secretKey2, axP);	//Generate key on another user 
		// cout <<"Encryption Key for User-B: " << *pkey2<< endl;		// Print user-B encryption key
		// cout << "\n" << endl;
	timeutils.stop("Public Key Generation");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Joint Public Key Generation at Server (b' = b1+b2+...+bn, e)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	timeutils.start("Joint Key Generation");
	Key* jkey= scheme.JointKeyGeneration(axP ,pkey, pkey1, pkey2);
	// uint64_t* rax = new uint64_t;
	// cout << "jkey -> rax = " << jkey->rax<<endl;
	// cout << "*(jkey ->rax) = " << *(jkey->rax)<< endl;
	// cout << "*(rax) = jkey->rax"<< *rax << endl; 
	timeutils.stop("Joint Key Generation");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Encryption at Device D(cipher = (v.b' + m + e) , (v.a + e))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Encrypt message 1 ---> n
	timeutils.start("Encryption");
	Ciphertext cipher;
	scheme.EncryptMsg(cipher, plain, jkey);
	
	Ciphertext cipher1;
	scheme1.EncryptMsg(cipher1, plain1, jkey);
	// Ciphertext ciphertest;
	// ciphertest.ax = cipher1.ax;
	// ciphertest.bx = cipher1.bx;
	// for(i=0;i<N;i++)
	// {
	// cout<< "ciphertest.ax["<<i<<"]: "<<ciphertest.ax[i]<<endl;
	// cout<< "ciphertest.bx["<<i<<"]: "<<ciphertest.bx[i]<<endl;
	// }

	Ciphertext cipher2;
	scheme2.EncryptMsg(cipher2, plain2, jkey);
	timeutils.stop("Encryption");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		Cipher Add(C_Sum0, C_sum1 = SUM(v.b' + m + e), SUM(v.a + e))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	timeutils.start("Ciphertext Addition");
	Ciphertext cipherAdd;
	scheme.AddCipherText(cipherAdd, cipher, cipher1, cipher2);
	timeutils.stop("Ciphertext Addition");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		Part Dec (D_i = s.C_sum1)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	timeutils.start("Decryption Share");
	Plaintext plain_t;
	scheme.DecryptionShare(plain_t, cipher, secretKey, cipherAdd.ax);
	// cout<<"-----"<<*plain_t.mx<<endl;
	// complex<double>* dec1 = scheme.decode(plain_t);
	// StringUtils::showVec_RP(dec1,10);

	Plaintext plain_t1;
	scheme1.DecryptionShare(plain_t1, cipher1, secretKey1, cipherAdd.ax);
	// cout<<"-----"<<*plain_t1.mx<<endl;

	Plaintext plain_t2;
	scheme2.DecryptionShare(plain_t2, cipher2, secretKey2, cipherAdd.ax);
	// cout<<"-----"<<*plain_t2.mx<<endl;	
	timeutils.stop("Decryption Share");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		 Merge Decryption (m = C_sum0 + SUM(D_i) + e)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	timeutils.start("Merge Decryption and Decode");
	scheme.Decryption(plain_t, cipherAdd, plain_t1, plain_t2);
	complex<double>* res=scheme.decode(plain_t);
	timeutils.stop("Merge Decryption and Decode");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Compare Results
	double* madd = new double[n];

	for(long i = 0; i < n; i++) {
		madd[i] = mvec[i] + mvec1[i] + mvec2[i];
	}
	cout<< "========================="<<endl;
	cout<< "G Truth : ";
	StringUtils::showVec(madd,19);
	cout << "========================"<<endl;

	cout<<"Output  : ";
	StringUtils::showVec_RP(res,19);
	StringUtils::compare(madd,res,n,"Add");
	// int vec= stoi(vector_size);
	// StringUtils::output(res,vec);
	//end of decryption
}
	return 0;

}