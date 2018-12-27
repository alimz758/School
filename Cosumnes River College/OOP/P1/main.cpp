//CISP400V10A1.cpp 
// Test program for modified GradeBook class.

#include <iostream>
using std::cout; 
using std::endl;

// include definition of class GradeBook from GradeBook.h
#include "GradeBook.h"

// function main begins program execution
int main()
{
   // create a GradeBook object; pass a course name and instructor name
   GradeBook gradeBook("CISP400 Object Oriented Programming with C++", "Professor Sonny" );

   // display initial value of instructorName of GradeBook object
   cout << "\n\n  CRC grade book instructor name is: " 
      << gradeBook.getInstructorName() << "\n\n"; 

   // modify the instructorName using set function
   gradeBook.setInstructorName( "Professor Huang" );

   // display new value of instructorName
   cout << "  new CRC grade book instructor name is: " 
      << gradeBook.getInstructorName() << "\n\n";

   // display welcome message and instructor's name
   gradeBook.displayMessage(); 
   system("PAUSE");

   return 0; // indicate successful termination
} // end main


