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
		if (msg == "Data Import"){
		timerec << "data_import_time ";
		timerec << timeElapsed;
		timerec << "\n";
		}
		else if (msg == "Encode"){
			timerec << "encode_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
		else if (msg == "Send AXP to Users"){
			timerec << "send_axp_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Receive Key from Users"){
			timerec << "recv_pkey_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Send Joint Key to Users"){
			timerec << "send_jkey_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Receive CipherText from the Users"){
			timerec << "recv_ctext_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Encryption"){
			timerec << "encryption_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "CipherAdd to devices"){
			timerec << "send_ct_add_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Receive Partial Decryption Share from Users"){
			timerec << "recv_dec_share_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Receive AXP from Server"){
			timerec << "recv_axp_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}		
			else if (msg == "Send Key to server"){
			timerec << "send_pkey_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}		
			else if (msg == "Receive Joint Key from server"){
			timerec << "recv_jkey_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}	
			else if (msg == "Send Ciphertext to Server"){
			timerec << "send_ct_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}	
			else if (msg == "Receive CipherAdd from server"){
			timerec << "recv_ctadd_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Send Partial Decryption Share to server"){
			timerec << "send_partdec_time ";
			timerec << timeElapsed;
			timerec << "\n"; 
		}
			else if (msg == "Send Decryption Share to client"){
			timerec << "send_dec_time ";
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

