#include <iostream>

using namespace std;

int main()

{
	string name, city, college, profession, animal, petname;
	int age;
	cout << "The name is:";
	cin >> name ;
	cin.ignore();
	cout << "The city is: ";
	cin >> city;
	cin.ignore();
	cout << "The college is: ";
	cin >> college;
	cin.ignore();
	cout << "The profession is: ";
	cin >> profession;
	cin.ignore();
	cout << "The animal is: ";
	cin >> animal;
	cin.ignore();
	cout << "The petname is: ";
	cin >> petname;
	cin.ignore();
	cout << "The age is: ";
	cin >> age;
	
	cout << "There once was a person named " << name << " who lived in " << city ;
	cout << ".At the age of " << age << ", " << name << " went to college at ";
	cout << college << ". " << name << " graduated and went to work as a " << profession;
	cout << ". Then, " << name << " adopted a(n) " << animal << " named " << petname;
	cout << ". They both lived happily ever after!\n";
	return 0;
}
