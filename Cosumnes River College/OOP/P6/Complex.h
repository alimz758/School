// Complex.h by Ali Mirabzadeh

#include <iostream> // allows program to output data to the screen
using namespace std; // program uses names from the std namespace

class Complex
{
public:
	Complex(double = 0.0, double = 0.0); // constructor
	const bool operator==(const Complex &rhs) const; // equal comparison operator
	friend ostream &operator<<(ostream&output, const Complex&complex); // extraction operator
private:
	double real; // real part
	double imaginary; // imaginary part
}; // end class Complex
