//
//  Set.cpp
//  eval
//
//  Created by ali mirabzadeh on 2/1/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//
#include "Set.h"
#include <iostream>
// Create an empty set (i.e., one with no items).
Set::Set():m_size(0)
{
    //Creating a dummy node that points to itself
    head = new List;
    head->next=head;
    head->prev=head;
}
//copy constructor
Set :: Set(const Set& other):
//copy the size of "other" into the current object
m_size(other.size())
{
    //creatig a new dummy head
    head = new List;
    head->next = head;
    head->prev = head;
    //loop through "other"
    for (List* p = other.head->next; p != other.head; p = p->next) // Loop through the current Sequence.
    {
        //creating a new List
        List* n = new List;
        //swaping the values
        n->value = p->value;
        n->next = head;
        n->prev = head->prev;
        head->prev->next = n;
        head->prev = n;
    }
}
//Destructor
Set :: ~Set()
{
    while (head->prev != head)
    {
        List* p = head->prev;
        p->prev->next= head;
        head->prev = p->prev;
        delete p;
    }
    delete head;
}
//Assignmnt operatior
Set& Set :: operator = (const Set &other)
{
    //if the same list is being assigned
    if(this==&other)
        //return itself
        return *this;
    else
    {
        Set temp= other;
        swap(temp);
    }
    return *this;
}
//if there is no string in the string set; return true
bool  Set ::empty() const// Return true if the set is empty, otherwise false.
{
    if (m_size == 0)
        return true;
    //else return false
    return false;
}
//return the size of the set
int Set :: size() const// Return the number of items in the set.
{
    return m_size;
}
// Insert value into the set if it is not already present.  Return
// true if the value was actually inserted.  Leave the set unchanged
// and return false if the value was not inserted (perhaps because it
// was already in the set or because the set has a fixed capacity and
// is full).
bool Set :: insert(const ItemType& value)
{
    if(!empty())
    {
        //check whether the value is in the List or not
        //if it is in the list, skip inserting and return false
        List *temp= head->next;
        while(temp!=head)
        {
            //the node with "Value already exist"
            if (temp->value==value)
                return false;
            else
            {
                temp=temp->next;
                continue;
            }
        }
        //if it was not in the list
        List *itr= head->next;
        while(itr!=head)
        {
            //if the passed value is samller than the current node, insert it before the current node
            if(itr->value>=value)
            {
                //creating a new node and fit it before "itr" and the node before "itr"
                List* newNode= new List;
                newNode->value=value;
                newNode->next=itr;
                newNode->prev=itr->prev;
                itr->prev->next=newNode;
                itr->prev=newNode;
                m_size++;
                return true;
            }
            //keep going util the above statement runs
            // if not, will exit the loop
            itr=itr->next;
        }
        //if value is biger all the values in the node
        //insert at the end
        List *p = head->prev;//hold the address of the last node
        List *newNode= new List;//creating a new node
        newNode->value= value;
        newNode->next=head;
        newNode->prev=p;
        p->next=newNode;
        head->prev=newNode;
        m_size++;
        return true;
    }
    //if the list is empty, add to the front
    else
    {
        //then create a new node List
        List *newList = new List;
        newList->value=value;
        newList->next=head;
        newList->prev=head;
        //make head points to the New List
        head->next=newList;
        head->prev=newList;
        //incrementing the size of list
        m_size++;
        //node got inserted, return true
        return true;
    }
}
//Check whether a particular value exists in the Node
bool Set :: contains(const ItemType& value) const
{
    //creat a Node that has the address of head
    List *itr=head;
    //loop through the Node
    while(itr->next!=head)
    {
        itr=itr->next;
        //if found the value in the list, return true
        if(itr->value==value)
        {
            return true;
        }
        //if has not found yet, keep going
        else
            continue;
    }
    //if didn't find, return false
    return false;
}
// Remove the value from the set if present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.
bool Set :: erase(const ItemType& value)
{
    //loop through the List
    for(List *itr =head->next; itr!=head;itr=itr->next)
    {
        if (itr->value==value)
        {
            List *del = itr;
            //connceting the noxt node after "itr"
            // to node before "itr"
            itr->prev->next= del->next;
            itr->next->prev=del->prev;
            //delete itr node
            delete del;
            //decrement the size
            m_size--;
            return true;
        }
        else
            continue;
    }
    return false;
}
// If 0 <= i < size(), copy into value the item in the set that is
// strictly greater than exactly i items in the set and return true.
// Otherwise, leave value unchanged and return false.
bool Set :: get(int i, ItemType& value) const
{
    //if "i" is not in the range, return false
    if (i < 0 || i >= m_size)
        return false;
    else
    {
        //creat a new node that points to head so it can iterate through the current node
        List *p=head;
        //loop through the node till get "i"
        for (int count =0; count <size();count++)
        {
            p=p->next;
            if (count==i)
            {
                //copy the passed value to the desired node then return true
                value=p->value;
                return true;
            }
            else
                continue;
        }
    }
    return false;
}
// Exchange the contents of this set with the other one.
void Set :: swap(Set& other)
{
    //swapping two List by excvhanging their sizes and address of their heads
    List *tempHead= other.head;
    other.head=head;
    head=tempHead;
    
    int tempSize= other.m_size;
    other.m_size=m_size;
    m_size=tempSize;
}
//non-member function that unite all data in s1 and s2 and store all in a new list called result
void unite(const Set& s1, const Set& s2, Set& result)
{
    //creat temp Set to avoid aliasing
    Set temp=result;
    ItemType dataInResult;
    //if result is not empty
    while(!temp.empty())
    {
        //get its data and store in dataInResult
        temp.get(0,dataInResult);
        //then erase that data from result
        temp.erase(dataInResult);
    }
    //if s1 is empty
    if(s1.empty())
        temp=s2;
    else if(s2.empty())
        temp=s1;
    else
    {
        ItemType data;
        //insert data from s1 to temp
        for (int count=0; count<s1.size();count++)
        {
            s1.get(count,data);
            temp.insert(data);
        }
        ItemType value;
        //insert data from s2 to
        for (int i=0;i<s2.size();i++)
        {
            s2.get(i,value);
            temp.insert(value);
        }
    }
    result=temp;
}
//this function would store list of nodes in result that have datas that are not in the other two sets
void subtract(const Set& s1, const Set& s2, Set& result)
{
    Set temp=result;
    ItemType dataInResult;
    //if result is not empty
    while(!temp.empty())
    {
        //get the data from the first node
        temp.get(0,dataInResult);
        //then erase that node from result
        temp.erase(dataInResult);
    }
    //if s1 is empty, copy s2 in result
    if(s1.empty())
        temp=s2;
    //if s2 is empty, copy s1 in result
    else if(s2.empty())
        temp=s1;
    else
    {
        ItemType data;
        for (int count=0; count<s1.size();count++)
        {
            //get data from s1
            s1.get(count,data);
            //if the data in s1 exist in s2
            if (s2.contains(data))
                //skip and continue the loop
                continue;
            //id the data doesn't exist in s2
            else
                //insert the
                temp.insert(data);
        }
    }
    result=temp;
}

