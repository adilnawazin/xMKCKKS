/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_MKScheme_server_H_
#define HEAAN_MKScheme_server_H_
#include "Plaintext.h"
#include "Key.h"
#include "Ciphertext.h"
#include "Ring.h"
#include "SecretKey.h"
#include "SerializationUtils.h"

using namespace std;
using namespace NTL;

class MKScheme_server {
public:
	Ring& ring;
	bool isSerialized;

	MKScheme_server(SecretKey& secretKey, Ring& ring, bool isSerialized = false);
    
    virtual ~MKScheme_server();
	
	void encode(Plaintext& plain, complex<double>* vals, long n, long logp, long logq);

	void encode(Plaintext& plain, double* vals, long n, long logp, long logq);

	complex<double>* decode(Plaintext& plain);

	complex<double> decodeSingle(Plaintext& plain);

	ZZ* PublicKeyGeneration(SecretKey& EncKey, ZZ* axP);

	ZZ* Jkeysend(ZZ* axP, ZZ* bxP, ZZ* bxP1, ZZ* bxP2);

	Key* JointKeyGeneration(ZZ* axP, ZZ* bxSum);

	void EncryptMsg(Ciphertext& cipher, Plaintext& plain, Key* jointkey);

	void AddCipherText(Ciphertext& cipherAdd, Ciphertext& cipher, Ciphertext& cipher1, Ciphertext& cipher2);

	void DecryptionShare(Plaintext& plain_t, Ciphertext& cipher, SecretKey& secretKey, ZZ* cipherAdd);

	void Decryption(Plaintext& plain_t, Ciphertext& cipherAdd, Plaintext& plain_t1, Plaintext& plain_t2);
    
	void ZZ_Receive(ZZ* receive, int socket, string op);

	void ZZ_Send(ZZ* send, int socket, string op);
};

#endif
