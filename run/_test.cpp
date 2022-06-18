/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include<string>  
#include "../src/HEAAN.h"
/**
  * This file is for test HEAAN library
  * You can find more in src/TestScheme.h
  * "./TestHEAAN Encrypt" will run Encrypt Test
  * There are Encrypt, EncryptSingle, Add, Mult, iMult, RotateFast, Conjugate Tests
  */
int main(int argc, char **argv) {

	long logq = 800; ///< (Ciphertext Modulus) should be less than logQ in params.h
	long logp = 40; ///< (Scaling) message will be quantized by multiplying 2^logp (larger value --> more accuracy)
	long logn = 15; ///< (The number of slots) should be less than logN in params.h

//----------------------------------------------------------------------------------
//   STANDARD TESTS
//----------------------------------------------------------------------------------

	if(string(argv[1]) == "Encrypt") {
		//arg1 and arg2 = str(rnd)+str(trial)
	 	std::string arg1(argv[2]);
		std::string arg2(argv[3]);
		std::string arg3(argv[4]);
		TestScheme::testEncrypt(logq, logp, logn,arg1,arg2, arg3);  
	}

return 0;
}
