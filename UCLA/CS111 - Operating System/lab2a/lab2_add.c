#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>
// Write a test driver program (called lab2_add) that:

// takes a parameter for the number of parallel threads (--threads=#, default 1).
// takes a parameter for the number of iterations (--iterations=#, default 1).
// initializes a (long long) counter to zero
// notes the (high resolution) starting time for the run (using clock_gettime(3))
// starts the specified number of threads, each of which will use the above add function to
//      add 1 to the counter the specified number of times
//      add -1 to the counter the specified number of times
//      exit to re-join the parent thread
// wait for all threads to complete, and notes the (high resolution) ending time for the run
// prints to stdout a comma-separated-value (CSV) record including:
//      the name of the test (add-none for the most basic usage)
//      the number of threads (from --threads=)
//      the number of iterations (from --iterations=)
//      the total number of operations performed (threads x iterations x 2, the "x 2" factor because you add 1 first and then add -1)
//      the total run time (in nanoseconds)
//      the average time per operation (in nanoseconds).
//      the total at the end of the run (0 if there were no conflicting updates)
// If bad command-line parameters are encountered or a system call fails, exit with a return code of one. 
//If the run completes successfully, exit with a return code of zero. If any errors (other than a non-zero final count) 
//are encountered, exit with a return code of two.
int opt_yield;
int numberOfIterations=1;
static int spinLock = 0;
static pthread_mutex_t lock;
static long long counter = 0;
static char opt_sync = 'n';//for none
//to the add
void add(long long *pointer, long long num){
  long long sum = *pointer + num;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
}
//helper for sync cases
void syncHelper(int num){
  int i;
  for(i=0; i<numberOfIterations;i++){
    switch(opt_sync){
      //normal
    case 'n':
      add(&counter,num);
      break;
      //mutex
    case 'm':
      //lock the thread
      pthread_mutex_lock(&lock);
      add(&counter, num);
      pthread_mutex_unlock(&lock);
      break;
      //spin_lock
    case 's':
      //a spinlock is a lock which causes a thread trying to acquire it to simply wait in 
      //a loop ("spin") while repeatedly checking if the lock is available.; busy waiting
      while(__sync_lock_test_and_set(&spinLock, 1)){
	continue;
      }
      add(&counter, num);
      __sync_lock_release(&spinLock);
      break;
      //compare & swap
    case 'c':{
      long long first;
      long long second;
      while(1){
	//counter: The pointer to a variable whose value is to be compared with
	//first: The value to be compared with the value of the variable that counter points to
	//second: The value to be stored in the address that counter points to
	first = counter;
	second = first + num;
	//The function returns the initial value of the variable that __p points to; first
	if(__sync_val_compare_and_swap(&counter, first, second) == first){
	  break;
	}
      }
      break;
    }
    default:
      break;
    }
  }
}
//the thread caller
void * threadFuntion(){
  syncHelper(1);
  syncHelper(-1);
  return NULL;
}
int main(int argc, char* const argv[]){
  //options to parse 
  static struct option opts[] = {
    {"iterations", required_argument, NULL, 'i'},
    {"threads" , required_argument, NULL, 't'},
    {"yield" , no_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {0,0,0,0}
  };
  //options for threads, iterations, and the c is for options check
  int c;
  int numberOfThreads = 1;
  //timing variables 
  struct timespec startTime;
  struct timespec endTime;
  //loop for getopt()
  while((c = getopt_long(argc , argv, "", opts, 0)) != -1){
    switch(c){
      //iterations
    case 'i':
      numberOfIterations=atoi(optarg);
      break;
      //thread
    case 't':
      numberOfThreads=atoi(optarg);
      break;
      //yield
    case 'y':
      opt_yield = 1;
      break;
      //sync
    case 's':
      opt_sync = optarg[0];
      if(opt_sync != 'm' && opt_sync != 's' && opt_sync != 'c'){
	fprintf(stderr, "sync failed\n");
	exit(1);
      }
      if(opt_sync == 'm'){
	if(pthread_mutex_init(&lock, NULL)){
	  fprintf(stderr, "mutex failed\n");
	  exit(1);
	}
      }
      break;
    default:
      fprintf(stderr,"argument not found!\n");
      exit(1);
      break;
    }
  }
  //start timing for threads and processes
  clock_gettime(CLOCK_MONOTONIC, &startTime);
  //create array of threads' allocate with the number of Threads
  pthread_t* threads = (pthread_t* ) malloc(sizeof(pthread_t* ) * numberOfThreads);
  if(threads==NULL){
    fprintf(stderr,"allocating memory for threads failed\n");
    exit(2);
  }
  //creating threads
  int i;
  for(i=0; i<numberOfThreads;i++){
    if(pthread_create(&threads[i], NULL, threadFuntion,NULL)){
      fprintf(stderr,"creating threads failed\n");
      free(threads);
      exit(2);
    }
  }
  //join the threads
  int j;
  for(j=0; j<numberOfThreads;j++){
    if(pthread_join(threads[j], NULL)){
      fprintf(stderr,"Joining threads failed\n");
      exit(2);
    }
  }
  //ending time for threads
  clock_gettime(CLOCK_MONOTONIC, &endTime);
  //calculate the needed things for the csv file
  long long totalNumberOfOperations= numberOfIterations * numberOfThreads *2;
  long long runTime = (endTime.tv_sec = startTime.tv_sec);
  runTime+=endTime.tv_sec;
  runTime -=startTime.tv_sec;
  long long avgTime= runTime/totalNumberOfOperations;
  //print the desired things depnding on the followings
  if(opt_yield == 1 && opt_sync == 'm'){
    printf("%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-yield-m", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
  }
  else if(opt_yield == 1 && opt_sync == 's'){
    printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-yield-s", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    else if(opt_yield == 1 && opt_sync == 'c'){
        printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-yield-c", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    else if(opt_yield == 1 && opt_sync == 'n'){
        printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-yield-none", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    else if(opt_yield == 0 && opt_sync == 'm'){
        printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-m", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    else if(opt_yield == 0 && opt_sync == 'c'){
        printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-c", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    else if(opt_yield == 0 && opt_sync == 's'){
        printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-s", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    else{
        printf( "%s,%d,%d,%lld,%lld,%lld,%lld\n", "add-none", numberOfThreads, numberOfIterations, totalNumberOfOperations, runTime, avgTime, counter);
    }
    //free the allocated memory for threads then exit with 0
    free(threads);
    exit(0);
}
