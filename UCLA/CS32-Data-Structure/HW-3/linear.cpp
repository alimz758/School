//
//  main.cpp
//  HW3.2
//
//  Created by ali mirabzadeh on 2/5/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

//For this part of the homework, you will turn in one file named linear.cpp that contains the five functions and nothing more: no #include directives, no using namespace std;, no implementation of somePredicate and no main routine. (Our test framework will precede the functions with appropriate #include directives, using statement, and our own implementation of a function named somePredicate that takes a double and returns a bool.)
bool anyTrue(const double a[], int n);
int countTrue(const double a[], int n);
int firstTrue(const double a[], int n);
int positionOfMin(const double a[], int n);
bool includes(const double a1[], int n1, const double a2[], int n2);
// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
    //if n is smaller than zero, which is invalid, return false
    if (n<=0)
        return false;
    //if n==0 ; then call somePredicate by passing the first element and check what does it return
    else if(n==1)
        return somePredicate(a[0]);
    //check the current element array an recursively and see if there is at least one in somePredicate that returns true
    else
        return (somePredicate(*a) || anyTrue(a+1, n-1));
}
// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
    //if n is smaller/equal to zero, which is invalid, return zero as the result
    if (n<=0)
        return 0;
    //if the fist element passed into somePredicate is true; increament, do a recursive call
    if (somePredicate(a[0]))
        return 1+countTrue(a+1,n-1);
    //recursively call countTrue till get to the break point and add one
    return countTrue(a+1, n-1);
}
// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    //if n is smaller/equal to zero, which is invalid, return -1 as the result
    if (n<=0)
        return -1;
    //if the fist element passed into somePredicate is true; return zero as the subscript
    if (somePredicate(a[0]))
        return 0;
    else
    {
        //if by calling recursively, no true got returned, then return -1
        if (firstTrue(a+1, n-1)==-1)
            return -1;
        //otherwise, recursively go through the function and increament
        return 1 + firstTrue(a+1, n-1);
    }
}
// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfMin(const double a[], int n)
{
    //define integer type variable
    int subScript;
    //if no element, return -1
    if (n<=0)
        return -1;
    //recursively call positionOfMin to, assign the subScript
    subScript = 1 + positionOfMin(a+1, n-1);
    if(a[0] <= a[subScript])
        return 0;
    else
        return subScript;
}
// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    //if the both array got ended without finding the match, return false
    if(n1==0 && n2==0)
        return false;
    //if n2 is bigger than n1, then stop the recursive call, and return false
    if (n1==0 && n2!=0)
        return false;
    //if the first element of each array is the same, go deeper in a1 and keep comparing
    if(a1[0]==a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);
    //otherwise keep going into both array
    else
        return includes(a1+1, n1-1 , a2, n2);
}
