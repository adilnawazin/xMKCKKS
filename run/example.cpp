#include "../src/HEAAN.h"

using namespace std;
using namespace NTL;

int main() {
  /*
  * Basic Parameters are in src/Params.h
  * If you want to use another parameter, you need to change src/Params.h file and re-complie this library.
  */

  // Parameters //
  long logq = 300; ///< Ciphertext modulus (this value should be <= logQ in "scr/Params.h")
  long logp = 30; ///< Scaling Factor (larger logp will give you more accurate value)
  long logn = 1; ///< number of slot is 1024 (this value should be < logN in "src/Params.h")
  long n = 1 << logn;
  long slots = n;
  long numThread = 8;
	
  // Construct and Generate Public Keys //
  srand(time(NULL));
  SetNumThreads(numThread);
  TimeUtils timeutils;
  Ring ring;
  SecretKey secretKey(ring);
  Scheme scheme(secretKey, ring);
  scheme.addLeftRotKeys(secretKey); ///< When you need left rotation for the vectorized message
  scheme.addRightRotKeys(secretKey); ///< When you need right rotation for the vectorized message
  
  // Make Random Array of Complex //
  complex<double>* mvec1 = EvaluatorUtils::randomComplexArray(slots);
  complex<double>* mvec2 = EvaluatorUtils::randomComplexArray(slots);

  // Encrypt Two Arry of Complex //
  Ciphertext cipher1;
  scheme.encrypt(cipher1, mvec1, n, logp, logq);
  Ciphertext cipher2;
  scheme.encrypt(cipher2, mvec2, n, logp, logq);
  
  // Addition //
  Ciphertext cipherAdd;
  scheme.add(cipherAdd, cipher1, cipher2);
  
  // Multiplication And Rescale //
  Ciphertext cipherMult;
  scheme.mult(cipherMult, cipher1, cipher2);
  Ciphertext cipherMultAfterReScale;
  scheme.reScaleBy(cipherMultAfterReScale, cipherMult, logp);
  
  // Rotation //
  long idx = 1;
  Ciphertext cipherRot;
  scheme.leftRotateFast(cipherRot, cipher1, idx);
  
  // Decrypt //
  complex<double>* dvec1 = scheme.decrypt(secretKey, cipher1);
  complex<double>* dvec2 = scheme.decrypt(secretKey, cipher2);
  complex<double>* dvec3 = scheme.decrypt(secretKey, cipherAdd);
  complex<double>* dvec4 = scheme.decrypt(secretKey, cipherMult);
  complex<double>* dvec5 = scheme.decrypt(secretKey, cipherMultAfterReScale);
  complex<double>* dvec6 = scheme.decrypt(secretKey, cipherRot);

  cout<< "Encrypted Array1 = " << *mvec1 <<endl;
  cout<< "Decrypted Array1 = " << *dvec1 <<endl;
  cout<< "Encrypted Array2 = " << *mvec2 <<endl;
  cout<< "Decrypted Array2 = " << *dvec2 <<endl;
  cout<< "Array1 + Array2 = "  << *dvec3 <<endl;
  cout<< "Array1 * Array2 = "  << *dvec4 <<endl;
  cout<< "Mult Check      = " << (*mvec1 * *mvec2) <<endl;
  // cout<< "Rescale After Mult = "<< *dvec5<<endl;
  // cout<< "Left Rotate Fast = " << *dvec6 <<endl;
  
  return 0;

}
