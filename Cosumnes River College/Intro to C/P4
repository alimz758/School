#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

int main()

{
	ifstream inputFile;

	string  fileName;
		   
	float alphabetPriceFirst=0,
		  alphabetPriceLast=0,
		  mostExpensivePrice=0,
		  leastExpensivePrice=0,
		  tempPrice=0;
	
	cout << "Please enter the name of an input file: " ;
	cin >>fileName;
	inputFile.open(fileName.c_str());
	if (inputFile.fail())
	{
		cout << "The file does not exist!" << endl;
	}
	
	else 
	{
	
	string alphabetNameFirst="",
		   alphabetNameLast="",
		   mostExpensiveName,
		   leastExpensiveName,
		   tempName="";
		   
	float alphabetPriceFirst=0,
		  alphabetPriceLast=0,
		  mostExpensivePrice=0,
		  leastExpensivePrice=0,
		  tempPrice=0;
		  
		getline(inputFile, tempName);
				
				 
		inputFile>>tempPrice;
		inputFile.ignore();
		
		alphabetNameFirst=tempName;
		mostExpensiveName=tempName;
		alphabetPriceFirst=tempPrice;
		leastExpensivePrice=tempPrice;
		for(;inputFile;)
		{
			
				
				//cout << tempName<< endl;
				//cout << alphabetNameFirst<<endl;
				if (tempName<alphabetNameFirst) 
				{
					alphabetNameFirst=tempName;
					alphabetPriceFirst=tempPrice;
				
				}
			
				else if (tempName>alphabetNameLast)
				{
					alphabetNameLast=tempName;
					alphabetPriceLast=tempPrice;
				}
			
				if (tempPrice>mostExpensivePrice)
				{
					mostExpensivePrice=tempPrice;
					mostExpensiveName=tempName;
				}
				 if(tempPrice<leastExpensivePrice)
				{
					leastExpensivePrice=tempPrice;
					leastExpensiveName=tempName;
				}	
				getline(inputFile, tempName);
				
				 
				inputFile>>tempPrice;
				inputFile.ignore();
			
		}
	//	cout << alphabetNameFirst ;
		cout <<left << setw(25)<< "The Most Expenisve item: " << setw(21)  << mostExpensiveName << setw(5)<< "$"<<right<<setw(10)<< mostExpensivePrice  << endl;
		cout << left<< setw(25)<< "The Least Expensive item: "  <<setw(20)<< leastExpensiveName << setw(5)<< "$" <<right <<setw(10)<<leastExpensivePrice  << endl;
		cout << left << setw(25)<<"The alphabetically first: " << setw(20)<<alphabetNameFirst << setw(5)<<"$"<<right <<setw(10)<<alphabetPriceFirst << endl;
		cout << left<< setw(25)<<"The alphabetically Last: " <<setw(21)<< alphabetNameLast << setw(5)<< "$"<<right <<setw(10)<<alphabetPriceLast << endl;
	
	}

	

	return 0;
	
}
