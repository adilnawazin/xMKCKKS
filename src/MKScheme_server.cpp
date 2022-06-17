#include <stdio.h>
#include "MKScheme_server.h"
#include <NTL/BasicThreadPool.h>
#include <NTL/ZZ.h>
#include "EvaluatorUtils.h"


MKScheme_server::MKScheme_server(SecretKey& secretKey, Ring& ring, bool isSerialized) : ring(ring), isSerialized(isSerialized) {};

MKScheme_server::~MKScheme_server() {
}

void MKScheme_server::encode(Plaintext& plain, double* vals, long n, long logp, long logq) {
	SetNumThreads(8);
	plain.logp = logp;
	plain.logq = logq;
	plain.n = n;
	ring.encode(plain.mx, vals, n, logp + logQ);
}

void MKScheme_server::encode(Plaintext& plain, complex<double>* vals, long n, long logp, long logq) {
	SetNumThreads(8);
	plain.logp = logp;
	plain.logq = logq;
	plain.n = n;
	ring.encode(plain.mx, vals, n, logp + logQ);
}

complex<double>* MKScheme_server::decode(Plaintext& plain) {
	SetNumThreads(8);
	complex<double>* res = new complex<double>[plain.n];
	ring.decode(plain.mx, res, plain.n, plain.logp, plain.logq);
	return res;
}

complex<double> MKScheme_server::decodeSingle(Plaintext& plain) {
	SetNumThreads(8);
	ZZ q = ring.qpows[plain.logq];
	complex<double> res;
	ZZ tmp = plain.mx[0] % q;
	if(NumBits(tmp) == plain.logq) tmp -= q;
	res.real(EvaluatorUtils::scaleDownToReal(tmp, plain.logp));
	tmp = plain.mx[Nh] % q;
	if(NumBits(tmp) == plain.logq) tmp -= q;
	res.imag(EvaluatorUtils::scaleDownToReal(tmp, plain.logp));

	return res;
}

ZZ* MKScheme_server::PublicKeyGeneration(SecretKey& EncKey, ZZ* axP){
	
	SetNumThreads(8);
	ZZ* bxP = new ZZ[N];
	long npP = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.mult(bxP, EncKey.sx, axP, npP, QQ);
	ring.subFromGaussAndEqual(bxP, QQ);
	return bxP;
}

ZZ* MKScheme_server::Jkeysend(ZZ* axP, ZZ* bxP, ZZ* bxP1)
{
	SetNumThreads(8);
	ZZ* bxSum = new ZZ[N];
	ring.addAndEqual(bxSum, bxP, QQ);
	ring.addAndEqual(bxSum, bxP1, QQ);
	return bxSum;
}
Key* MKScheme_server::JointKeyGeneration( ZZ* axP, ZZ* bxSum) {

	Key* keySum = new Key();
	ring.CRT(keySum->rax, axP, nprimes);
	ring.CRT(keySum->rbx, bxSum, nprimes);
	return keySum;
}

void MKScheme_server::EncryptMsg(Ciphertext& cipher, Plaintext& plain, Key* jointkey) {
	
	SetNumThreads(8);
	cipher.logp = plain.logp;
	cipher.logq = plain.logq;
	cipher.n = plain.n;
	ZZ qQ = ring.qpows[plain.logq + logQ];
	ZZ* vx = new ZZ[N];
	ring.sampleZO(vx);
	long npP = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(cipher.ax, vx, jointkey->rax, npP, qQ);
	ring.addGaussAndEqual(cipher.ax, qQ);
	ring.multNTT(cipher.bx, vx, jointkey->rbx, npP, qQ);
	ring.addGaussAndEqual(cipher.bx, qQ);
	ring.addAndEqual(cipher.bx, plain.mx, qQ);
	ring.rightShiftAndEqual(cipher.ax, logQ);
	ring.rightShiftAndEqual(cipher.bx, logQ);
	delete[] vx;
}

void MKScheme_server::AddCipherText(Ciphertext& cipherAdd, Ciphertext& cipher, Ciphertext& cipher1){
	
	SetNumThreads(8);
	ZZ q = ring.qpows[cipher.logq];
	cipherAdd.copyParams(cipher);
	ring.add(cipherAdd.ax, cipher.ax, cipher1.ax, q);
	ring.add(cipherAdd.bx, cipher.bx, cipher1.bx, q);
	// ring.add(cipherAdd.ax, cipherAdd.ax, cipher2.ax, q);
	// ring.add(cipherAdd.bx, cipherAdd.bx, cipher2.bx, q);
}

void MKScheme_server::DecryptionShare(Plaintext& plain_t, Ciphertext& cipher, SecretKey& secretKey, ZZ* cipherAdd){

	SetNumThreads(8);
	ZZ qQ = ring.qpows[plain_t.logq + logQ];
	double _sigma = 19.0;
	ZZ q = ring.qpows[cipher.logq];
	plain_t.logp = cipher.logp;
	plain_t.logq = cipher.logq;
	plain_t.n = cipher.n;
	long np = ceil((1 + cipher.logq + logN + 2)/(double)pbnd);
	ring.mult(plain_t.mx, cipherAdd, secretKey.sx, np, q);
	ring.addGaussAndEqual(plain_t.mx, qQ, _sigma);
}

void MKScheme_server::Decryption(Plaintext& plain_t, Ciphertext& cipherAdd, Plaintext& plain_t1){

	SetNumThreads(8);
// Add (mu = D1+D2+...+Dn)
	ZZ q = ring.qpows[cipherAdd.logq];
	ring.addAndEqual(plain_t.mx, plain_t1.mx, q);
	// ring.addAndEqual(plain_t.mx, plain_t2.mx, q);

//ADD C_sum0 + mu
	ring.addAndEqual(plain_t.mx, cipherAdd.bx, q);
}