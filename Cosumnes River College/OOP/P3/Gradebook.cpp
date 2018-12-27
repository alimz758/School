
// FILE: GradeBook.cpp

// Description: Member-function definitions for class GradeBook that
//				uses a switch statement to count A, B, C, D and F grades.

//Include input/output stream, and stream manipulators
#include <iostream>
#include <iomanip> 

#include "GradeBook.h" // include definition of class GradeBook

// constructor initializes courseName with string supplied as argument;
// initializes counter data members to 0
GradeBook::GradeBook(string name)
{
	cout << "The Grade Book Constructor is called" << endl;
	initializeData(); // call initializeData function
	setCourseName(name); // name to setCourseName
	displayMessage(); // call displayMessage function
	cout << "The Grade Book, " << getCourseName() << ", contains" << endl << endl << endl;
	displayGradeReport(); // call displayGradeReport function
	cout << "*****The end of Grade Book Constructor.*****" << endl << endl << endl;
} // end GradeBook constructor

  // function to initialize counter data members to 0
void GradeBook::initializeData()
{
	courseName = ""; // initialize courseName to an empty string
	countGrades[0] =  // initialize a count to 0
		countGrades[1] =  // initialize b count to 0
		countGrades[2] =  // initialize c count to 0
		countGrades[3] =  // initialize d count to 0
		countGrades[4] =  // initialize f count to 0
		countGrades[5] = 0;  // initialize error count to 0
	letterGrades[ARRAY_SIZE] = NULL; // initialize all of the array to empty strings
} // end function initializeData

  // function to set the course name; limits name to 25 or fewer characters
void GradeBook::setCourseName(string name)
{
	if (name.length() <= 30) // if name has 30 or fewer characters
		courseName = name; // store the course name in the object
	else // if name is longer than 30 characters
	{ // set courseName to first 30 characters of parameter name
		courseName = name.substr(0, 30); // select first 30 characters
		cout << setw(8) << " " << "Name: \"" << name << "\"" << endl;
		cout << setw(8) << " " << "exceeds maximum length (30)." << endl << endl;
		cout << setw(8) << " " << "Limiting courseName to first 30 characters.\n" << endl;
	} // end if...else
} // end function setCourseName

  // function to retrieve the course name
string GradeBook::getCourseName()
{
	return courseName;
} // end function getCourseName

  // display a welcome message to the GradeBook user
void GradeBook::displayMessage()
{
	// this statement calls getCourseName to get the 
	// name of the course this GradeBook represents
	cout << setw(8) << " " << "Welcome to the grade book for" << endl;
	cout << setw(8) << " " << getCourseName() << "!" << endl << endl;
} // end function displayMessage

  // input arbitrary number of grades from user; update grade counter
