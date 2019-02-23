#include <stdio.h> //perform I/O operations                                                                                                                                                                                                   
#include <stdlib.h>
#include <string.h>
//main function                                                                                                                                                                                                                               
int main(int argNum, char **arg)
{
  //output error in stderr if the number of argumnets are not equal to 3, including the program itself                                                                                                                                        
  if(argNum!=3)
    {
      fprintf(stderr, "ERROR! Number of arguments should be two.");
      exit(1);
    }

  char *from=arg[1];
  char *to= arg[2];

  //if the length of from and to are no the same, print error                                                                                                                                                                                 
  if(strlen(from)!=strlen(to))
    {
      fprintf(stderr, "ERROR! Length of the two arguments are not the same.");
      exit(1);
    }
  size_t i,j;
  //check if from has duplicates                                                                                                                                                                                                              
  for ( i=0 ; i<strlen(from);i++)
    {
      for( j=i+1;j<strlen(from);j++)
	{
	  if(from[i]==from[j])
	    {
	      fprintf(stderr, "ERROR! Duplicates found in from.");
	      exit(1);
	    }
	}
    }
  //copying standard input to standard output, transliterating every byte in from to the corresponding byte in to                                                                                                                             
  int cur= getchar();
  size_t k;
  while(cur!=EOF)
    {
      for (k=0 ; k<strlen(from);k++)
	{
	  if (cur== from[k])
	    {
	      cur=to[k];
	      break;
	    }
	}
      //using putchar(): Writes a character to the standard output (stdout)                                                                                                                                                                     
      putchar(cur);
      //get a another cahr                                                                                                                                                                                                                      
      cur=getchar();

    }
  return 0;
}


