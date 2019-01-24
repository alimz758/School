////
////  SSNSet.cpp
////  Homework#1
////
////  Created by ali mirabzadeh on 1/17/19.
////  Copyright © 2019 AMZ Development. All rights reserved.
////
//#include "SSNSet.h"
//#include "Set.h"
//
//
//   // Create an empty SSN set.
//SSNSet :: SSNSet():m_SSNSet()
//{
//}
//// Add an SSN to the SSNSet.  Return true if and only if the SSN
//bool SSNSet :: add(unsigned long ssn)
//{
//    //call insert member function of class Set
//    //that function will return true if ssn gets added
//    if(m_SSNSet.insert(ssn))
//        return true;
//    //else return false
//    else
//        return false;
//}
//// Return the number of SSNs in the SSNSet.
//int SSNSet :: size() const
//{
//    //calling size function in Set class and return the result
//    return m_SSNSet.size();
//}
//// Write every SSN in the SSNSet to cout exactly once, one per
//// line.  Write no other text.
//void SSNSet :: print() const
//{
//    ItemType x;
//    //Loop through SSNSet
//    for (int count=0; count< size(); count++)
//    {
//        m_SSNSet.get(count,x);
//        //printing each SSN in SSNSet; one SSN per line
//        cout << "SSN #" << count +1 << ": " << x << endl;
//    }
//}
//
//
//
