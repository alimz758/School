//
//  tree.cpp
//  HW3.4
//
//  Created by ali mirabzadeh on 2/6/19.
//  Copyright © 2019 AMZ Development. All rights reserved.

//                                                  ----------------------start------------------------------
// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then for this value of a2     the function must return
//    10 20 40            1
//    10 40 30            2
//    20 10 40            0
//    50 40 30            3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    //if nothing in n2, aka empty sequence, return 1 as the instruction above says
    if (n2<=0)
        return 1;
    //if checking the last element of a2
    if (n1<=1)
    {
        //if found pattern return 1
        if (a1[0]==a2[0])
            return 1;
        //otherwise, zero as no pattern got found
        return 0;
    }
    //if the the current fisrt element of both a1 and a2 are the same
    if (a1[0]==a2[0])
        //do two recursive fuction calls
        //one move forward for both arrays and one only for a1 to find all the possible matches; sum the result for both
        return countIncludes(a1+1, n1-1, a2+1, n2-1) + countIncludes(a1+1, n1-1, a2, n2);
    //otherwise, move in forward in a1
    else
        return countIncludes(a1+1, n1-1, a2, n2);
}
// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}
// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements. Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider,
            int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < divider)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > divider)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}
// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    // If n <= 1, do nothing.
    if(n<=1)
        return;
    //pivot point is the middle of array--> a[middle] == devider
    int middle= n/2;
    int firstNotG,firstL;
    divide(a, n, a[middle], firstNotG, firstL);
    //sort the first half
    order(a,firstNotG);
    //sort the second half
    order(a+ firstL, n-firstL);
}
