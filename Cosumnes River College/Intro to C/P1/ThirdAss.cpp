#include <iostream>

using namespace std;

int main()
{
	const float mealCharge= 88.67;
	float tax;
	float tip;
	float totalAmount;
	
	// Tax is 7.75 percent of the meal charge.
	tax= mealCharge * 0.0775;
	
	// Tip is 20 percent of the meal charge.
	tip= (mealCharge +tax) * 0.2;
	
	totalAmount= mealCharge+ tax+ tip;
	
	cout << "The meal cost is: " << mealCharge << " dollars.\n";
	cout << "The tax should be: " << tax << " dollars.\n";
	cout << "The tip shuld be: "  << tip << " dollars.\n";
	cout << "The total bill is "  << totalAmount << " dollars.\n";
	return 0;
		
}
