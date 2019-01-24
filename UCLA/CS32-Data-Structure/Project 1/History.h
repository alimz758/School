//
//  History.hpp
//  Zombies
//
//  Created by ali mirabzadeh on 1/9/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

#ifndef History_hpp
#define History_hpp

#include <stdio.h>
#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;

private:
    int m_row;
    int m_col;
    char m_grid[MAXROWS][MAXCOLS];
    
};
#endif /* History_hpp */
