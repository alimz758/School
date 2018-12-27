// Date.cpp by Ali Mirabzadeh
// Date class member-function definitions.
#include <array>
#include <iostream>
#include <stdexcept>
#include "Date.h" // include Date class definition
#include "Time.h"
using namespace std;

// constructor confirms proper value for month; calls
// utility function checkDay to confirm proper value for day
Date::Date(int mn, int dy, int yr, Time defaultTime)
	:time(defaultTime)
{
   if ( mn > 0 && mn <= monthsPerYear ) // validate the month
      month = mn;
   else
   {
	   month = 1; // invalid month set to 1
	   cout << "Invalid month (" << mn << ") set to 1.\n";
   }
   year = yr; // could validate yr
   day = checkDay( dy ); // validate the day

   // output Date object to show when its constructor is called
   cout << "Date object constructor for date   ";
   print();                   
 
} // end Date constructor

//increase a day private member
void Date::increaseADay()
{
	checkDay(++day);
	if (day > 31 && month == 12) 
	{
		day = 1;
		month = 1;
		year++;
	}
}// end of the function

// It increases one second to the Time object of the Date class private data member
void Date::tick()
{
	int tempSecond = time.getSecond();
	if (tempSecond < 59)
	{
		tempSecond++;
		time.setSecond(tempSecond);
	}
	else if (tempSecond >= 59)
	{
		time.setSecond(0);
		int tempMin = time.getMinute();
		if (tempMin < 59)
		{
			tempMin++;
			time.setMinute(tempMin);
		}
		else if (tempMin >= 59)
		{
			time.setMinute(0);
			int tempHour = time.getHour();
			if (tempHour < 23)
			{
				tempHour++;
				time.setHour(tempHour);
			}
			else if (tempHour >= 23)
			{
				time.setHour(0);
				increaseADay();
			}
		}
	}
	
}//end of the function
// print Date object in form month/day/year
void Date::print() const
{
	cout << month << '/' << day << '/' << year;
	cout << "    ";
	time.printStandard();
	cout << "    ";
	time.printUniversal();
	
} // end function print

// output Date object to show when its destructor is called
Date::~Date()
{ 
   cout << "Date object destructor is called "<<endl;
   print();
   cout << endl;
} // end ~Date destructor

// utility function to confirm proper day value based on 
// month and year; handles leap years, too
unsigned int Date::checkDay( int testDay ) const
{
   static const array< int, monthsPerYear + 1 > daysPerMonth = 
      { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   // determine whether testDay is valid for specified month
   if ( testDay > 0 && testDay <= daysPerMonth[ month ] )
      return testDay;

   // February 29 check for leap year 
   if (month == 2 && testDay == 29 && (year % 400 == 0 ||
	   (year % 4 == 0 && year % 100 != 0)))
	   return testDay;

   cout << "Day (" << testDay << ") set to 1.\n";
   return 1; // leave object in consistent state if bad value
} // end function checkDay
