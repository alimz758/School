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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <mraa.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
//vsriables for lab4c
SSL_CTX* sslContext = NULL;
SSL* sslInst = NULL;
struct hostent* serverNumber;
struct sockaddr_in serverAddress;
int portNumber = -1;
char* hostName = "";
char* idNumber = "";
int socketNumber;
//---------------------
float tempConverter();
void timeFunction();
void readLine(char * );
void report(char * , int );
void shutdown_mine();

#define TEMPPIN 1
// #define BUTTONPIN 60
//for temp and button
mraa_aio_context temperature;
// mraa_gpio_context button;
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
																					    //The difference in thi part is we are using SSL
																					    void report(char * toPrint, int toServer ){
																					        //if for server
																						    if(toServer==1){
																						            char output[200];
																							            sprintf(output,"%s\n", toPrint);
																								            //ssl writing
																									            SSL_write(sslInst, output, strlen(output));
																										        }
																											    fprintf(stderr, "%s\n", toPrint);
																											        fprintf(fileDescriptorOutput, "%s\n", toPrint);
																												    fflush(fileDescriptorOutput);
																												    }
																												    //read the line
																												    void readLine(char *input){
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
																																																																		            shutdown_mine();
																																																																			        }
																																																																				    else if(strcmp(input, "SCALE=F") == 0) {
																																																																				            scale = 'F';
																																																																					        }
																																																																						    else if(strcmp(input, "SCALE=C") == 0) {
																																																																						            scale = 'C';
																																																																							        }
																																																																								    
																																																																								    }
																																																																								    //The idea of doing this is not mine!
																																																																								    void sendToServer(char * input){
																																																																								        //read ssl
																																																																									    int sslResult = SSL_read(sslInst, input, 256);
																																																																									        if(sslResult > 0){
																																																																										        input[sslResult] = 0;
																																																																											    }
																																																																											        char* temp = input;
																																																																												    while (temp < &input[sslResult]) {
																																																																												            char* temp2 = temp;
																																																																													            while (temp2 < &input[sslResult] && *temp2 != '\n') {
																																																																														                temp2++;
																																																																																        }
																																																																																	        *temp2 = 0;
																																																																																		        readLine(temp);
																																																																																			        temp = &temp2[1];
																																																																																				    }
																																																																																				    }
																																																																																				    //on shutdown
																																																																																				    void shutdown_mine(){
																																																																																				        //get the current time. to print the shutdown time
																																																																																					    currentTime = localtime(&timer.tv_sec);
																																																																																					        char shutdownOutput[200];
																																																																																						    //to pass to report function and there do printf
																																																																																						        sprintf(shutdownOutput, "%02d:%02d:%02d SHUTDOWN", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
																																																																																							    report(shutdownOutput, 1);
																																																																																							        //shutdown  the ssl
																																																																																								    SSL_shutdown(sslInst);
																																																																																								        SSL_free(sslInst);
																																																																																									    exit(0);
																																																																																									    }
																																																																																									    int main(int argc, char** argv){
																																																																																									        //options
																																																																																										    struct option opts[] = {
																																																																																										            {"id", required_argument, NULL, 'i'},
																																																																																											            {"host", required_argument, NULL, 'h'},
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
																																																																																																																																										                //ID
																																																																																																																																												            case 'i':
																																																																																																																																													                    idNumber= optarg;
																																																																																																																																															                    break;
																																																																																																																																																	                //host
																																																																																																																																																			            case 'h':
																																																																																																																																																				                    hostName= optarg;
																																																																																																																																																						                    break;
																																																																																																																																																								                default:
																																																																																																																																																										                fprintf(stderr,"Invalid argument!\n");
																																																																																																																																																												                exit(1);
																																																																																																																																																														        }
																																																																																																																																																															    }
																																																																																																																																																															        //check if there is a hostname
																																																																																																																																																																    if(strlen(hostName) == 0){
																																																																																																																																																																            fprintf(stderr, "Something wrong with the hostname!!!! %s\n" , hostName);
																																																																																																																																																																	            exit(1);
																																																																																																																																																																		        }
																																																																																																																																																																			    //extracting the port number
																																																																																																																																																																			        if(optind < argc){
																																																																																																																																																																				        portNumber = atoi(argv[optind]);
																																																																																																																																																																					        if(portNumber <= 0){
																																																																																																																																																																						            fprintf(stderr, "failed opeing the port %x\n" , portNumber);
																																																																																																																																																																							                exit(1);
																																																																																																																																																																									        }
																																																																																																																																																																										    }
																																																																																																																																																																										        //log file is mandatory so!!!
																																																																																																																																																																											    if(fileDescriptorOutput == 0){
																																																																																																																																																																											            fprintf(stderr , "Forgot that log is mandatory!\n");
																																																																																																																																																																												            exit(1);
																																																																																																																																																																													        }
																																																																																																																																																																														    //checkfor the size of ID
																																																																																																																																																																														        if(strlen(idNumber) != 9){
																																																																																																																																																																															        fprintf(stderr, "Size of the ID is invalid  %x\n", strlen(idNumber));
																																																																																																																																																																																        exit(1);
																																																																																																																																																																																	    }
																																																																																																																																																																																	        //get the hostname
																																																																																																																																																																																		    if((serverNumber = gethostbyname(hostName)) == NULL){
																																																																																																																																																																																		            fprintf(stderr, "failed getting the hostname!! %s\n" , hostName);
																																																																																																																																																																																			            exit(1);
																																																																																																																																																																																				        }
																																																																																																																																																																																					    //try to to open socket
																																																																																																																																																																																					        if((socketNumber = socket(AF_INET, SOCK_STREAM, 0)) < 0){
																																																																																																																																																																																						        fprintf(stderr, "Unable to open the socket\n");
																																																																																																																																																																																							        exit(1);
																																																																																																																																																																																								    }
																																																																																																																																																																																								        memset((void*) &serverAddress, 0, sizeof(serverAddress));
																																																																																																																																																																																									    //setting type of internet address
																																																																																																																																																																																									        serverAddress.sin_family = AF_INET; 
																																																																																																																																																																																										    //getting the server IP
																																																																																																																																																																																										        memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)serverNumber->h_addr, serverNumber->h_length);
																																																																																																																																																																																											    //part of marshalling: transform endianness to readable by server format
																																																																																																																																																																																											        serverAddress.sin_port = htons(portNumber);
																																																																																																																																																																																												    //connect to the server
																																																																																																																																																																																												        if((connect(socketNumber, (struct sockaddr*) &serverAddress, sizeof(serverAddress))) < 0){
																																																																																																																																																																																													        fprintf(stderr, "failed making a connection with the server!!\n");
																																																																																																																																																																																														        exit(1);
																																																																																																																																																																																															    }
																																																																																																																																																																																															        //initialize SLL
																																																																																																																																																																																																    SSL_library_init();
																																																																																																																																																																																																        SSL_load_error_strings();
																																																																																																																																																																																																	    OpenSSL_add_all_algorithms();
																																																																																																																																																																																																	        sslContext = SSL_CTX_new(TLSv1_client_method());
																																																																																																																																																																																																		    //do some checks for SSL
																																																																																																																																																																																																		        if(sslContext == NULL){
																																																																																																																																																																																																			        fprintf(stderr , "failed setting ssl context\n");
																																																																																																																																																																																																				        exit(2);
																																																																																																																																																																																																					    }
																																																																																																																																																																																																					        sslInst = SSL_new(sslContext);
																																																																																																																																																																																																						    if(sslInst == NULL){
																																																																																																																																																																																																						            fprintf(stderr, "failed setting SSL\n");
																																																																																																																																																																																																							            exit(2);
																																																																																																																																																																																																								        }
																																																																																																																																																																																																									    if(!SSL_set_fd(sslInst, socketNumber)){
																																																																																																																																																																																																									            fprintf(stderr, "failed setting fd!!\n");
																																																																																																																																																																																																										            exit(2);
																																																																																																																																																																																																											        }
																																																																																																																																																																																																												    if(SSL_connect(sslInst) != 1){
																																																																																																																																																																																																												            fprintf(stderr, "failed TLS connection!!!\n");
																																																																																																																																																																																																													            exit(2);
																																																																																																																																																																																																														        }
																																																																																																																																																																																																															    //print the following over the server
																																																																																																																																																																																																															        char sendServer[50];
																																																																																																																																																																																																																    sprintf(sendServer, "ID=%s", idNumber);
																																																																																																																																																																																																																        report(sendServer, 1);
																																																																																																																																																																																																																	    //initialize temprature,1, and button,60
																																																																																																																																																																																																																	        temperature = mraa_aio_init(TEMPPIN);
																																																																																																																																																																																																																		    if(temperature == NULL){
																																																																																																																																																																																																																		            fprintf(stderr, "Initializing temp fialed %d\n" , TEMPPIN);
																																																																																																																																																																																																																			            exit(1);
																																																																																																																																																																																																																				        }
																																																																																																																																																																																																																					    //struct for poll
																																																																																																																																																																																																																					        struct pollfd poller;
																																																																																																																																																																																																																						    poller.fd = socketNumber;
																																																																																																																																																																																																																						        poller.events = POLLIN; 
																																																																																																																																																																																																																							    //allocate buffer for input
																																																																																																																																																																																																																							        char buff[256];
																																																																																																																																																																																																																								    while(1){
																																																																																																																																																																																																																								            timeFunction();
																																																																																																																																																																																																																									            //do a poll http://man7.org/linux/man-pages/man2/poll.2.html
																																																																																																																																																																																																																										            //if poll was succefull 
																																																																																																																																																																																																																											            poll(&poller, 1, 0);
																																																																																																																																																																																																																												            if(poller.revents & POLLIN){
																																																																																																																																																																																																																													                sendToServer(buff);
																																																																																																																																																																																																																															        }
																																																																																																																																																																																																																																    }
																																																																																																																																																																																																																																        //close AIO/GPIO
																																																																																																																																																																																																																																	    mraa_aio_close(temperature);
																																																																																																																																																																																																																																	        exit(0);
																																																																																																																																																																																																																																		}

