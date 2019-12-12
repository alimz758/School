#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <ctype.h>
#include <mraa.h>
float tempConverter();
void timeFunction();
void readLine(char * );
void report(char * , int );
void shutdown();

#define TEMPPIN 1
#define BUTTONPIN 60
//for temp and button
mraa_aio_context temperature;
mraa_gpio_context button;
struct timeval timer;
int logFlag = 1;
time_t n_time = 0;
struct tm* currentTime;
//default period
int period=1;
//By default, temperatures should be reported in degrees Fahrenheit
char scale='F';
//file descriptor for stdout
FILE *  fileDescriptorOutput= 0; 
// temp algo
// used http://wiki.seeedstudio.com/Grove-Temperature_Sensor_V1.2/
float tempConverter(){
    int tempFromBB = mraa_aio_read(temperature);
    int thermis = 4275;
    float r0 = 100000;
    float r = 1023.0/tempFromBB-1.0;
    r *= r0;
    float tempInC = 1.0/(log(r/r0)/thermis+1/298.15)-273.15;
    //F
    if(scale == 'F'){
        // convert to F
        return (tempInC * (9/5)) + 32;
    }
    //C
    else{
        return tempInC;
    }
}
//time function
//thiswould basically save, sprintf, the time and temp as desired
void timeFunction(){
    gettimeofday(&timer, 0);
    if (logFlag && timer.tv_sec >= n_time) {
        //get the temp by calling the function
        float temperatureVal = tempConverter();
        //will be used for a decimal temperature in degrees and tenths (e.g. 98.6
        int tempFormated = temperatureVal * 10;
        //time of the sample (e.g. 17:25:58) in the local timezone
        currentTime = localtime(&timer.tv_sec);
        char timeOutput[200];
        //store in output to be printed in report
        sprintf(timeOutput, "%02d:%02d:%02d %d.%1d", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, tempFormated/10, tempFormated%10);
        report(timeOutput,1);
        n_time = timer.tv_sec + period;
    }
}
//report to be written in stdout
//The idea is not from me; to use sprintf and print all one place
void report(char * toPrint, int toSTDOut ){
    //if for stdout
    if(toSTDOut==1){
        fprintf(stdout,"%s\n", toPrint);
    }
    //if fd
    if(fileDescriptorOutput!=0){
        fprintf(fileDescriptorOutput, "%s\n", toPrint);
        fflush(fileDescriptorOutput);
    }
}
//read the line
void readLine(char *input){
    int inputLength = strlen(input);
    //end of the line with 0; last element of char*
    input[inputLength - 1] = '\0';
    while(*input == '\t' || *input == ' '){
        input++;
    }
    //---------------------
    //process the input and check with the following statements then process accordingly 
    //---------------
    //extract the preiod
    //find the pointer that points to PERIOD
    char *periodPointer = strstr(input, "PERIOD=");
    //If periodPointer is the same as input
    if(periodPointer==input){
        //extract the number
        char *tempPtr= input;
        //advance 7 in input, length of 'PERIOD=' IS 7
        tempPtr+=7;
        //if of the period is not zero, store it 
        if(*tempPtr!=0){
            int per = atoi(tempPtr);
            //keep reading the period as long as it is not zero
            while(*tempPtr != 0) {
                //if didn't encounter a number then return
                if (!isdigit(*tempPtr)) {
                    return;
                }
                tempPtr++;
            }
            //store the period
            period = per;
        }
    } 
    report(input, 0);
    if(strcmp(input, "STOP") == 0) {
        logFlag = 0;
    }
    else if(strcmp(input, "START") == 0) {
        logFlag = 1;
    }
    else if(strcmp(input, "OFF") == 0) {
        shutdown();
    }
    else if(strcmp(input, "SCALE=F") == 0) {
        scale = 'F';
    }
    else if(strcmp(input, "SCALE=C") == 0) {
        scale = 'C';
    }
    
}
//on shutdown
void shutdown(){
    //get the current time. to print the shutdown time
    currentTime = localtime(&timer.tv_sec);
    char shutdownOutput[200];
    //to pass to report function and there do printf
    sprintf(shutdownOutput, "%02d:%02d:%02d SHUTDOWN", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
    report(shutdownOutput, 1);
    exit(0);
}
int main(int argc, char** argv){
    //options
    struct option opts[] = {
        {"scale", required_argument, NULL, 's'},
        {"period", required_argument, NULL , 'p'},
        {"log", required_argument, NULL, 'l'},
        {0,0,0,0}
    };
    int c;
    //read the command
    while((c = getopt_long(argc, argv, "", opts, NULL)) != -1){
        switch(c){
            //scale
            case 's':
                //only F and C are acceptable
                if(optarg[0] == 'F' || optarg[0] == 'C'){
                    scale = optarg[0];
                    break;
                }
            //period
            case 'p':
                period= atoi(optarg);
                break;
            //log
            case 'l':
                //open the logfile, give writing permision
                fileDescriptorOutput = fopen(optarg, "w+");
                if(fileDescriptorOutput == NULL){
                    fprintf(stderr, "Failed opening the logfile\n");
                    exit(1);
                }
                break;
            default:
                fprintf(stderr,"Invalid argument!\n");
                exit(1);
        }
    }
    //initialize temprature,1, and button,60
    temperature = mraa_aio_init(TEMPPIN);
    if(temperature == NULL){
        fprintf(stderr, "Initializing temp fialed %d\n" , TEMPPIN);
        exit(1);
    }
    button = mraa_gpio_init(BUTTONPIN);
    if(button == NULL){
        fprintf(stderr, "Initializing button failed %d\n" , BUTTONPIN);
        exit(1);
    }
    //input for button
    mraa_gpio_dir(button, MRAA_GPIO_IN); 
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &shutdown, NULL);
    //struct for poll
    struct pollfd poller;
    poller.fd = STDIN_FILENO;
    poller.events = POLLRDNORM; 
    //allocate buffer for input
    char* buff;
    buff = (char *)malloc(1024 * sizeof(char));
    if(buff == NULL){
        fprintf(stderr, "Allocating buffer failed\n");
        exit(1);
    }
    while(1){
        timeFunction();
        //do a poll http://man7.org/linux/man-pages/man2/poll.2.html
        //if poll was succefull 
        if(poll(&poller, 1, 0)){
            //process hthe input from stdin
            fgets(buff, 1024, stdin);
            readLine(buff);
        }
    }
    //close AIO/GPIO
    mraa_aio_close(temperature);
    mraa_gpio_close(button);
    exit(0);
}
