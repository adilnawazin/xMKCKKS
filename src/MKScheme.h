/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_MKScheme_H_
#define HEAAN_MKScheme_H_
#include "Plaintext.h"
#include "Key.h"
#include "Ciphertext.h"
#include "Ring.h"
#include "SecretKey.h"
#include "SerializationUtils.h"

using namespace std;
using namespace NTL;

class MKScheme {
public:
	Ring& ring;
	bool isSerialized;

	MKScheme(SecretKey& secretKey, Ring& ring, bool isSerialized = false);
	
	void encode(Plaintext& plain, complex<double>* vals, long n, long logp, long logq);

	void encode(Plaintext& plain, double* vals, long n, long logp, long logq);

	complex<double>* decode(Plaintext& plain);

	complex<double> decodeSingle(Plaintext& plain);

	ZZ* PublicKeyGeneration(SecretKey& EncKey, ZZ* axP);

	Key* JointKeyGeneration(ZZ* axP, ZZ* bxP, ZZ* bxP1, ZZ* bxP2);

	void EncryptMsg(Ciphertext& cipher, Plaintext& plain, Key* jointkey);

	void AddCipherText(Ciphertext& cipherAdd, Ciphertext& cipher, Ciphertext& cipher1, Ciphertext& cipher2);

	void DecryptionShare(Plaintext& plain_t, Ciphertext& cipher, SecretKey& secretKey, Ciphertext& cipherAdd);

	void Decryption(Plaintext& plain_t, Ciphertext& cipherAdd, Plaintext& plain_t1, Plaintext& plain_t2);
    
};

#endif
