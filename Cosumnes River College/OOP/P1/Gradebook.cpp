// GradeBook.cpp
// GradeBook member-function definitions. This file contains
// implementations of the member functions prototyped in GradeBook.h.
#include <iostream>
#include "GradeBook.h" // include definition of class GradeBook
using namespace std;

// constructor initializes courseName with string supplied as argument
 GradeBook::GradeBook(string name, string instrucName)
{
	setCourseName(name); // member initializer to initialize courseName 
	setInstructorName(instrucName); // member initializer to initialize intructor's name
} // end GradeBook constructor

  // function to set the course name
void GradeBook::setCourseName(string name)
{
	courseName = name; // store the course name in the object
} // end function setCourseName

  //function to set the instructor name
void GradeBook::setInstructorName(string instrucName)
{
	instructorName = instrucName; //store the instructor's name in the object
}// end function setInstructorName

 // function to get the course name
string GradeBook::getCourseName() const
{
	return courseName; // return object's courseName
} // end function getCourseName

  //function to get the instructor name
string GradeBook::getInstructorName()const
{
	return instructorName; //return object's instructorName
}//ebd fuction getInstructorName

 // display a welcome message to the GradeBook user
void GradeBook::displayMessage() const
{
	// call getCourseName to get the courseName
	cout << "Welcome to the grade book for\n" << getCourseName()
		<< "!" << endl;
	//cal getInstructorName to the Instructor name
	cout << "This course is presented by: " << getInstructorName() << endl;
} // end function displayMessage

