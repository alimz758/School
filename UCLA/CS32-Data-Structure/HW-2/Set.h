//
//  Set.hpp
//  eval
//
//  Created by ali mirabzadeh on 2/1/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

#ifndef Set_h
#define Set_h

using ItemType = char;
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

#endif /* Set_hpp */
