#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

const int studentMax=3;
const int assignMax=5;

bool validate(short int grade[][assignMax]);
void printGradeScores( short int grade[][assignMax]);
void printGradeLetters(short int grade[][assignMax]);
int	getStudentTotalScores(short int grade[][assignMax],int );
float getStudentAverageScore(short int grade[][assignMax], int );
char getStudentAverageLetter(short int grade[][assignMax], int );
int getAssignmentTotalScores( short int grade[][assignMax], int );
float getAssignmentAverageScore(short int grade[][assignMax], int );
int getAssignmentMaxScore( short int grade[][assignMax], int );
int getAssignmentMinScore(short int grade[][assignMax], int );






int main()
{
	int  totalscore, assignTotalScore, assignMaxScore,assignMinScore;
	float avgStudentScore, avgAssignScore;
	char avgStudentLetter;

	
	short int grade[studentMax][assignMax]={{34,99,84,35,87},
								  		    {100,72,78,56,88},
								  		    {95,62,33,79,69}};
		
		
			printGradeScores(grade); // to print the score
			printGradeLetters(grade);// to print the letter grade
		
			
				int studentNum = 1;
				
				totalscore=getStudentTotalScores(grade,studentNum); // to get the total score for each student
				avgStudentScore=getStudentAverageScore(grade,studentNum);// to get the average score for each student
				avgStudentLetter=getStudentAverageLetter(grade, studentNum);// to get the average letter grade for each student
				cout << "The total   score for student " << (studentNum ) << " is: ";
				cout << totalscore << endl;
		
				cout << "The average Score for student " << (studentNum ) << " is: ";
				cout << fixed  <<setprecision(2) << avgStudentScore<< endl;
	
			
				cout << "The average letter grade for student " << (studentNum) << " is: ";
				cout << avgStudentLetter<< endl<< endl;
			
		
				int assignNum=3;
			
				assignTotalScore=getAssignmentTotalScores(grade,assignNum );// to get the total assignment score for each assignment
				avgAssignScore=getAssignmentAverageScore(grade, assignNum); // to get the average assignment score for each assignment
				assignMaxScore=getAssignmentMaxScore(grade, assignNum);
				assignMinScore=getAssignmentMinScore(grade, assignNum);
			
				cout << "The total   score for the assignment # " ;
				cout << (assignNum) << " is: " << assignTotalScore<< endl;
		
	
				cout << "The average score for the assignment # " ;
				cout << (assignNum) << " is: " << fixed << setprecision(2)<< avgAssignScore << endl;
		
		
				cout << "The maximum grade for the assignment # ";
				cout<< (assignNum) << " is: " << assignMaxScore<< endl;
			
		
				cout << "The minimum grade for the assignment # ";
				cout << (assignNum) << " is: " << assignMinScore<< endl<< endl;		
			
	

		
}
bool validate(short int grade[studentMax][assignMax])// for validating the grades
{
	for (int x=0;x<studentMax;x++)
		{
			for(int y=0;y<assignMax;y++)
			{
				if (grade[x][y]<0 || grade[x][y]>100)// validating the grades
				{
					return false;
				}
				
			} 
		}return true;
	
}

void printGradeScores( short int grade[studentMax][assignMax])
{
	if(validate(grade))
	{
	
		int totalscore;
		float avgStudentScore;
		for (int x=0;x<studentMax;x++)
		{
			cout << "Student # " << x+1 << ": ";
			for(int y=0;y<assignMax;y++)
			{
			 	cout << setw(4) << grade[x][y]<< "  ";
			}cout << endl;
			
		} cout <<endl << endl<< endl;
	
	}
	else 
	{
		cout << "Invalid Grade(s). Grades should be between 0-100."<< endl<<endl;
	}	
}

