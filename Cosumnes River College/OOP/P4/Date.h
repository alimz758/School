// Date.h by Ali Mirabzadeh
// Date class definition; Member functions defined in Date.cpp
#include "Time.h"
#ifndef DATE_H
#define DATE_H

class Date 
{
public:
   static const unsigned int monthsPerYear = 12; // months in a year
   explicit Date( int = 1, int = 1, int = 1900, Time=Time()); // default constructor
   void increaseADay(); // increase the day by one if data is valid
   void tick(); // increase time by one second if data is valid
   void print() const; // print date in month/day/year format
   ~Date(); // provided to confirm destruction order
private:
   unsigned int month; // 1-12 (January-December)
   unsigned int day; // 1-31 based on month
   unsigned int year; // any year
   Time time; // compostion : member function

   // utility function to check if day is proper for month and year
   unsigned int checkDay( int ) const; 
}; // end class Date

#endif
