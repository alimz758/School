////
////  set.hpp
////  Homework#1
////
////  Created by ali mirabzadeh on 1/16/19.
////  Copyright © 2019 AMZ Development. All rights reserved.
////
//
#ifndef set_hpp
#define set_hpp
#include <string>
using namespace std;
const int DEFAULT_MAX_ITEMS = 250;
//Allies
using ItemType = string;
class Set
{
public:
    Set(ItemType);
    Set();
    bool empty() const;
    int  size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int i, ItemType& value) const;
    void swap(Set& other);
private:
    int m_size;
    ItemType m_set[DEFAULT_MAX_ITEMS];

};

#endif /* set_hpp */