void printGradeLetters(short int grade[studentMax][assignMax])
{
	if (validate(grade))
	{
	
		char letterGrade,avgStudentLetter;
		for (int x=0;x<studentMax;x++)
		{
		
			cout << "Student # " << x+1 << ": ";
			for(int y=0;y<assignMax;y++)
			{
			
				
					if (grade[x][y]>=90 && grade[x][y]<=100)
					{
						letterGrade='A';
					}
					else if(grade[x][y]>=80 && grade[x][y]<=89)
					{	
						letterGrade='B';
					}
					else if (grade[x][y]>=70 && grade[x][y]<=79)
					{
						letterGrade='C';
					}
					else if( grade[x][y]>=60 && grade[x][y]<=69)
					{
						letterGrade='D';
					}
					else  
					{
						letterGrade='F';
					}
			
					cout << setw(4) << letterGrade<< "  ";
				
				
			}
			cout << endl;
		
		}cout <<endl << endl<< endl;
	}
	else
	{
		cout << "Invalid Grade(s). Grades should be between 0-100."<< endl<< endl;
		
	}
}
int	getStudentTotalScores(short int grade[][assignMax],int row)// get the total score of each student
{
	if(validate(grade))
	{
	
		if(row>=0 && row<=studentMax)
		{
		
		
			int total=0;
			for( int y=0; y<assignMax;y++)
			{
					total+=grade[row-1][y];	
	
			}
			return total;
	
		}
		else 
		{
			return -1;
		}
	}
	else 
	{
		return -1;
	}
}

float getStudentAverageScore(short int grade[][assignMax], int row)// finding average score of each student
{
	if(validate(grade))
	{
	
		float avg;
		if(row>0 && row<=studentMax)

		{
			float total=0;
			for( int y=0; y<assignMax;y++)
			{
				total+=grade[row-1][y];
		
			}
			avg=total/assignMax;
			return avg;
		}
		else 
		{
			return -1;
		}
	}
	else 
	{
		return -1;
	}
}

char getStudentAverageLetter(short int grade[][assignMax], int row) //finding average letter grade of each student
{
	if(validate(grade))
	{
	
		char letterGrade;
		float avg;
		if(row>0 && row<=studentMax)
		{
	

			int total=0;
		
			for(int y=0;y<assignMax;y++)
			{
				
				if (validate(grade))
				{
					total+=grade[row-1][y];
				}
				else
				{
					return -1;
				}
		
			
			}
			avg=total/assignMax;
			if (avg>=90 && avg<=100)
			{
				letterGrade='A';
			}
			else if(avg>=80 && avg<=89)
			{	
				letterGrade='B';
			}
			else if (avg>=70 && avg<=79)
			{
				letterGrade='C';
			}
			else if( avg>=60 && avg<=69)
			{
				letterGrade='D';
			}
			else  
			{
				letterGrade='F';
			}
			return letterGrade;
		
		}
		else
		{
			return -1;
		}
	}
	else 
	{
		return -1;
	}
		
		
	
	
}

int getAssignmentTotalScores( short int grade[][assignMax], int col)// finding total score of each assignment
{
	if (validate(grade))
	{
	
		if(col>0 && col<=assignMax)
		{
			int total=0;
			for (int x=0;x<studentMax;x++)
			{
				total+=grade[x][col-1];
			
			}
			return total;
		}
		else 
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
float getAssignmentAverageScore(short int grade[][assignMax], int col)// finding average score of each assignment
{
	if(validate(grade))
	{
	
	float avg;
	if (col>0 && col<=assignMax )
	{
		float total=0;
		
		for (int x=0;x<studentMax;x++)
		{
			
			total+=grade[x][col-1];
			
			
		}
		avg= total/studentMax;
		return avg;
	}
	else 
	{
		return -1;
	}
}
else 
{
	return-1;
}
	
}

int getAssignmentMaxScore( short int grade[][assignMax], int col)// finding the max of each assignment
{
	if(validate(grade))
	{
	
	int max;
	
	if (col>0 && col<=assignMax)
	{
		max =grade[0][col-1];
		for (int x=1;x<studentMax;x++)
		{
			
			
			if (grade[x][col-1]> max)
				max=grade[x][col-1];
			
		}
		return max;
	}
	else 
	{
		return -1;
	}
}
else
{
	return -1;
}
}
int getAssignmentMinScore(short int grade[][assignMax], int col) //finding the min of each assignment
{
	if (validate(grade))
	{
	
	int min;

	
	
		
		if (col>0 && col<=assignMax)
		{
			min =grade[0][col-1];
			for (int x=1;x<studentMax;x++)
			{
				
				if (grade[x][col-1]< min)
					min=grade[x][col-1];
				
				
			}
			return min;
		}
		else 
		{
			return -1;
		}
		
	}
	else 
	{
		return -1;
	}
	
	
}








