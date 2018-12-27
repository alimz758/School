 #include <iostream>
 #include <fstream>
#include <string>
using namespace std;

int main()
{
	ofstream outputFile;
	int size, choice,number;
	string fileName,name;
	
	cout << "Please choose a number between 2-80 for the size of the right triangle: ";
	cin >> size;
	
	
	if ( size >=2 && size <= 80)
	{
		cout << "Please enter number of triangles between 1-10: " ; 	
		cin >> number;
		if (number>=1 && number <=10)
		{
		
			cout << "How do you want to place '+' in the triangle?" <<endl;
			cout << "Choose a number from the menu." << endl;
			cout << "1. Upper Left."<< endl;
			cout << "2. Upper Right." << endl;
			cout << "3. Lower Left." << endl;
			cout << "4. Lower Right." << endl;
			cin >> choice;
			cout << "Please enter a file name: " << endl;// doesn't work!
			cin >>fileName; //getline does not work
			cin.ignore(255,'\n');
			outputFile.open(fileName.c_str());
			
			switch (choice) 
			{
				
				
				case 1:
				
				if (outputFile)
				{
				//	cout << "Enter the describtion of this triangle: ";
				//	getline (cin, name);
					
					outputFile << "Size " << size <<", Upper Left, Number "<< number << ":"<< endl;
					for (int count=1 ; count<=number;count++)
					{
				
						for (int row=size;row>0;row--)
						{
					
							for ( int star=1; star<=row; star++)
							{
								outputFile << '+';
							}
							outputFile << endl;							}
						}
				}
				else 
				{
					cout << "The file with the given name already exist!"<< endl;
				}
	
				break;
				
					case 2:
					if (outputFile)
					{
					
						outputFile << "Size " << size <<", Upper Right, Number "<< number << ":"<< endl;
						for (int count=1 ; count<=number;count++)
						{
							for (int row=size; row>0; row--) // it doesn't show correctly for star>40
												// is it because they don't fit inside the window?
							{
								for (int space=0; space< size -row; space++)
								{
									outputFile<< "  ";
								}
								for (int star=1; star<=row; star++)
								{
									outputFile << '+' << " ";
								}
								outputFile << endl;
							}
						}
					outputFile.close();
					}
					else 
					{
						cout << "The file with the given name already exist!"<< endl;
					}
					
					break;
				
					case 3:
					if (outputFile)
					{
						outputFile << "Size " << size <<", Lower Left, Number "<< number << ":"<< endl;
						for (int count=1 ; count<=number;count++)
						{
					
							for (int row=1 ;row<=size;row++)
							{
								for (int star=1; star<=row; star++)
								{
									outputFile <<'+';
								}
								outputFile << endl;
							}
						}
						outputFile.close();
					}
					else 
					{
						cout << "The file with the given name already exist!"<< endl;
					}
					break;
			
					case 4:
					if (outputFile)
					{
						outputFile << "Size " << size <<", Lower Right, Number "<< number << ":"<< endl;
						for (int count=1 ; count<=number;count++)
						{
				
							for ( int row=1; row<=size; row++)
							{	
								for (int space= size -row;space>0;space--)
								{
									outputFile << "  ";
								}
								for (int star=1;star<=row;star++)
								{
									outputFile << '+' <<" ";
								}
								outputFile << endl;
							}
						}
					outputFile.close();
				}
				else 
				{
					cout << "The file with the given name already exist!"<< endl;
				}
				
					break;
					
					default: cout << "You should choose between 1-4 from the menu. Thanks!";
			
			}
			
		}
		else
			cout << "Please enter between 1-10 for the number of triangles." << endl;
	
	}
	
	else 
	{
		cout << "Enter a number between 2 - 80. Thanks!" << endl;
	}
	
}
