#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

void setRandomValues(int[], int[], int length);
void bubbleSort(int[], int length);
void selectionSort(int[], int length);
void compareBubbleAndSelectionSort(int length);
void swap(int &, int &);



int main()
{
	int size= 25000;
	for( int count=1 ; count<4 ; count++)
	{
		compareBubbleAndSelectionSort(size);
		size=size *2;
	}
	return 0;		
}


void setRandomValues(int array1[], int array2[], int length)
{
	double random;
	for(int count=0; count< length ; count++)
	{
		unsigned srand(time(0));
		random= (rand() % length)+1;
		array1[count]=random;
		array2[count]=random;
	}
}

void bubbleSort(int array1[], int length)
{
	bool swap;
	int temp;
	do
	{
		swap=false;
		for(int count=0 ; count <(length-1); count ++)
		{
			if ( array1[count]> array1[count+1])
			{
				temp=array1[count];
				array1[count]=array1[count+1];
				array1[count+1]=temp;
				swap=true;
			}
		}
		
	}while(swap);
	
	
}

void selectionSort(int array2[], int length)
{
	int  minIndex, minValue;
	for( int count=0; count< (length-1); count++)
	{
		minIndex=count;
		minValue=array2[count];
		for( int index= count+1; index<length; index++)
		{
			if (array2[index] < minValue)
			{
				minValue= array2[index];
				minIndex=index;
			}
		}
		swap(array2[minIndex],array2[count]);
	}
}

void swap (int &a, int &b)
{
	int temp=a;
	a=b;
	b=temp;
}

void compareBubbleAndSelectionSort(int length)
{
	int time1,time2,change_time_bubble,	change_time_selection,array1[length], array2[length];;

	cout << "Comparing bubble and selection for a list of size " << length << endl;
	setRandomValues(array1, array2, length);
	
	time1=time(0);
	bubbleSort(array1, length);
	time2=time(0);
	change_time_bubble=time2-time1;
	cout << "Bubble sort took " << change_time_bubble << " seconds(s)." << endl;
	
	time1=time(0);
	selectionSort(array2, length);
	time2=time(0);
	change_time_selection=time2-time1;
	
	cout << "Selection sort took " << change_time_selection <<" second(s)." << endl<< endl;	
}

