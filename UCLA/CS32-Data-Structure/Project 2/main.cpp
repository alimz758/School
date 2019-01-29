//
//  main.cpp
//  Project 2
//
//  Created by ali mirabzadeh on 1/23/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
#include "Set.h"
#include <iostream>
#include <cassert>
#include <type_traits>

#define CHECKTYPE(f, t) { auto p = static_cast<t>(f); (void)p; }

static_assert(std::is_default_constructible<Set>::value,
              "Set must be default-constructible.");
static_assert(std::is_copy_constructible<Set>::value,
              "Set must be copy-constructible.");

void thisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Set::operator=, Set& (Set::*)(const Set&));
    CHECKTYPE(&Set::empty,     bool (Set::*)() const);
    CHECKTYPE(&Set::size,      int  (Set::*)() const);
    CHECKTYPE(&Set::insert,    bool (Set::*)(const ItemType&));
    CHECKTYPE(&Set::erase,     bool (Set::*)(const ItemType&));
    CHECKTYPE(&Set::contains,  bool (Set::*)(const ItemType&) const);
    CHECKTYPE(&Set::get,       bool (Set::*)(int, ItemType&) const);
    CHECKTYPE(&Set::swap,      void (Set::*)(Set&));
    CHECKTYPE(unite,    void (*)(const Set&, const Set&, Set&));
    CHECKTYPE(subtract, void (*)(const Set&, const Set&, Set&));
}

int main()
{}


//int main()
//{
//    //for this testcases; ItemType in Set.h should be changed to unsigned long
//    Set ss, pp;
//    ItemType x;
//    assert(!pp.get(0,x));//there is nothing in pp to get
//    assert(ss.size()==0); //check size function works
//    assert(ss.empty()); // check to see if s set is empty
//    assert(pp.empty()); // check to see if a set is empty
//
//    for (int i = 0; i < 50; i++)
//    {
//        ss.insert(i);        // Insert 50 items in s
//    }
//
//    assert(ss.size() == 50);    // Check to see if s contains 50
//    ss.swap(pp);  // Swap the random values from a for numbers 0-50 into a
//    assert(pp.size() == 50);    // Check to see if the size is 50
//
//    for (int k = 0; k < 20; k++)
//    {
//        pp.erase(k);    //remove the first 20 numbers
//    }
//    assert(pp.size() == 30);    // Checks if size is now 30
//    Set set1;
//    set1.insert(30);  // The following inserts test if the data is correctly inserted in the sequence
//    set1.insert(21);
//    set1.insert(63);
//    set1.insert(42);
//    set1.insert(17);
//    set1.insert(10);
//    cout <<"set1:"<<endl;
//    set1.dump();//check whether insert works correctly and they have been added in order
//    assert(!set1.erase(654));//nothing to erase
//    Set set2;
//    set2.insert(17);
//    set2.insert(63);
//    set2.insert(29);
//    cout<< "set2 before the assignment operator:"<<endl;
//    set2.dump();
//    ItemType data;
//    assert(set1.get(1,data) && data==17);//check get works and gets the correct data
//    Set result;
//    cout << "Result in Unite:" << endl;
//    unite(set1, set2, result);//check the unite function whether gives out 10, 17, 21, 29, 30, 42, 63
//    result.dump();
//    cout<<endl<< endl;
//
//    cout << "Result in Subtract:" << endl;
//    subtract(set1, set2, result);//check the unite function whether gives out 10, 21, 30 , 42
//    result.dump();
//
//    set2=set1; //check assignment operator works
//    assert(set1.size()==set2.size());//if the assignment operator worked correctly they should both have the same size
//    assert(set1.contains(21) && set2.contains(21));// they should both contain 21
//    cout<<"set2 after the assignment:"<<endl;
//    set2.dump();
//    Set setNew(pp);
//    assert(setNew.size()==30);//Check that copy constructor works by checking whether newSet has the same size as pp =30
//}
