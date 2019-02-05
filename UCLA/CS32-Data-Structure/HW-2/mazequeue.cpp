//
//  mazequeue.cpp
//  HW2.3
//
//  Created by ali mirabzadeh on 1/30/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
#include <iostream>
#include <queue>
using namespace std;
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
#include <iostream>

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    //creat a stack
    queue <Coord> coordQueue;
    //starting point
    Coord startingPoint(sr,sc);
    //push the starting point to the stack
    coordQueue.push(startingPoint);
    
    //indicate that the algorithm has encountered
    maze[sr][sc]='$';
    int r=sr;
    int c= sc;
    //while the stack is not empty
    while (!coordQueue.empty())
    {
        
        //storing the starting point
        startingPoint = coordQueue.front();
        r = startingPoint.r();
        c = startingPoint.c();
        //pop the top coordinate
        coordQueue.pop();
        
        // If the current (r,c) coordinate is equal to the ending coordinate,
        //then we've solved the maze so return true!
        if (r==er && c==ec)
            return true;
        //Check each place you can move from the current cell as follows:
        // If you can move SOUTH and haven't encountered that cell yet,
        //then push the coordinate (r+1,c) onto the stack and update
        //maze[r+1][c] to indicate the algorithm has encountered it.
        if(r!=9 && maze[r+1][c] != 'X' && maze[r+1][c]!='$')
        {
            //was enable to move south
            Coord moved(r+1,c);
            //push to the stack
            coordQueue.push(moved);
            //met the algorythm
            maze[r+1][c]='$';
        }
        //        If you can move WEST and haven't encountered that cell yet,
        //        then push the coordinate (r,c-1) onto the stack and update
        //        maze[r][c-1] to indicate the algorithm has encountered it.
        if(c!=0 && maze[r][c-1] != 'X' && maze[r][c-1]!='$')
        {
            //was enable to move west
            Coord moved(r,c-1);
            //push to the stack
            coordQueue.push(moved);
            //met the algorythm
            maze[r][c-1]='$';
        }
        //        If you can move NORTH and haven't encountered that cell yet,
        //        then push the coordinate (r-1,c) onto the stack and update
        //        maze[r-1][c] to indicate the algorithm has encountered it.
        if(r!=0 && maze[r-1][c] != 'X' && maze[r-1][c]!='$')
        {
            //was enable to move north
            Coord moved(r-1,c);
            //push to the stack
            coordQueue.push(moved);
            //met the algorythm
            maze[r-1][c]='$';
        }
        //        If you can move EAST and haven't encountered that cell yet,
        //        then push the coordinate (r,c+1) onto the stack and update
        //        maze[r][c+1] to indicate the algorithm has encountered it.
        if(c!=9 && maze[r][c+1] != 'X' && maze[r][c+1]!='$')
        {
            //was enable to move east
            Coord moved(r,c+1);
            //push to the stack
            coordQueue.push(moved);
            //met the algorythm
            maze[r][c+1]='$';
        }

    }
    
    return false;
}


//client function
int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X....X...X",
        "X.XX.XX..X",
        "XXX....X.X",
        "X.XXX.XXXX",
        "X.X...X..X",
        "X...X.X..X",
        "XXXXX.X.XX",
        "X........X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    
}
