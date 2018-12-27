#include <iostream>

using namespace std;

int main()
{
	char package;
	float dataUsed, Abill,Bbill,Cbill;
	cout << "Enter the package you have purchased: ";
	cin >> package;
	cout << "Please enter how many Gigabytes you have used: " << endl;
	cin >> dataUsed;
	
	if (dataUsed>4)
		Abill=39.99 + (dataUsed-4)*10;
	else
		Abill = 39.99;
	if (dataUsed>8)
		Bbill=59.99 +(dataUsed-8)*5;
	else
		Bbill=59.99;
		
	Cbill=69.99;
	switch (package)
	{
		case 'a':
		case 'A':
			cout <<"Your final payment for package A is: " << Abill << " $" << endl;
			if (Abill>Bbill)
			{
				cout << "The amount of money you would save with the " ;
				cout << "same amount of data with " << endl; 
				cout <<  "package B is " << Abill-Bbill << " $" <<endl;
			}
			if (Abill>Cbill)
			{
				cout << "The amount of money you would save";
				cout << "with thesame amount of data with" << endl;
				cout <<  "package C is " << Abill-Cbill << " $" <<endl;
			}
			break;
		case 'b':	
		case 'B':
			cout << "Your final payment for package B is " << Bbill << " $" << endl;
			if (Bbill>Cbill)
			{
				cout << "The amount of money you would save with the " ;
				cout << "same amount of data with " << endl;
			 	cout <<  "package C is " << Bbill-Cbill << " $" <<endl;
				 
			}
			break;
		case 'c':
		case 'C':
		cout <<	"Your final payment for package C is " << Cbill << " $" << endl;
		break;
		
		default : cout << "Invalid input. You should choose between A, B, OR C.";
	}
}
	
