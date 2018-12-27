// CISP400V10A4.cpp
#include <iostream> 
using namespace std;

#include "Time.h" // include Time class definition
#include "Date.h" // include Date class definition

const int MAX_TICKS = 30000;

int main()
{
	Time t(23, 59, 58);// create a time object

    Date d(12, 31, 2017, t); // create date object

   // output Time object t's values 
   for ( int ticks = 1; ticks < MAX_TICKS; ++ticks ) 
   {
      d.print(); // invokes print 
	  cout << endl;
      d.tick(); // invokes function tick
   } // end for
   d.~Date();// call Date destructor
system("PAUSE");
   return 0;
} // end main
