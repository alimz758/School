#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
	int num1,num2, total;
	unsigned seed = time(0);
	srand(seed);
	int max=999, min =100;
	num1=(rand()% (max-min+1))+min;
	num2=(rand()% (max-min+1))+min;
	cout << "The two numbers are : " << num1 << " and " << num2;
	cin.get();
	total= num1 +num2;
	cout <<"The total is " << total;
	return 0;
	
}