void GradeBook::inputGrades()
{
	cout << setw(8) << " " << "Enter letter grades." << endl;
	cout << setw(8) << " " << "Or Enter the EOF character to end input." << endl;
	cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
	
	// loop until user types end-of-file key sequence
	for (int grade = 0; ((letterGrades[grade] = cin.get()) != EOF) && (grade < 100); grade++)
	{
		
		switch (letterGrades[grade]) // switch statement nested in while
		{
		case 'A': case'a': // grade was uppercase A / lowercase a
			letterGrades[grade] = toupper(letterGrades[grade]); // convert lowercase to uppercase
			countGrades[0]++; // increment a count
			cout << setw(8) << " " << "Enter letter grades." << endl;
			cout << setw(8) << " " << "-->     Or Enter the EOF character to end input." << endl;
			cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
			
			break; // exit switch

		case 'B': case'b': // grade was uppercase B / lowercase b
			letterGrades[grade] = toupper(letterGrades[grade]); // convert lowercase to uppercase
			countGrades[1]++; // increment b count
			cout << setw(8) << " " << "Enter letter grades." << endl;
			cout << setw(8) << " " << "-->     Or Enter the EOF character to end input." << endl;
			cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
			
			break; // exit switch

		case 'C': case'c': // grade was uppercase C / lowercase c
			letterGrades[grade] = toupper(letterGrades[grade]); // convert lowercase to uppercase
			countGrades[2]++; // increment c count
			cout << setw(8) << " " << "Enter letter grades." << endl;
			cout << setw(8) << " " << "-->     Or Enter the EOF character to end input." << endl;
			cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
			
			break; // exit switch

		case 'D': case'd': // grade was uppercase A / lowercase a
			letterGrades[grade] = toupper(letterGrades[grade]); // convert lowercase to uppercase
			countGrades[3]++; // increment d count
			cout << setw(8) << " " << "Enter letter grades." << endl;
			cout << setw(8) << " " << "-->     Or Enter the EOF character to end input." << endl;
			cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
			
			break; // exit switch

		case 'F': case'f': // grade was uppercase A / lowercase a
			letterGrades[grade] = toupper(letterGrades[grade]); // convert lowercase to uppercase
			countGrades[4]++; // increment f count
			cout << setw(8) << " " << "Enter letter grades." << endl;
			cout << setw(8) << " " << "-->     Or Enter the EOF character to end input." << endl;
			cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
			
			break; // exit switch

		case '\n': // ignore newlines,
		case '\t': // tabs,
		case ' ': // and spaces in input
			grade--; //to ignore the whitespace index
			break; // exit switch

		default: // catch all other characters and mention that incorrect grade was entered
			countGrades[5]++; // increment error count
			cout << setw(8) << " " << "XXXX An incorrect letter grade entered.  XXXX" << endl;
			cout << setw(8) << "\"" << letterGrades[grade] << "\"" << " is not a proper letter grade." << endl << endl;
			cout << setw(8) << " " << "Enter letter grades." << endl;
			cout << setw(8) << " " << "-->     Or Enter the EOF character to end input." << endl;
			cout << setw(8) << " " << "(Use Ctl + D, or Ctl + Z)" << endl;
			
		} // end switch
	} // end for
} // end function inputGrades

  // function to display the grade array in table form
void GradeBook::displayInputs()
{
	cout << "The data entered is listed at the following:" <<endl;
	for (int a = 0; a < countGrades[0] + countGrades[1] + countGrades[2] + countGrades[3] + countGrades[4] + countGrades[5];)
	{
		cout << "[" << a << "]->" << right << setw(2) << letterGrades[a] << "\t";
		if (((a+1) % 4) == 0) // if a is divisible by 4, start new line
		{
			cout << endl;
		} // end if
		++a;
		
	}; // end for
} // end function displayInputs

  // display a report based on the grades entered by user
void GradeBook::displayGradeReport()
{
	cout << endl << endl << "The total number of data entered is " << countGrades[0] + countGrades[1] + countGrades[2] + countGrades[3] + countGrades[4] + countGrades[5];
	cout << "\nThe total number of students receive grades is " << countGrades[0] + countGrades[1] + countGrades[2] + countGrades[3] + countGrades[4] << endl;
	cout << "Number of students who received each letter grade:"
		<< "\nA:       " << countGrades[0] // display number of A grades
		<< "\nB:       " << countGrades[1] // display number of B grades
		<< "\nC:       " << countGrades[2] // display number of C grades
		<< "\nD:       " << countGrades[3] // display number of D grades
		<< "\nError:   " << countGrades[5] // display number of incorrect grades
		<< "\nF:       " << countGrades[4] // display number of F grades
		<< endl;

	if (countGrades[0] + countGrades[1] + countGrades[2] + countGrades[3] + countGrades[4] != 0) // if grade count doesnt = 0
	{
		// call function calculateGrade and display the calculated GPA
		cout << "\nThe class average is: " << fixed << setprecision(2) << calculateGrade()
			<< endl << endl << endl;
	}
	else
	{
		cout << "\nThe class average is: 0.0" << endl << endl;
	}
} // end function displayGradeReport

  // function to calculate the class average (GPA)
double GradeBook::calculateGrade()
{
	// gpa formula
	return ((A_RATE * countGrades[0] + B_RATE * countGrades[1] + C_RATE * countGrades[2] + countGrades[3]) /
		(countGrades[0] + countGrades[1] + countGrades[2] + countGrades[3] + countGrades[4]));
} // end function calculateGrade

  //call deconstructor
GradeBook::~GradeBook()
{
	cout << "Destructor is called" << endl << endl;
	string name;
	name = ""; // set name to empty
	setCourseName(name); // clear name
} // end GradeBook Destructor

 
