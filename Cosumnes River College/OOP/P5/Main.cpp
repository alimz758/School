// V10A5.cpp by Ali Mirabzadeh
// Complex class test program.
#include <iostream>
#include "Complex.h"
using namespace std;

int main()
{
	Complex x, y(4.3, 8.2), z(3.3, 1.1), k,l, m(0, 0.1);

	cout << "Enter a complex number in the form: <a, b> for Complex object k.\n";
	cin >> k;
	cout << "\nThe existing Complex objects are:\n";
	cout << "(Performing << operator overloading.)";
	cout << "\nx: " << x << "\ny: " << y << "\nz: " << z << "\nk: " << k << "\nl: " << l << "\nm: " << m << '\n';

	// demonstrating overloaded + and =
	cout << "\nPerforming +, = and << operators overloading.";
	x = y + z;
	cout << "\nx = y + z:\n" << x << " = " << y << " + " << z << '\n';

	// demonstrating overloaded - and =
	cout << "\nPerforming -, = and << operators overloading.";
	x = y - z;
	cout << "\nx = y - z:\n" << x << " = " << y << " - " << z << '\n';

	// demonstrating overloaded * and =
	cout << "\nPerforming *, = and << operators overloading.";
	x = y * z;
	cout << "\nx = y * z:\n" << x << " = " << y << " * " << z << "\n\n";

	cout << "Performing /, = and << operators overloading.";
	x = y / z;
	cout << "\nx = y / z:\n" << x << " = " << y << " / " << z << '\n';

	//This is a divide by zero situation
	cout << "\nPerforming /, = and << operators overloading with divisor is (0, 0).";
	x = y / l;
	cout << "\nx = y / l:\n" << x << " = " << y << " / " << l << '\n';

	//This is a divide by a close to zero situation
	cout << "\nPerforming /, = and << operators overloading with divisor is (0, 0.1).";
	x = y / m;
	cout << "\nx = y / m:\n" << x << " = " << y << " / " << m << '\n';

	// demonstrating overloaded !=
	cout << "\nPerforming != and << operators overloading.\n" << "check x != k\n";
	if (x != k) 
		cout << x << " != " << k << '\n';

	// demonstrating overloaded ==
	cout << "\n\nPerforming =, == and << operators overloading." << endl
		<< "assign k to x by using x = k statement." << endl
		<< "check x == k" << endl;
	x = k;
	if (x == k) 
		cout << x << " == " << k << '\n';

	system("PAUSE");
	return 0;
} // end main
