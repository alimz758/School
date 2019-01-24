//
//  History.cpp
//  Zombies
//
//  Created by ali mirabzadeh on 1/9/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

//Constructor
History :: History(int nRows, int nCols)
{
    //initializing the member functions of history with the passed arguments
    m_col=nCols;
    m_row=nRows;
    //making a m_row * m_col grid with dots
    for (int r=1 ; r<= m_row; r++)
    {
        for(int c=1; c<= m_col; c++)
        {
            m_grid[r][c]='.';
        }
    }
}
//record function; that keeps track of dead zombies
bool History :: record(int r, int c)
{
    //if in the bound
    if ((r >= 0 && r < m_row) && (c >= 0 && c < m_col))
    {
        if (m_grid[r][c] >= 'A' && m_grid[r][c] <= 'Z')
            m_grid[r][c] += 1;
        else
            m_grid[r][c] = 'A';
        return true;
    }
    else
        return false;
}
//display the history
void History :: display() const
{
    clearScreen();
    for (int r = 1; r <= m_row; r++)
    {
        for (int c = 1; c <= m_col; c++)
            cout << m_grid[r][c];
        cout << endl;
    }
    cout << endl;
}
