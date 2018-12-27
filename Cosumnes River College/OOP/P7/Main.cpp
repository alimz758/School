// Created by Ali Mirabzadeh

#include <iostream> // allows program to output data to the screen
#include <iomanip> // parameterized stream manipulators
#include <cstdlib> // uses srand and rand
#include <ctime> // used for the time(0) seed
#include <vector> // used array/vector

using namespace std;

void quickSortHelper(vector <int> &, int, int); // function to sort numbers from smallest to largest
int partition(vector <int> &, int, int); // function to  find the first value of the sorted vector
void swap(int *const, int *const);

int main()
{
	const int MAX_NUMBER = 100;

	const int MAX_SIZE = 20;

	vector <int> vectorToBeSorted(MAX_SIZE);

	int loop = 0;

	int newSize = 0; // new Size needed

	int numberOfSet = 0; // need to display 10 sets

	srand(time(0)); // seeding random numb generator with current time

	newSize = rand() % MAX_SIZE;

	for (numberOfSet = 1; numberOfSet < 11; ++numberOfSet)
	{
		if (numberOfSet == 1)
		{
			cout << "The " << numberOfSet << "st set of data(" << newSize << "):" << endl;
		}

		else if (numberOfSet == 2)
		{
			cout << "The " << numberOfSet << "nd set of data(" << newSize << "):" << endl;
		}
		else if (numberOfSet == 3)
		{
			cout << "The " << numberOfSet << "rd set of data(" << newSize << "):" << endl;
		}
		else
		{
			cout << "The " << numberOfSet << "th set of data(" << newSize << "):" << endl;
		} // end if..else


		for (loop = 0; loop < newSize; loop++)
		{
			vectorToBeSorted[loop] = rand() % MAX_NUMBER;
		}

		if (newSize == 0)
		{
			cout << fixed << right << setw(4) << "The vector is empty." << endl;
		}
		else if (newSize == 1)
		{
			cout << fixed << right << setw(4) << "Initial vector value is:" << endl;

		}
		else
		{
			cout << fixed << right << setw(4) << "Initial vector values are:" << endl;
		}

		for (loop = 0; loop < newSize; loop++)
		{
			cout << fixed << right << setw(4) << vectorToBeSorted[loop];
		}


		cout << endl;



		if (newSize == 0)
		{
			cout << setw(4) << "The sorted vector is empty." << endl;

			cout << endl << endl;

		}
		else if (newSize == 1)
		{
			cout << setw(4) << "Vector is sorted: " << endl;

			cout << setw(4) << vectorToBeSorted[0] << endl;

			cout << endl << endl;
		}
		else
		{
			quickSortHelper(vectorToBeSorted, 0, newSize - 1);
			cout << setw(4) << "The sorted vector values are:" << endl;

			for (loop = 0; loop < newSize; loop++)
			{
				cout << setw(4) << vectorToBeSorted[loop];
			}

			cout << endl << endl;
		} // end if..else

		newSize = rand() % MAX_SIZE; // set new size
	}

	system("pause");
	return 0;
} // end main

  // function to sort numbers from smallest to largest
void quickSortHelper(vector<int> &data, int first, int last)
{
	int currentLocation;

	if (first >= last)
	{
		return;
	}

	currentLocation = partition(data, first, last);
	quickSortHelper(data, first, currentLocation - 1);
	quickSortHelper(data, currentLocation + 1, last);

} // end quickSortHelper function

  // Partition: Take the first element of the unsorted vector and 
  // 			  determine its final location in the sorted vector
int partition(vector <int> &data, int left, int right)
{
	int position = left;

	while (true)
	{
		while (data[position] <= data[right] && position != right)
		{
			right--;
		}

		if (position == right)
		{
			return position;
		}

		if (data[position] > data[right])
		{
			swap(&data[position], &data[right]);
			position = right;
		}

		while (data[left] <= data[position] && left != position)
		{
			left++;
		}

		if (position == left)
		{
			return position;
		}

		if (data[left] > data[position])
		{
			swap(&data[position], &data[left]);
			position = left;
		}
	}
} // end partition function

void swap(int * const ptr1, int *const ptr2)
{
	int temp;

	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
} // end swap function
