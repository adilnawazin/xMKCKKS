/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "StringUtils.h"
#include <fstream>


//----------------------------------------------------------------------------------
//   SHOW ARRAY
//----------------------------------------------------------------------------------


void StringUtils::showVec(long* vals, long size) {
	cout << "[";
	cout << vals[0];
	for (long i = 1; i < size; ++i) {
		cout << ", " << vals[i];
	}
	cout << "]" << endl;
}

void StringUtils::showVec(double* vals, long size) {
	cout << "[";
	cout << vals[0];
	for (long i = 1; i < size; ++i) {
		cout << ", " << vals[i];
	}
	cout << "]" << endl;
}

void StringUtils::showVec(complex<double>* vals, long size) {
	cout << "[";
	cout << vals[0];
	for (long i = 1; i < size; ++i) {
		cout << ", " << vals[i];
	}
	cout << "]" << endl;
}

void StringUtils::showVec_RP(complex<double>* vals, long size) {
	cout << "[";
	cout << vals[0].real();
	for (long i = 1; i < size; ++i) {
		cout << ", " << vals[i].real();
	}
	cout << "]" << endl;
}

void StringUtils::showVec(ZZ* vals, long size) {
	cout << "[";
	cout << vals[0];
	for (long i = 1; i < size; ++i) {
		cout << ", " << vals[i];
	}
	cout << "]" << endl;
}


//----------------------------------------------------------------------------------
//   SHOW & COMPARE ARRAY
//----------------------------------------------------------------------------------


void StringUtils::compare(double val1, double val2, string prefix) {
	cout << "---------------------" << endl;
	cout << "m" + prefix + ":" << val1 << endl;
	cout << "d" + prefix + ":" << val2 << endl;
	cout << "e" + prefix + ":" << val1-val2 << endl;
	cout << "---------------------" << endl;
}

void StringUtils::compare(complex<double> val1, complex<double> val2, string prefix) {
	cout << "---------------------" << endl;
	cout << "m" + prefix + ":" << val1 << endl;
	cout << "d" + prefix + ":" << val2 << endl;
	cout << "e" + prefix + ":" << val1-val2 << endl;
	cout << "---------------------" << endl;
}

void StringUtils::compare(double* vals1, double* vals2, long size, string prefix) {
	for (long i = 0; i < size; ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << vals1[i] << endl;
		cout << "d" + prefix + ": " << i << " :" << vals2[i] << endl;
		cout << "e" + prefix + ": " << i << " :" << (vals1[i]-vals2[i]) << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::compare(complex<double>* vals1, complex<double>* vals2, long size, string prefix) {
	for (long i = 0; i < size; ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << vals1[i] << endl;
		cout << "d" + prefix + ": " << i << " :" << vals2[i] << endl;
		cout << "e" + prefix + ": " << i << " :" << (vals1[i]-vals2[i]) << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::compare(double* vals1, complex<double>* vals2, long size, string prefix) {
	double e_single=0;
	double error=0;
	fstream mserec;
	for (long i = 0; i < size; ++i) {
		e_single = vals1[i]-vals2[i].real();
		// cout << "---------------------" << endl;
		// cout << "m" + prefix + ": " << i << " :" << vals1[i] << endl;
		// cout << "d" + prefix + ": " << i << " :" << vals2[i].real() << endl;
		// cout << "e" + prefix + ": " << i << " :" << e_single << endl;
		// cout << "---------------------" << endl;
		error = error + e_single; 
	}
	error = (pow(error,2))/size;
	cout<<"=========================="<<endl;
	cout << "MSE = "<<error<<endl;
	cout<<"=========================="<<endl;

	mserec.open("/home/adel/xMKCKKS/run/MSE.txt", ios::app);
		if (!mserec){
			cout<<"MSE.txt not found"<< endl;
		}
		else{
			mserec << error;
			mserec << "\n";
			mserec.close();
		}
}


void StringUtils::output(complex<double>* vals2, long size) {

	fstream outFile;
	outFile.open("/home/adel/xMKCKKS/run/Output.txt", ios::app);
	for (long i = 0; i < size; ++i) {
		if(!outFile){
		cout <<"Log File Output.txt not found" << endl;	
		}
		else{
		outFile << vals2[i].real();
		outFile << "	";
		}

	}
	outFile << "\n";
	outFile.close();		
}


void StringUtils::compare(double* vals1, double val2, long size, string prefix) {
	for (long i = 0; i < size; ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << vals1[i] << endl;
		cout << "d" + prefix + ": " << i << " :" << val2 << endl;
		cout << "e" + prefix + ": " << i << " :" << vals1[i]-val2 << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::compare(complex<double>* vals1, complex<double> val2, long size, string prefix) {
	for (long i = 0; i < size; ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << vals1[i] << endl;
		cout << "d" + prefix + ": " << i << " :" << val2 << endl;
		cout << "e" + prefix + ": " << i << " :" << vals1[i]-val2 << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::compare(double val1, double* vals2, long size, string prefix) {
	for (long i = 0; i < size; ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << val1 << endl;
		cout << "d" + prefix + ": " << i << " :" << vals2[i] << endl;
		cout << "e" + prefix + ": " << i << " :" << val1-vals2[i] << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::compare(complex<double> val1, complex<double>* vals2, long size, string prefix) {
	for (long i = 0; i < size; ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << val1 << endl;
		cout << "d" + prefix + ": " << i << " :" << vals2[i] << endl;
		cout << "e" + prefix + ": " << i << " :" << val1-vals2[i] << endl;
		cout << "---------------------" << endl;
	}
}
