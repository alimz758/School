// Complex.cpp by Ali Mirabzadeh
// Complex class member-function definitions.
#include <iostream>
#include <iomanip>
#include "Complex.h" // Complex class definition
using namespace std;

// Constructor
Complex::Complex( double realPart, double imaginaryPart ) 
   : real( realPart ),
   imaginary( imaginaryPart ) 
{ 
   // empty body
} // end Complex constructor

// addition operator
Complex Complex::operator+( const Complex &operand2 ) const
{
   return Complex( real + operand2.real, 
      imaginary + operand2.imaginary );
} // end function operator+

// subtraction operator
Complex Complex::operator-( const Complex &operand2 ) const
{
   return Complex( real - operand2.real, 
      imaginary - operand2.imaginary );
} // end function operator-

//Multipication operator
Complex Complex::operator*(const Complex &operand) const
{
	return Complex(((real*operand.real)-(imaginary * operand.imaginary)), ((real *operand.imaginary) +(imaginary*operand.real))) ;
}//end of function

//Division operator
Complex Complex::operator/(const Complex & operand) const
{
	if ((pow(operand.real, 2) + pow(operand.imaginary, 2)) != 0)
	{
		return Complex(((real * operand.real) + (imaginary * operand.imaginary)) / pow(operand.real, 2) + pow(operand.imaginary, 2), ((imaginary * operand.real) - (real* operand.imaginary)) / pow(operand.real, 2) + pow(operand.imaginary, 2));
	}
	else
	{
		return Complex (INFINITY,INFINITY);
	}
}

//set equal-to function
const Complex &Complex::operator=(const Complex &number)
{
	real = number.real;
	imaginary = number.imaginary;
	return *this;
}//end of function

//not-equal-to operator
bool Complex::operator!=(const Complex &complex) const
{
	if ((real != complex.real) || (imaginary != complex.imaginary))
		return true;
	else
		return false;
}//end of function

//equal-to operator
bool Complex::operator==(const Complex &complex) const
{
	if ((real == complex.real) && (imaginary == complex.imaginary))
		return true;
	else
		return false;
}//end of function
	
//end of function 

//output operator
ostream & operator<<(ostream &output, const Complex &number)
{
	output << "<" << number.real << ", " << number.imaginary << ">";
	return output; //returning the output
}//end of function

//input operator
istream & operator>>(istream &input, Complex &number)
{
	cout << "(Performing >> operator overloading.)\n? ";
	input.ignore(); //ignore <
	input >> setw(2) >> number.real; // getting the real number
	input.ignore(2); // ignore "," and a space
	input >> setw(2) >> number.imaginary; //getting 
	return input; //return the inputted complex number
}// end of function
