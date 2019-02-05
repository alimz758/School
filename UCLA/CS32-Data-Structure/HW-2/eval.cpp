//
//  eval.cpp
//  hw 3.5
//
//  Created by ali mirabzadeh on 2/1/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//
#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;
//               FUNCTION Declaration
string removeSpaces(string infixBeforeRemovingSpaces);
bool isValid (string stringToCheck);
int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result);
bool paranthesis(string pCheck);
string convertToPostfix (string infix, string &postfix);
int precedence(char pre);
int numberOfOperandInString(string expression);

//check whether the parnthesis match or not using stack
bool paranthesis(string pCheck)
{
    //creat a stack using C++ library
    stack <char> paranthesis;
    //loop through the strig
    for (int counter=0; counter<pCheck.size(); counter++)
    {
        //if found '(', push to the stack
        if(pCheck[counter]=='(')
            paranthesis.push('(');
        //if found ')'
        else if(pCheck[counter]==')')
        {
            //check if the stack is not empty and top in the stack is '('
            if(paranthesis.empty() && paranthesis.top()==')')
                return false;
            else
                //if found a matching '(' and ')', pop '(' form the stack
                paranthesis.pop();
        }
    }
    //return true as they were all matching paranthesis
    return paranthesis.empty() ? true:false;
}
//postfix conversion
string convertToPostfix (string infix, string &postfix)
{
    stack <char> conversionStack;
    //Initialize postfix to empty
    postfix="";
    char ch;
    for (int count=0;count<infix.size();count++)
    {
        ch=infix[count];
        if (islower(ch))
            //append ch to end of postfix
            postfix+=ch;
        if(ch=='(')
            conversionStack.push(ch);
        if(ch==')')
        {
            //pop stack until matching '('
            while(conversionStack.top()!='(')
            {
                //append the stack top to postfix
                postfix+=conversionStack.top();
                //pop the stack
                conversionStack.pop();
            }
            //pop the stack --> remove the '('
            conversionStack.pop();
        }
        if(ch == '!' || ch=='&' || ch=='|')
        {
            //While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
            while(!conversionStack.empty() && conversionStack.top()!='(' && (precedence(ch)<=precedence(conversionStack.top())))
            {
                //append the stack top to postfix
                postfix+=conversionStack.top();
                //pop the stack
                conversionStack.pop();
            }
            //push ch onto the stack
            conversionStack.push(ch);
        }
    }
    while (!conversionStack.empty())
    {
        //append the stack top to postfix
        postfix+=conversionStack.top();
        //pop the stack
        conversionStack.pop();
    }
    //return the converted postfix
    return postfix;
}
//precedence of the operator; '!' > '&' > '|'
//assign integer for evaluating precedence of each operator
//return -1 if the char wasn't any of those three
int precedence(char pre)
{
    switch (pre)
    {
        case '!':
            return 2;
        case '&':
            return 1;
        case '|':
            return 0;
        default:
            return -1;
    }
}
//It checks if infix is valid
bool isValid (string stringToCheck)
{
    //first make sure the string does not have space
    string tempToCheck = removeSpaces(stringToCheck);
    //check if the last char is not '|' or '&' or '!' or '!'
    if (tempToCheck[tempToCheck.size()-1]== '|' || tempToCheck[tempToCheck.size()-1]== '&' || tempToCheck[tempToCheck.size()-1]=='(' || tempToCheck[tempToCheck.size()-1]=='!')
        return false;
    //if the string is empty
    if(tempToCheck=="")
        return false;
    //if the first char in the string is either '|' or '&' or ')'
    if (tempToCheck[0]=='&' || tempToCheck[0]=='|' || tempToCheck[0]==')')
        return false;
    //check if the paranthesis match
    if(!paranthesis(stringToCheck))
        return false;
    //loop through the string to check if the string is valid
    for (int counter=0 ; counter<tempToCheck.size();counter++)
    {
        //if there is uppercase char in the string, return false
        if(isupper(tempToCheck[counter]))
             return false;
        //if there is '|' or '&', check its previous and next char
        //before those chars only valid chars are lowercase letters, ')',
        //after only chars and '(', and '!'
        if (tempToCheck[counter]=='&' || tempToCheck[counter]=='|')
            if ((!isalpha(tempToCheck[counter-1]) || tempToCheck[counter-1]=='(') && (!isalpha(tempToCheck[counter+1]) ||
                    tempToCheck[counter+1]==')' || tempToCheck[counter+1]=='!' || tempToCheck[counter+1]=='|' || tempToCheck[counter+1]=='&'))
                 return false;
        //if the current is an alphabet
        if(isalpha(tempToCheck[counter]))
            //check for its next char and if it is an alphabet or it is '!' or '(' then it is false
            if( isalpha(tempToCheck[counter+1]) || tempToCheck[counter+1] == '!' || tempToCheck[counter+1] == '(')
                 return false;
        if(tempToCheck[counter]=='(')
            if (tempToCheck[counter+1] == '&' || tempToCheck[counter+1]== '|' || tempToCheck[counter+1] == ')')
                return false;
    }
    //if non of the cases in the loop met, then the stirng is valid
    //return true
    return true;
}
//remove spaces in the string and retirn a new string without space
string removeSpaces(string infixBeforeRemovingSpaces)
{
    //create a temp stirng that stores the  old string without the spaces
    string tempWithoutSpace="";
    for (int counter=0; counter<infixBeforeRemovingSpaces.size();counter++)
    {
        //if the current char in the string doesn't have space
        if(infixBeforeRemovingSpaces[counter]!=' ')
            //then add that char into the new string
            tempWithoutSpace+=infixBeforeRemovingSpaces[counter];
    }
    return tempWithoutSpace;
}
//evaluates an infix boolean expression that consists of the binary boolean operators
int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
   // If infix is a syntactically valid infix boolean expression whose
    //   only operands are single lower case letters (whether or not they
    //   appear in the values sets), then postfix is set to the postfix
    //   form of the expression.
    if(isValid(infix))
    {
        //converting infix to postfix
        postfix= convertToPostfix(infix, postfix);
        
        
        //result is unchanged and the value the function returns
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
        for (int count=0; count<postfix.length();count++)
        {
            if (islower(postfix[count]))
            {
                // at least one letter in the expression is in neither the
                // trueValues nor the falseValues sets
                if(!trueValues.contains(postfix[count]) && !falseValues.contains(postfix[count]))
                    //If only the first condition holds, the function returns 2
                    return 2;
                //at least one letter in the expression is in both the
                // trueValues and the falseValues set.
                if(trueValues.contains(postfix[count]) && falseValues.contains(postfix[count]))
                    // if only the second holds, the function returns 3
                    return 3;
                //If both hold the function returns either 2 or 3
                if((trueValues.contains(postfix[count]) && falseValues.contains(postfix[count])) ||
                   (!trueValues.contains(postfix[count]) && !falseValues.contains(postfix[count]) ))
                    return 2; // confused by returning 2 or 3
            }
        }
        //EVALUATING
        stack<bool> operandStack;
        char ch;
        if (postfix.empty())
            return 1;
        //loop to evaluate the expression
        for (int count=0; count<postfix.length();count++)
        {
            ch=postfix[count];
            // if ch is an operand
            if (islower(ch))
            {
                //push true, if ch is in the trueValues set
                if(trueValues.contains(ch))
                    operandStack.push(true);
                //push true, if ch is in the falseValues set
                else if(falseValues.contains(ch))
                    operandStack.push(false);
            }
            //for '!', change the boolean
            else if (postfix[count] == '!')
            {
                bool tmp = operandStack.top();
                operandStack.pop();
                operandStack.push(!tmp);
            }
            //ch is a binary operator
            else
            {
                bool operand1,operand2;
                //set operand2 to the top of the operand stack
                operand2=operandStack.top();
                //pop the stack
                operandStack.pop();
                //if stack became empty after pop()
                if (operandStack.empty())
                    return 1;
                //set operand1 to the top of the operand stack
                operand1=operandStack.top();
                //pop the stack
                operandStack.pop();
                //apply the operation that ch represents to operand1 and operand2, and push the result onto the stack
                if (ch == '|')
                {
                    if (operand1 || operand2)
                        operandStack.push(true);
                    else
                        operandStack.push(false);
                }
                else if(ch == '&')
                {
                    if (operand1 && operand2)
                        operandStack.push(true);
                    else
                        operandStack.push(false);
                }
            }
        }
        //When the loop is finished, the operand stack will contain one item,
        //the result of evaluating the expression
        result=operandStack.top();
        return 0;
    }
    // If not, postfix might or might not be
    // changed, result is unchanged, and the function returns 1.
    return 1;
}
//count how many operands are in the string
int numberOfOperandInString(string expression)
{
    int counter=0;
    for (int count=0;count<expression.size();count++)
    {
        //if the char is a lower case letter
        if(islower(expression[count]))
            //increament the counter
            counter++;
    }
    //return the number of operands
    return counter;
}
//client function
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
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0 &&  pf == "ff!tn&&|"  &&  !answer);
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
