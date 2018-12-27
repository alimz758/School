// Date.h by Ali Mirabzadeh

#include <iostream> // allows program to output data to the screen
#include <string> 
using namespace std; // program uses names from the std namespace

class Date
{
public:
	static string monthName[13];
	static const int monthsPerYear = 12; // months in a year
	Date(int = 1, int = 1, int = 1900); // constructor
	const bool operator==(const Date &rhs) const; // ==overloadcompares objects, returns true/false
private:
	friend ostream &operator<<(ostream &, const Date &); // <<overload print, prints out imaginary number using only object name
	int month, day, year;
	int checkDay(int testDay) const; // utility function to check if day is proper for month and year
}; // end class Date
#pragma once
