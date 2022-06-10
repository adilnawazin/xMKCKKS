/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "TimeUtils.h"
#include <iostream>
#include <sys/time.h>
#include <string>
#include <fstream>

TimeUtils::TimeUtils() {
	timeElapsed = 0;
}

void TimeUtils::start(string msg) {
	std::cout << "------------------------------------" << std::endl;
	std::cout <<"Start " + msg << std::endl;
	gettimeofday(&startTime, 0);
}

void TimeUtils::stop(string msg) {
	fstream timerec;
	gettimeofday(&stopTime, 0);
	timeElapsed = (stopTime.tv_sec - startTime.tv_sec) * 1000.0;
	timeElapsed += (stopTime.tv_usec - startTime.tv_usec) / 1000.0;
	timerec.open("/home/adel/xMKCKKS/run/timerec.txt",ios::app);
	if(!timerec){
		cout<<"Timerec File not found"<<endl;
	}
	else{
		if (msg == "Data Import Complete"){
		timerec << "data_import_time ";
		timerec << timeElapsed;
		timerec << "\n";
		}
		else if (msg == "Encoding"){
			timerec << "encode_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
		else if (msg == "Public Key Generation"){
			timerec << "pKey_Gen_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Joint Key Generation"){
			timerec << "Keysum ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Encryption"){
			timerec << "encryption_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Ciphertext Addition"){
			timerec << "ct_add_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Decryption Share"){
			timerec << "dec_share_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Merge Decryption and Decode"){
			timerec << "merge_n_decode_time ";
			timerec << timeElapsed;
			timerec << "\n";
			}
			else{
			std::cout<< "Key Error: msg cannot be saved in timerec"<<std::endl;
			} 
		}
	timerec.close();
	std::cout<< msg +  " Time = "<< timeElapsed << " ms" << std::endl;
	std::cout<< "------------------------------------" << std::endl;
}

