// Date.cpp by Ali Mirabzadeh

#include <iostream> 
#include <iomanip>
#include <string>
#include "Date.h" // Date class definition
using namespace std; 

// give each month a name
string Date::monthName[13] = { "invalid", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

// << print operator overload
ostream &operator<<(ostream &output, const Date &date) // takes numbers to print
{
	cout << date.monthName[date.month] << " " << date.day << ", " << date.year;
	return output;
} //end overloaded extraction operator

// constructor confirms proper value for month; calls
Date::Date(int mn, int dy, int yr)
{
	if (mn > 0 && mn <= monthsPerYear) // validate the month
		month = mn; // sets input to private variable
	else
		month = 1; // sets month back to zero
	year = yr; // sets input to variable
	day = checkDay(dy); // validate the day

} // end Date constructor

// utility function checkDay to confirm proper value for day
int Date::checkDay(int testDay) const
{
	static const unsigned int daysPerMonth[(monthsPerYear + 1)] = // number of days in each month
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	// determine whether testDay is valid for specified month
	if ((testDay > 0) && (testDay <= daysPerMonth[month]))
	{ // if proper days in month pass the date back
		return testDay;
	}
	else
	{
		return 1;
	}
	// February 29 check for leap year 
	if (month == 2 && testDay == 29 && (year % 400 == 0 || // if failed previous if statement check for leap year
		(year % 4 == 0 && year % 100 != 0))) {
		return testDay;
	}
	return 1; // if day is too large for month then set it back to 1
} // end function checkDay

  // takes complex date number places it into object
const bool Date::operator==(const Date &rhs) const
{
	if ((day == rhs.day) && (month == rhs.month) && (year == rhs.year)) {
		return true;
	}
	return false;
} // end overloaded== operator
