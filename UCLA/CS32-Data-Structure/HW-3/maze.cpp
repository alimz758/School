//
//  maze.cpp
//  HW3.3
//
//  Created by ali mirabzadeh on 2/6/19.
//  Copyright © 2019 AMZ Development. All rights reserved.


// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
//using recursion to do assignment
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    //If the start location is equal to the ending location, then we've
    //solved the maze, so return true.
    if(sr==er && sc==ec )
        return true;
    //Mark the start location as visted
    maze[sr][sc]='#';
    //recursively call pathExists based on the direction, north, south, east, west return true at the end, if there was a open spot
    //north
    if (sr!=0 && maze[sr-1][sc]!='X' && maze[sr-1][sc]!='#')
       if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
           return true;
    //south
    if(sr!=9 && maze[sr+1][sc]!='X' && maze[sr+1][sc]!='#')
        if(pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
           return true;
    //west
    if(sc!=0 && maze[sr][sc-1]!='X' && maze[sr][sc-1]!='#')
        if(pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    //east
    if(sc!=9 && maze[sr][sc+1]!='X' && maze[sr][sc+1]!='#')
        if(pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    return false;
}

