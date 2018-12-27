// GradeBook.h
// GradeBook class definition. This file presents GradeBook's public 
// interface without revealing the implementations of GradeBook's member
// functions, which are defined in GradeBook.cpp.
#include <string> // class GradeBook uses C++ standard string class

using namespace std;
// GradeBook class definition
class GradeBook
{
public:
   explicit GradeBook( string, string); // constructor initialize courseName
   void setCourseName( string ); // sets the course name
   void setInstructorName(string); // sets the instructor's name
   string getCourseName() const; // gets the course name
   string getInstructorName() const; // gets the instructor's name
   void displayMessage() const; // displays a welcome message
private:
   string courseName; // course name for this GradeBook
   string instructorName; // Instructor's name for this Gradebook
}; // end class GradeBook  



