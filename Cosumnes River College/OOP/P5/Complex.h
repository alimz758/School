// Complex.h by Ali Mirabzadeh
// Complex class definition.
#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <string>

using namespace std;

class Complex
{
public:
   explicit Complex( double = 0.0, double = 0.0 ); // constructor
   Complex operator+( const Complex & ) const; // addition
   Complex operator-( const Complex & ) const; // subtraction
   Complex operator* (const Complex &) const; //Multipication 
   Complex operator/ (const Complex &) const; //Multipication 
   const Complex & operator=(const Complex &) ;// set-equal-to operator
   bool operator!=(const Complex &) const; //not-equal-to comparison
   bool operator==(const Complex &) const;//equal-to comparison
   friend ostream &operator<<(ostream &, const Complex &);
   friend istream &operator>>(istream &, Complex &);
  
private:
   double real; // real part
   double imaginary; // imaginary part
}; // end class Complex

#endif
