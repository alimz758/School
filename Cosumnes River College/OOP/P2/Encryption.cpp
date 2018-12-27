//encrypt member functions
#include <iostream>


// include header of class Encrypt
#include "Encrypt.h"

// use the standard namespace libraries 
using namespace std;



/*===============================================
Name / Type: Encrypt / Constructor
Parameters: num
Description: stores and displays original/
encrypted data.
=================================================*/
Encrypt::Encrypt(int num)
{
	cout << "** The default constructor is called" << endl;
	cout << "   and the passed in number is: " << num << ".**" << endl << endl;
	// stores data from the num variable
	storeData(num);
	// display the original data
	displayOriginalData();
	// display the encrypted data
	displayEncryptedData();
} // end Encrypt constructor
  /*===============================================
  Name / Type: storeData / void
  Parameters: data
  Description: Stores data in the array digits
  =================================================*/
void Encrypt::storeData(int data) {
	if (data <= 0) 
	{ // if the data is <= 0, then set default to 9436
		digits[0] = 9;
		digits[1] = 4;
		digits[2] = 3;
		digits[3] = 6;
		cout << endl;
		cout << " XXX The inputed number is less than or equal to 0."<<endl;
		cout << "     The number is reset to 9436. XXX" << endl<<endl;
	}
	else {
	
		// store in the corresponding element of the arrays
		digits[3] = data % 10;
		digits[2] = (data / 10) % 10;
		digits[1] = ((data / 10) / 10) % 10;
		digits[0] = (((((data / 10) / 10) / 10) % 10));
	}
	// calls encryptData function
	encryptData();
} // end storeData function
  /*===============================================
  Name / Type: displayOriginalData / void
  Parameters: NULL
  Description: Display the 4 original elements in
  the array
  =================================================*/
void Encrypt::displayOriginalData()
{
	cout << "     The original data is:  ";
	// loop thought each element in the array 
	for (int i = 0; i < 4; i++)
	{
		// output the ith term
		cout << " " << digits[i];
	}
	cout << endl;
} // end displayOriginalData function
  /*===============================================
  Name / Type: displayEncryptedData / void
  Parameters: NULL
  Description: Display the 4 encrypted elements in
  the array
  =================================================*/
void Encrypt::displayEncryptedData()
{
	cout << "     The encrypted data is: ";
	// loop thought each element in the array 
	for (int i = 4; i < 8; i++)
	{
		// output the ith term
		cout << " " << digits[i];
	}
	cout << endl<< endl;
} // end displayEncryptedData function
  /*===============================================
  Name / Type: encryptData / void
  Parameters: NULL
  Description: This will encrypt the first 4 digits of
  the array - digits - stores them in the
  lsat 4 digits of the array
  =================================================*/
void Encrypt::encryptData()
{
	// add 7 to the number and modulus by 10 to get encrypted number
	digits[4] = ((digits[2] + 7) % 10);
	digits[5] = ((digits[3] + 7) % 10);
	digits[6] = ((digits[0] + 7) % 10);
	digits[7] = ((digits[1] + 7) % 10);
} // end encryptData function
