/*
NAME: Ali Mirabzadeh
EMAIL: thealimz758@ucla.edu
ID: 305179067
Lab0
*/
#include "stdio.h"
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
void readFromWriteIn(int ifd, int ofd){
    char buf;
    //inifint loop till hit the break
 	while (1){
         //read till end of the line
		if (read(ifd, &buf, sizeof(char)) == 0) 
			break;
        write(ofd, &buf, sizeof(char));
	}
}
//force to sefault
void forcedSegFault(){
    char* segPointer = NULL;
    *segPointer = 'N';
}
//signal handler
void signalHandler(int signal){
    if(signal==SIGSEGV){
        fprintf(stderr, "Segmentation Fault!\n");
        exit(4);
    } 
}

int main(int argc, char **argv)
{
    //Input and output files
	char* inputFile = 0;
	char* outputFile = 0;
    //used http://man7.org/linux/man-pages/man3/getopt.3.html to use getopt
    int c;
    static struct option long_options[] = {
        {"input",   1,  0,  'i' },
        {"output",  1,  0, 'o' },
        {"segfault",  0, 0,  's'},
        {"catch",    0, 0,    'c' },
        {"dump-core",    0, 0,    'd' },
        {0,         0,  0,    0 }
    };
    while (1) {
        c = getopt_long(argc, argv, "i:o:sc", long_options,NULL);
        if (c == -1)
            break;
        switch (c) {
            //input
            case 'i':
                inputFile=optarg;
                break;
            //output
            case 'o':
                outputFile=optarg;
                break;
            //segmentation falut
            case 's':
                forcedSegFault();
                break;
            //catch
            case 'c':
                signal(SIGSEGV,signalHandler);
                break;
            case 'd':
                signal(SIGSEGV,SIG_DFL);
                break;
            default:
                fprintf(stderr,"Unrecognized argument! Try again %s!\n", strerror(errno));
                exit(1);
            }
    }
    //used http://web.cs.ucla.edu/classes/fall19/cs111/labs/fd_juggling.html
    //Input Redirection
    //Open the file
    if(inputFile){
        int ifd = open(inputFile, O_RDONLY);
        //If such a file exist
        if (ifd >= 0) {
            //closing the file descriptor (standard input read-only) to be replaced
            close(0);
            dup(ifd);
            close(ifd);
        }
        else{
            //could not open the file
            fprintf(stderr,"--input failed. Could not open %s due to error: %s\n", inputFile, strerror(errno));
            //exit with 2: unable to open input file
            exit(2);
        }
    }
    if(outputFile){
        //Output Redirection
        int ofd = creat(outputFile, 0666);
        if (ofd >= 0) {
            close(1);
            dup(ofd);
            close(ofd);
        }
        //unable to open output file
        else{
            fprintf(stderr,"--output failed. Could not open %s due to error: %s\n", outputFile, strerror(errno));
            //exit with 3: unable to open output file
            exit(3);
        }
    }
    //copy from stdin to write stdout
    readFromWriteIn(0,1);
    //copy successful
	exit(0);
}