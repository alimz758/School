//Encrypt class

/*
This is the Encrypt class header file. Here we declare
member functions used in Encrypt.cpp and the member
functions will be defined in Encrypt.cpp.
*/

using namespace std;

class Encrypt
{
public:
	
	// declare default constructor
	Encrypt(int);
	// declare storeData function with parameter int
	void storeData(int);
	// declare displayOriginalData function with no parameters
	void displayOriginalData();
	// declare displayEncryptedData function with no parameters
	void displayEncryptedData();
	// declare encryptData function with no parameters
	void encryptData();

private: // make variables only available to Encrypt.cpp
		 // array that stores the original and encrypted data
	int digits[8];
}; // end Encrypt class header#pragma once
