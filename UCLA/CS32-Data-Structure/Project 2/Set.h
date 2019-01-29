//
//  Set.h
//  Project 2
//
//  Created by ali mirabzadeh on 1/23/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

#ifndef Set_h
#define Set_h
#include <string>
using namespace std;
using ItemType = string;
//Declration of calss Set
class Set
{
    //public member functions
    public:
        Set();
        Set(const Set& other);
        bool empty() const;
        int size() const;
        bool insert(const ItemType& value);
        Set& operator = (const Set &other);
        bool erase(const ItemType& value);
        bool contains(const ItemType& value) const;
        bool get(int pos, ItemType& value) const;
        void swap(Set& other);
        void dump() const;
        ~Set();
    //private memebers
    private:
        struct List{
            ItemType value;
            List *next;
            List *prev;
        };
        //head of node
        List *head;
        //store size of the List
        int m_size;
};
//non-member functions
void subtract(const Set& s1, const Set& s2, Set& result);
void unite(const Set& s1, const Set& s2, Set& result);
#endif /* Set_hpp */
