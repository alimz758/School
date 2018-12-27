#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>


using namespace std;
string descrip(int , int );
int sum(int &,int &, string &);
bool game(ofstream &);




int main()
{
	int point;
	float wintime;
	bool win;
	
	float percentage;
	
	ofstream output;
	output.open("craps.out.txt");
	
	srand(time(0));
	for (int count=1;count<=5000;count++) //Number of games
	{
		
		win=game(output);
		if(win)
		{
			wintime++;
		}
			
	}
	
	percentage= (wintime/5000)*100;
	cout << "The winning percentage is: " << percentage << " %"<<endl;
	output.close();
}
bool game(ofstream &output)//play the game
{	
	int reftotal,dice1,dice2,point, reftotal1,dice1_1,dice2_1;
	string result,description,description_1;
	reftotal=sum(dice1,dice2,description);
	if (reftotal==7 || reftotal==11)
	{
		result="Player Win";
		output << "Roll # 1"  << endl;
		output << dice1 << "-" << dice2<< endl;
		output << description<< endl;
		output << result<< endl<< endl;
		return true;
	
	}
	 else if (reftotal==2 ||reftotal==3 || reftotal==12)
	{
		result="Player Lose";
		output << "Roll # 1" <<endl;
		output << dice1 << "-" << dice2<< endl;
		output << description<< endl;
		output<< result << endl<< endl;
		return false;
	}
	else
	{
		
		output << "Roll # 1" << endl;
		output<< dice1 << "-" << dice2<< endl;
		output << description<< endl;
		point=reftotal;
		do
		{
			for(int roll=2;;roll++)
			{
				reftotal1=sum(dice1_1,dice2_1,description_1);
				if (reftotal1==point)
				{
					result="Player Win";
					output << "Roll # " << roll << endl;
					output<< dice1_1 << "-" << dice2_1<< endl;
					output << description_1<< endl;
					output << result<< endl<< endl;
					return true;
				}
				else if (reftotal1==7)
				{
					result="Player Lose";
					output << "Roll # " << roll << endl;
					output << dice1_1 << "-" << dice2_1<< endl;
					output << description_1<< endl;
					output << result<< endl<< endl;
					return false;
					

				}
				else 
				{
					output<< "Roll # " << roll << endl;
					output<< dice1_1 << "-" << dice2_1<< endl;
					output << description_1<< endl;
				}
			}
	 	} while(reftotal1!=point ||reftotal1!=7);
	 	
	 }
	 

	
}
int sum(int &refdice1,int &refdice2, string &refdescription) // generate dice and find the sum
{
		
	refdice1=(rand()%6) +1; 
	refdice2=(rand()%6)+1;
	refdescription=descrip(refdice1,refdice2);
	return (refdice1+refdice2); 	
	
}


string descrip(int first, int sec) //finding the slang description
{
	string description;
	if (first==1 && sec==1)
	{
		description="Snake Eyes";
	}
	else if ((first==1 && sec==2) || (first==2 && sec==1))
	{
		description="Ace Deuce";
	}
	else if ((first==1 && sec==3) || (first==3 && sec==1))
	{
		description="Easy For";
	}
	else if (first==2 && sec==2)
	{
		description="Hard Four";
	}
	else if((first==1&& sec==5) || (first==2 && sec==4) || (first==4 && sec==2) || (first==5 && sec==1))
	{
		description="Easy Six";
	}
	else if (first==3 && sec==3)
	{
		description="Hard Six";
	}
	else if ((first==2 && sec==6) || (first==3 && sec==5) || (first==5 && sec==3) || (first==6 && sec==2))
	{
		description="Easy Eight";
	}
	else if (first==4 && sec==4)
	{
		description="Hard Eight";
	}
	else if ((first==4 && sec==6) || (first==6 && sec==4))
	{
		description="Easy Ten";
	}
	else if (first==5 && sec==5)
	{
		description="Hard Ten";
	}
	else if ((first==5 && sec==6) || (first==6 && sec==5))
	{
		description="Yo";
	}
	else if (first==6 && sec==6)
	{
		description="Boxcars";
	}
	else
		description="";
		
	return description;
}
