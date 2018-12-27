#include <iostream>

using namespace std;


int main()
{
	int secs, secs1,mins,hours, days;
	cout << "Please enter a number of seconds: ";
	cin  >> secs;
	
	if (secs>=86400)
	{
		days=secs/86400;
		cout << "Number of days are: "	<< days << "." ;
		
	}
	else if (secs>=3600)
	{
		hours=secs/3600;
		
		cout << "It passed " << hours << " hours" ;
		
    }
	else if(secs>=60)
	{
		mins= secs/60;
		cout << "It passed  " << mins << " minutes." ;
		
    
}
	
	else
		cout << "Number of seconds is invalid. It should be greater than 60." << endl;
		
	return 0;	
		
		

}
