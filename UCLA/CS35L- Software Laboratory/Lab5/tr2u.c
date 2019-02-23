#include <stdlib.h>
#include <unistd.h>//for read and write                                                                                                                                                                                                       
#include <stdio.h>
#include <string.h>
int main(int argNum, char **arg)
{
  //output error in stderr if the number of argumnets are not equal to 3, including the program itself                                                                                                                                       \
                                                                                                                                                                                                                                              
  if(argNum!=3)
    {
      fprintf(stderr, "ERROR! Number of arguments should be two.");
      exit(1);
    }

  char *from=arg[1];
  char *to= arg[2];

  //if the length of from and to are no the same, print error                                                                                                                                                                                \
                                                                                                                                                                                                                                              
  if(strlen(from)!=strlen(to))
    {
      fprintf(stderr, "ERROR! Length of the two arguments are not the same.");
      exit(1);
    }

  //check if from has duplicates                                                                                                                                                                                                              
  size_t i,j,k;
  for (i=0 ; i<strlen(from);i++)
    {
      for(j=i+1;j<strlen(from);j++)
        {
          if(from[i]==from[j])
            {
              fprintf(stderr, "ERROR! Duplicates found in from.");
              exit(1);
            }
        }
    }
  //char array that the read content will be stored                                                                                                                                                                                           
  //The nbyte arguments to read and write should be 1, so that the program reads and writes single bytes at a time                                                                                                                            
  char content[1];

  //0 refers to standard input                                                                                                                                                                                                                
  //1 refers to the number of bytes to read before truncating the data. If the data to be read is smaller than nbytes, all data is saved in the buffer                                                                                        
  ssize_t readStatus=  read(0, content, 1);

  //do a while loop till get to end of file                                                                                                                                                                                                   
  while(readStatus>0)
    {
      for (k=0; k<strlen(from);k++)
        {
          if (from[k]==content[0])
            {
              content[0]=to[k];
              break;
            }
        }

      //use 1 for standard output                                                                                                                                                                                                             
      //The second parameter: A pointer to a buffer of at least nbytes bytes, which will be written to the file.                                                                                                                              
      //The third: The number of bytes to write. If smaller than the provided buffer, the output is truncated.                                                                                                                                
      write( 1, content , 1);
      readStatus= read(0, content, 1);
    }
  return 0;
}
