#include <iostream>
#include <fstream>

using namespace std;

int count();
int *scoreFunction(int);
void sortFunction(int *, int) ;
void averageFunction(int*, int);
void medianFunction( int *, int);

int main()
{
	int nonOutlier_count;
	int *scorePtr= NULL;
	
	nonOutlier_count=count(); //get the number of non-outliers
	scorePtr=scoreFunction(nonOutlier_count); // get the score array
	sortFunction(scorePtr,nonOutlier_count); // sort the array
	
	delete [] scorePtr;
	scorePtr=NULL;
	
	return 0;
}

int count()
{
	int number, count=0, count2=0;
	ifstream inputFile;
	inputFile.open("scores.txt");
			
	while(inputFile>>number)
	{
		if(number>=0)
		{
			count++;
		}
		else
			count2++;
	}
	
	if(count2==1) //if there is one outlier, print 1.
	{
		cout << "There is 1 outlier value." << endl;
	}
	else
		cout << "There are " << count2 << " outliers values." << endl;
		
	inputFile.close();
	return count;
	
}

int *scoreFunction(int size)
{
	int number;
	int * score= NULL;
	score= new int [size];
	ifstream inputFile;
	inputFile.open("scores.txt");
	int count=0;

	while(inputFile>>number && count<size)
	{
		if(number>=0)
		{
			*(score+count)=number;
			count++;
		}	
	}
		
	inputFile.close();
	return score;
}

void sortFunction( int *sort, int count)
{
	int temp;
	cout << "Sorted scores: ";
	for(int sub1=0; sub1<count;sub1++)
	{
		for(int sub2=sub1+1; sub2<count;sub2++)
		{
			if(*(sort+sub1)< *(sort+sub2))
			{
				temp=*(sort+sub1);
				*(sort+sub1)= *(sort+sub2);
				*(sort+sub2)=temp;
			}
		}
		
		if (sub1==count-1)// just for makes it nicer for the lst element print with "."
		{
			cout << *(sort+sub1) << ".";
		}
		else
			cout << *(sort+sub1) << ", ";
	}
	
	cout << endl;
	averageFunction(sort,count); //get the average
	medianFunction(sort,count); //get the median
}

void averageFunction (int *scores, int count)
{
	double average, total=0.0;
	for (int index=0; index<count; index++)
	{
		total+=*(scores+index);
	}
	average=total/count;
	cout << "Average Score: " << average << endl;
}

void medianFunction(int *score, int count)
{
	int first,
		second,
		middle;
	double median;	
	
	if (count%2==0) //if there are even elements
	{
		first=count/2; // to find the firt half
		second=first+1; // to get the second half
		first=first-1;// minus one because want to use in pointr
		second=second-1;
		median= (static_cast<double>(*(score+first))+ *(score+second))/2; //used static_cast to have float output
		
		cout << "Median Score: " << median << endl;
	}	
	else // for the odd number of elements
	{
		middle=count/2;
		median=*(score+middle);
		cout << "Median Score: " << median << endl;
		
	}
}
