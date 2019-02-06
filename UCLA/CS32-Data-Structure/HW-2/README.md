Write a C++ function named pathExists that determines whether or not a there's a path from start to finish in a rectangular maze. Here is the prototype:

        bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
          // Return true if there is a path from (sr,sc) to (er,ec)
          // through the maze; return false otherwise
The parameters are

A rectangular maze of Xs and dots that represents the maze. Each string of the array is a row of the maze. Each 'X' represents a wall, and each '.' represents a walkway.
The number of rows in the maze.
The number of columns in the maze. Each string in the maze parameter must be this length.
The starting coordinates in the maze: sr, sc; the row number is in the range 0 through nRows−1, and the column number is in the range 0 through nCols−1.
The ending coordinates in the maze: er, ec; the row number is in the range 0 through nRows−1, and the column number is in the range 0 through nCols−1.
Here is an example of a simple maze with 5 rows and 7 columns:

        "XXXXXXX"
        "X...X.X"
        "XXX.X.X"
        "X.....X"
        "XXXXXXX"
The function must return true if in the maze as it was when the function was called, there is a path from maze[sr][sc] to maze[er][ec] that includes only walkways, no walls; otherwise it must return false. The path may turn north, east, south, and west, but not diagonally. When the function returns, it is allowable for the maze to have been modified by the function.

Your solution must use the following simple class (without any changes), which represents an (r,c) coordinate pair:

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

(Our convention is that (0,0) is the northwest (upper left) corner, with south (down) being the increasing r direction and east (right) being the increasing c direction.)

Your implementation must use a stack data structure, specifically, a stack of Coords. You may either write your own stack class, or use the stack type from the C++ Standard Library. Here's an example of the relevant functions in the library's stack type:

        #include <stack>
        using namespace std;

        int main()
        {
            stack<Coord> coordStack;     // declare a stack of Coords
        
            Coord a(5,6);
            coordStack.push(a);             // push the coordinate (5,6)
            coordStack.push(Coord(3,4));    // push the coordinate (3,4)
            ...
            Coord b = coordStack.top();     // look at top item in the stack
            coordStack.pop();               // remove the top item from stack
            if (coordStack.empty())         // Is the stack empty?
                cout << "empty!" << endl;
            cout << coordStack.size() << endl;  // num of elements
        }
Here is pseudocode for your function:

        Push the starting coordinate (sr,sc) onto the coordinate stack and
            update maze[sr][sc] to indicate that the algorithm has encountered
            it (i.e., set maze[sr][sc] to have a value other than '.').
        While the stack is not empty,
        {   Pop the top coordinate off the stack. This gives you the current
                (r,c) location that your algorithm is exploring.
            If the current (r,c) coordinate is equal to the ending coordinate,
                then we've solved the maze so return true! 
            Check each place you can move from the current cell as follows:
                If you can move SOUTH and haven't encountered that cell yet,
                    then push the coordinate (r+1,c) onto the stack and update
                    maze[r+1][c] to indicate the algorithm has encountered it.
                If you can move WEST and haven't encountered that cell yet,
                    then push the coordinate (r,c-1) onto the stack and update
                    maze[r][c-1] to indicate the algorithm has encountered it.
                If you can move NORTH and haven't encountered that cell yet,
                    then push the coordinate (r-1,c) onto the stack and update
                    maze[r-1][c] to indicate the algorithm has encountered it.
                If you can move EAST and haven't encountered that cell yet,
                    then push the coordinate (r,c+1) onto the stack and update
                    maze[r][c+1] to indicate the algorithm has encountered it.
        }
        There was no solution, so return false
Here is how a client might use your function:

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
Because the focus of this homework is on practice with the data structures, we won't demand that your function be as robust as we normally would. In particular, your function may make the following simplifying assumptions (i.e., you do not have to check for violations):

