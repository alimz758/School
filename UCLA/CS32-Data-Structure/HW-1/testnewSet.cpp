////
////  testnewSet.cpp
////  Homework#1
////
////  Created by ali mirabzadeh on 1/18/19.
////  Copyright © 2019 AMZ Development. All rights reserved.
////
//
#include "newSet.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Set ss;
    assert(ss.insert("roti"));
    assert(ss.insert("pita"));
    assert(ss.size() == 2);
    assert(ss.contains("pita"));
    ItemType x = "laobing";
    assert(ss.get(0, x)  &&  x == "pita");
    assert(ss.get(1, x)  &&  x == "roti");
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
