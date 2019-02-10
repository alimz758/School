//including the neccessary libraries
#include<stdio.h>
#include<stdlib.h>
//function declartion
char unfrobByte(const char a);
int frobcmp(char const* a, char const* b);
//main function to call other functions
int main()
{
    // Its a pointer so that it can store a variably sized string.
    char* currentWord;
    // Double pointer to store pointers to "currentWords".
    char** allTheWords;
    currentWord = (char*)malloc(sizeof(char));
    // if no current word then print error
    if (currentWord == NULL)
        errorPrint();
    allTheWords = NULL;
    int n=0, m = 0;
    // getting the first char
    currentWord[n] = getchar();
    if (!checkForIOError())
    {
        free(currentWord);
        exit(1);
    }
    // while char isnt EOF and there is no I/O error, do the while loop
    while (currentWord[n] != EOF && checkForIOError())
    {
        // store next char from stdin into the next char
        char next = getchar();
        if (!checkForIOError())
        {
            free(currentWord);
            for (int i = 0; i < m; i++)
                free(allTheWords[i]);
            free(allTheWords);
            exit(1);
        }
        //new word if there is a space
        if (currentWord[n] == ' ')
        {
            // reserve space
            char** temp = realloc(allTheWords, m*sizeof(char*) + sizeof(char*));
            if (temp != NULL)
            {
                allTheWords = temp;
                // point last element of allTheWords to currentWord
                allTheWords[m] = currentWord;
                m++;
                n = -1;
                // make the neccesssary space for a char
                currentWord = (char*)malloc(sizeof(char));
            }
            else
            {
                free (currentWord);
                for (int i = 0; i < m; i++)
                    free(allTheWords[i]);
                free (allTheWords);
                errorPrint();
            }
            //if nex is end of file
            if (next == EOF)
                break;
            while (next == ' ')
            {
                next = getchar();
                checkForIOError();
            }
        }
        else if (next == EOF)
            next = ' ';
        n++;
        char* temp = realloc(currentWord, n*sizeof(char*) + sizeof(char*));
        if (temp != NULL)
        {
            currentWord = temp;
            currentWord[n] = next;
        }
        else
        {
            //free the currentworld
            free (currentWord);
            //then free allthewords
            for (int i = 0; i < m; i++)
                free(allTheWords[i]);
            free (allTheWords);
            //print error with this function call
            errorPrint();
        }
    }
    
    qsort(allTheWords, m, sizeof(char*), genericFrbCmp);
    
    // print the sorted list
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; allTheWords[i][j] != ' '; j++)
        {
            printf("%c", allTheWords[i][j]);
        }
        printf(" ");
    }
    
    // free the memory then exit
    for (size_t i = 0; i < m; i++)
        free (allTheWords[i]);
    free (allTheWords);
    free (currentWord);
    return 0;
}

void errorPrint()
{
    fprintf(stderr, "Error in memory allocation. Exiting the program\n");
    exit(1);
}
//Function to call frbcmp
int genericFrbCmp(const void* a, const void* b)
{
    const char* inp1 = *(const char**)a;
    const char* inp2 = *(const char**)b;
    return frobcmp(inp1, inp2);
}
//frobcmp function as in assignment
int frobcmp(char const* a, char const* b)
{
    for(; ;a++, b++)
    {
        // Null Bytes are allowed
        while (*a == '\0')
            a++;
        while (*b == '\0')
            b++;
        // Space indicates end of word
        if (*a == ' ' && *b == ' ')
            return 0;
        if (*a == ' ' || unfrobByte(*a) < unfrobByte(*b))
            return -1;
        if (*b == ' ' || unfrobByte(*b) < unfrobByte(*a))
            return 1;
    }
}
char unfrobByte(const char a)
    return (a^42);
//it checks if there ia an error  in I/O then output error
int checkForIOError(void)
{
    if (ferror(stdin))
    {
        fprintf(stderr, "I/O Error! Exiting...\n");
        return 0;
    }
    return 1;
}