the maze array contains nRows rows (you couldn't check for this anyway);
each string in the maze is of length nCols;
the maze contains only Xs and dots when passed in to the function;
the top and bottom rows of the maze contain only Xs, as do the left and right columns;
sr and er are between 0 and nRows-1, and sc and ec are between 0 and nCols-1;
maze[sr][sc] and maze[er][ec] are '.' (i.e., not walls)
(Of course, since your function is not checking for violations of these conditions, make sure you don't pass bad values to the function when you test it.)

For this part of the homework, you will turn in one file named mazestack.cpp that contains the Coord class and your stack-based pathExists function. (Do not leave out the Coord class and do not put it in a separate file.) If you use the library's stack type, your file should include the appropriate header.

Given the algorithm, main function, and maze shown at the end of problem 1, what are the first 12 (r,c) coordinates popped off the stack by the algorithm?

For this problem, you'll turn in either a Word document named hw.docx or hw.doc, or a text file named hw.txt, that has your answer to this problem (and problem 4).

Now convert your pathExists function to use a queue instead of a stack, making the fewest changes to achieve this. You may either write your own queue class, or use the queue type from the C++ Standard Library:

        #include <queue>
        using namespace std;
        
        int main()
        {
            queue<Coord> coordQueue;    // declare a queue of Coords
        
            Coord a(5,6);
            coordQueue.push(a);            // enqueue item at back of queue
            coordQueue.push(Coord(3,4));   // enqueue item at back of queue
            ...
            Coord b = coordQueue.front();  // look at front item
            coordQueue.pop();              // remove the front item from queue
            if (coordQueue.empty())        // Is the queue empty?
                cout << "empty!" << endl;
            cout << coordQueue.size() << endl;  // num of elements
        }
For this part of the homework, you will turn in one file named mazequeue.cpp that contains the Coord class and your queue-based pathExists function. (Do not leave out the Coord class and do not put it in a separate file.) If you use the library's queue type, your file should include the appropriate header.

Given the same main function and maze as are shown at the end of problem 1, what are the first 12 (r,c) coordinates popped from the queue in your queue-based algorithm?

How do the two algorithms differ from each other? (Hint: how and why do they visit cells in the maze in a different order?)

For this problem, you'll turn in either a Word document named hw.docx or hw.doc, or a text file named hw.txt, that has your answer to this problem (and problem 2).

Implement this function that evaluates an infix boolean expression that consists of the binary boolean operators & (meaning and) and | (meaning inclusive or), the unary boolean operator ! (meaning not), parentheses, and operands (with blanks allowed for readability). Following convention, ! has higher precedence than &, which has higher precedence than |, and the operators & and | associate left to right, so the postfix form of a|b|a is ab|a|, not aba||, which would be the postfix form of a|(b|a).

The operands in the expression are single lower case letters. Each letter represents the value true or false. Along with the infix expression string, you will pass the function two Sets whose elements are of type char; one Set contains the letters that represent true, and the other contains the letters that represent false. For example, if the Set of letters representing true contains a, c, l, and u, while the other Set contains n, s, and x, then here are some infix expressions and what they evaluate to:

        u                          evaluates to true
        u&c&l&a & !(u&s&c)         evalates to true
        (n)                        evaluates to false
        a&(s)                      evaluates to false
        a & !s                     evaluates to true
        !(n|u)                     evaluates to false
        !n|u                       evaluates to true
        a|n&n                      evaluates to true
        a&!(s|u&c|n)|!!!(s&u&n)    evaluates to true
Here is the function:

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
  // Evaluate a boolean expression
  //   If infix is a syntactically valid infix boolean expression whose
  //   only operands are single lower case letters (whether or not they
  //   appear in the values sets), then postfix is set to the postfix
  //   form of the expression.  If not, postfix might or might not be
  //   changed, result is unchanged, and the function returns 1.
  //
  //   If infix is a syntactically valid infix boolean expression whose
  //   only operands are single lower case letters:
  //
  //      If every operand letter in the expression appears in either
  //      trueValues or falseValues but not both, then result is set to the
  //      result of evaluating the expression (using for each letter in the
  //      expression the value true if that letter appears in trueValues or
  //      false if that letter appears in false values) and the function
  //      returns 0.
  //
  //      Otherwise, result is unchanged and the value the function returns
  //      depends on these two conditions:
  //        at least one letter in the expression is in neither the
  //            trueValues nor the falseValues sets; and
  //        at least one letter in the expression is in both the
  //            trueValues and the falseValues set.
  //      If only the first condition holds, the function returns 2; if
  //      only the second holds, the function returns 3.  If both hold
  //      the function returns either 2 or 3 (and the function is not
  //      required to return the same one if called another time with the
  //      same arguments).
Adapt the algorithms presented on pp. 203-207 of the textbook to convert the infix expression to postfix, then evaluate the postfix form of the expression. The algorithms use stacks. Rather than implementing the stack types yourself, you must use the stack class template from the Standard C++ library. You must not assume that the infix string passed to the function is syntactically valid; part of adapting the algorithm from the text is adding code to detect whether or not the infix string is syntactically valid.

For this problem, you will turn in a file named eval.cpp whose structure is probably of the form

        #include lines you need, including "Set.h"

        declarations of any additional functions you might have written
                to help you evaluate an expression

	int evaluate(string infix, const Set& trueValues, const Set& falseValues,
	             string& postfix, bool& result)
        {
            your expression evaluation code
        }

        implementations of any additional functions you might have written
                to help you evaluate an expression

        a main routine to test your function
Use a correct implementation of Set (perhaps from the Homework 1 or Project 2 solution). Build your program from eval.cpp and Set.cpp. You will not turn in Set.h or Set.cpp. (We will use correct versions when we test your code.)

If we take your eval.cpp file, rename your main routine (which we will never call) to something harmless, prepend the lines

        #include "Set.h"  // with ItemType being a type alias for char
        #include <iostream>
        #include <string>
        #include <stack>
        #include <cctype>
        #include <cassert>
        using namespace std;
if necessary, and append the lines

        int main()
        {
            string trueChars  = "tywz";
            string falseChars = "fnx";
            Set trues;
            Set falses;
            for (int k = 0; k < trueChars.size(); k++)
                trues.insert(trueChars[k]);
            for (int k = 0; k < falseChars.size(); k++)
                falses.insert(falseChars[k]);

            string pf;
            bool answer;
            assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
            assert(evaluate("y|", trues, falses, pf, answer) == 1);
            assert(evaluate("n t", trues, falses, pf, answer) == 1);
            assert(evaluate("nt", trues, falses, pf, answer) == 1);
            assert(evaluate("()", trues, falses, pf, answer) == 1);
            assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
            assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
            assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
            assert(evaluate("", trues, falses, pf, answer) == 1);
            assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
                                   &&  pf == "ff!tn&&|"  &&  !answer);
            assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
            trues.insert('x');
            assert(evaluate("((x))", trues, falses, pf, answer) == 3);
            falses.erase('x');
            assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
            trues.erase('w');
            assert(evaluate("w| f", trues, falses, pf, answer) == 2);
            falses.insert('w');
            assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
            cout << "Passed all tests" << endl;
        }
then the resulting file must compile and build successfully when Set.h and Set.cpp are present, and when executed, produce no output other than Passed all tests.

Do not use variables named and, or, or not; the g++ and clang++ compilers treat these as keywords (as the C++ Standard requires) even though Visual C++ doesn't.

(Tips: In case you didn't already know it, you can append a character c to a string s by saying s += c. You'll have to adapt the code from the book, since it doesn't do any error checking and assumes that all operators are binary operators. It's possible to do the error checking as you do the infix-to-postfix conversion instead of in a separate step before that; as you go through the infix string, almost all syntax errors can be detected by noting whether it is legal for the current nonblank character to follow the nearest nonblank character before it. When converting infix to postfix, a unary operator like ! behaves a lot more like an open parenthesis than like a binary operator.)

By Monday, February 4, there will be a link on the class webpage that will enable you to turn in this homework. Turn in one zip file that contains your solutions to the homework problems. The zip file should contain

mazestack.cpp, if you solved problem 1
mazequeue.cpp, if you solved problem 3
eval.cpp, if you solved problem 5
hw.docx, hw.doc, or hw.txt, if you solved problems 2 and/or 4
Each source file you turn in may or may not contain a main routine; we'd prefer that it doesn't. If it does, our testing scripts will rename your main routine to something harmless. Our scripts will append our own main routine, then compile and run the resulting source file. Therefore, to get any credit, each source file you turn in must at least compile successfully (even though it's allowed to not link because of a missing main routine).

In each source file you turn in, do not comment out your implementation; you want our test scripts to see it! (Some people do this when testing other files' code because they foolishly put all their code in one project instead of having a separate project for each of problems 1, 3, and 5.)
