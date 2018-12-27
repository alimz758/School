#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	int choice;
	unsigned long rad, width, lenght, base, height;
	double area;
	const double  PI = 3.14159;
	
	cout <<"Geometry Calculator Menu" << endl;
	cout << "\t\t1. Calculate the Area of a circle" << endl;
	cout << "\t\t2. Calculate the Are of a Rectangle" << endl;
	cout << "\t\t3. Calculate the Area of a Triangle" << endl;
	cout << "\t\t4. Quit" << endl;
	cout << "Enter your choice:";
	cin >> choice;
	
	if ( choice==1)
	{
		cout << "What is the Radius: " ;
		cin >> rad;
		area = (PI)* (pow (rad,2));
		cout << "The are of the a Cirlce with radius " << rad << " ,is " << area << endl;
	}
	else if (choice==2)
	{
		cout << "What is the lenght:" ;
		cin >> lenght;
		cin.ignore();
		cout << "What is the width:" ;
		cin >> width;
		area= lenght * width;
		cout << "The are of a rectangle with lenght of " << lenght << " , and width of " << width ;
		cout << " , is " << area;
	}
	else if(choice==3)
	{
		cout <<"What is the base: ";
		cin >> base;
		cin.ignore();
		cout << "What is the height: " ;
		cin >> height;
		area = height* base * 0.5;
		cout << "The are of a triagle with base of " << base << " , and the height of " << height;
		cout << " ,is " << area;
	}
	else if (choice==4)
	{
		cout << "Program ending.\n";
	}
	else
	{
		cout << "The valid choices are 1 through 4. Run the";
		cout << "program again and select one of those." << endl;
	}
}
