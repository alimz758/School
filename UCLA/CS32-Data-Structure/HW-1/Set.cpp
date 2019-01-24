//
//  set.cpp
//  Homework#1
//
//  Created by ali mirabzadeh on 1/16/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//
#include "Set.h"


// Create an empty set (i.e., one with no items).
Set::Set()
{
    m_size=0;
}
//if there is no string in the string set; return true
bool  Set ::empty() const// Return true if the set is empty, otherwise false.
{
    if (m_size == 0)
        return true;
    else
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
    //ItemType temp;
    if (m_size<=DEFAULT_MAX_ITEMS)
    {
        //if the set is not empty
        if (!empty())
        {
            //check whether the value is in the set
            for(int j=0 ; j<size();j++)
            {
                if(m_set[j]==value)
                    return false;
                else
                    continue;
            }
            //if value wasn't in the set loop through to insert it
            for(int count =0; count<size();count++)
            {
                //if the value is smaller than the current elemnt in the set
                if(m_set[count]>=value)
                {
                    //increase the size of the set
                    m_size++;
                    //shift the set from the end
                    for (int i=size() ; i>count; i--)
                    {
                        m_set[i]=m_set[i-1];
                    }
                    //after shifting put the value
                    m_set[count]=value;
                    return true;
                }
                else
                    continue;
            }
            //insert at the end of the list and return true
            m_set[m_size++]=value;
            return true;
        }
        else
        {
            //copy value in the first element of m_set
            m_set[m_size++]=value;
            return true;
        }
    }
    else
        return false;
}
// Remove the value from the set if present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.
bool Set :: erase(const ItemType& value)
{
    for (int counter=0; counter <size() ; counter++)
    {
        //if found the item
        if(m_set[counter]==value)
        {
            //start looping through the array from that item to the end
            for (int counter2=counter;counter2<size();counter2++)
            {
                //copy the next element to the previous one
                m_set[counter2]=m_set[counter2+1];
            }
            //decrement the size of the array
            m_size--;
            return true;
        }
        else
            continue;
    }
    return false;
}
// Return true if the value is in the set, otherwise false.
bool Set :: contains(const ItemType& value) const
{
    for (int counter=0; counter <size() ; counter++)
    {
        if (m_set[counter]==value)
            return true;
        else
            continue;
    }
    //return false if item was not in the array
    return false;
}
// If 0 <= i < size(), copy into value the item in the set that is
// strictly greater than exactly i items in the set and return true.
// Otherwise, leave value unchanged and return false.
bool Set :: get(int i, ItemType& value) const
{
    if (i>=0 && i<size())
    {
        //copy the item greater than i from the array into value
        value= m_set[i];
        return true;
    }
    else
        return false;
}
// Exchange the contents of this set with the other one.
void Set :: swap(Set& other)
{
    ItemType tempArr[DEFAULT_MAX_ITEMS];
    int tempSize=  m_size;
    m_size=other.m_size;
    other.m_size=tempSize;
    for(int count=0;count<DEFAULT_MAX_ITEMS;count++)
    {
        tempArr[count]=other.m_set[count];
        other.m_set[count]=m_set[count];
        m_set[count]=tempArr[count];
    }
}
