#include <iostream>
#include <iomanip>
using namespace std;

struct info //global structure called info
{
	string name;
	int length;
	string genre;
	double price;
}movie[10];

void menu1(struct info [], int &);
void menu2(struct info [], int);
void menu3(struct info []);
void menu4(struct info [] , int);
void menu5(struct info [], int);

int main()
{
	int choice, num_movie=0;
	cout << "Welcome to the movies!" << endl;
	
	do
	{
		cout << "Please selecet one from the menu:"<< endl;
		cout << "1) Enter all movie data into the array."<< endl;
		cout << "2) Display all movie data currently stored in the array."<< endl;
		cout << "3) Change the contents of any one array element."<< endl;
		cout << "4) Sort the array by movie name in ascending order."<<endl;
		cout << "5) Sort the array by movie name in descending order."<< endl;
		cout << "6) Quit the program."<< endl;
		
		cin>> choice;
				
		switch(choice)
		{
			
			case 1:
				cout << endl<< endl; // to have space for the next menu
				cout << "Selection 1:" << endl;
				menu1(movie, num_movie);  // fisrt choice of the menu
				break;
			case 2:
				cout << endl<< endl;
				cout << "Selection 2:"<< endl; 
				menu2(movie, num_movie); //second choice of the menu
				break;
			case 3:
				cout << endl<< endl;
				cout << "Selection 3:"<< endl;
				menu3(movie); // third choice of the menu
				break;
			case 4:
				cout << endl<< endl;
				cout << "Selection 4:"<< endl;
				menu4(movie, num_movie); // fourth choice of the menu
				break;
			case 5:
				cout << endl<< endl;
				cout << "Selection 5:" << endl;
				menu5(movie, num_movie); // fifth choice of the menu
				break;
			case 6:
				cout << "Exiting..." ; // exit for the sixth choice
				exit(0);
			
			default	: // if any other numbers other than 1-6 
				cout << "You have entered a wrong number for the menu. ";
				cout << "Please selcet a number between 1-6." << endl;
				break;
		}
		
	
	}while(choice!=6); // repeatedly ask the user the mneu option until get 6
	
	return 0;
} 

void menu1(struct info movie[], int &num_movie) //ask the user the data
{
		cout << "How many movies do you want to enter(up to 10): " << endl;
		cin >> num_movie;
		cin.ignore();
		
		for (int count=0; count < num_movie; count++)
		{
			cout << "Please enter data for movie #" << count+1 << endl;
		
			cout << "Name: " ;
			getline(cin, movie[count].name);
			
			cout <<"Length (in minutes): " ;
			cin >>movie[count].length;
			cin.ignore();
			
			cout<< "Genre: " ;
			getline(cin,movie[count].genre);
		
			cout << "Price (in dollars and cents): " ;
			cin >> movie[count].price ;
			cin.ignore();
		}
}

void menu2(struct info display_movie[], int numMovie) // diplay the data have inputed in menu1
{
	cout << fixed << setprecision(2); 
	for (int count=0;count<numMovie; count++)
	{
		cout << "Movie #" << count+1<< endl;
		cout << "Name: " << display_movie[count].name << endl;
		cout << "Length: " << display_movie[count].length << " minutes." <<endl;
		cout << "Genre: " << display_movie[count].genre << endl;
		cout << "Price: " << display_movie[count].price<< " $."<<endl<< endl;
	}
	
}

void menu3(struct info movie[]) 
{
	int num;
	cout << "Enter an element number to replace." << endl;
	cin.ignore();
	cin >> num;
	cout << "PLease enter data for movie #" << num<< endl;
	cin.ignore();
	
	cout << "Name: " ;
	getline(cin, movie[num-1].name );
	
	cout << "Length (in minutes) ";
	cin>> movie[num-1].length;
	cin.ignore();
	
	cout << "Genre: ";
	getline(cin, movie[num-1].genre);

	cout << "Price (in dollars and cents format): " ;
	cin >> movie[num-1].price ;
	cin.ignore();
}

void menu4(struct info ascending_sort[], int count)
{
	info temp;
	for(int sub1=0; sub1<count-1;sub1++)
	{
		for(int sub2=sub1+1; sub2<count ;sub2++)
		{
			if( ascending_sort[sub1].name > ascending_sort[sub2].name)
			{
				temp=ascending_sort[sub1];
				ascending_sort[sub1]=ascending_sort[sub2];
				ascending_sort[sub2]=temp;
			}
		}
	}
	cout <<"Ascending sort completed." << endl;
}

void menu5 (struct info descending_sort[], int size) 
{
	info temp;
	for(int sub1=0; sub1<size-1;sub1++)
	{
		for(int sub2=sub1+1; sub2<size ;sub2++)
		{
			if( descending_sort[sub1].name <descending_sort[sub2].name)
			{
				temp=descending_sort[sub1];
				descending_sort[sub1]=descending_sort[sub2];
				descending_sort[sub2]=temp;
			}
		}
	}
	cout <<"descending sort completed." << endl;
}
