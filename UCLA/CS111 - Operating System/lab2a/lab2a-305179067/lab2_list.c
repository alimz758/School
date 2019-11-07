#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "SortedList.h"
// takes a parameter for the number of parallel threads (--threads=#, default 1).
// takes a parameter for the number of iterations (--iterations=#, default 1).
// takes a parameter to enable (any combination of) optional critical section yields (--yield=[idl], i for insert, d for delete, l for lookups).
// initializes an empty list.
// creates and initializes (with random keys) the required number (threads x iterations) of list elements. Note that we do this before creating the threads so that this time is not included in our start-to-finish measurement. Similarly, if you free elements at the end of the test, do this after collecting the test execution times.
// notes the (high resolution) starting time for the run (using clock_gettime(3)).
// starts the specified number of threads.
// each thread:
// starts with a set of pre-allocated and initialized elements (--iterations=#)
// inserts them all into a (single shared-by-all-threads) list
// gets the list length
// looks up and deletes each of the keys it had previously inserted
// exits to re-join the parent thread
// waits for all threads to complete, and notes the (high resolution) ending time for the run.
// checks the length of the list to confirm that it is zero.
// prints to stdout a comma-separated-value (CSV) record including:
// the name of the test, which is of the form: list-yieldopts-syncopts: where
// yieldopts = {none, i,d,l,id,il,dl,idl}
// syncopts = {none,s,m}
// the number of threads (from --threads=)
// the number of iterations (from --iterations=)
// the number of lists (always 1 in this project)
// the total number of operations performed: threads x iterations x 3 (insert + lookup + delete)
// the total run time (in nanoseconds) for all threads
// the average time per operation (in nanoseconds).
// If bad arguments are encountered or a system call fails exit with a return code of one. 
//If the run completes successfully, exit with a return code of zero. If any inconsistencies are discovered, exit with 
//a return code of two.

int numberOfIterations=1;
int lock = 0;
char opt_sync;
pthread_mutex_t mutex;
SortedList_t *list;
SortedListElement_t *elems;
//do inset, delete, lookup for the list with the desired sync
void * threadFunction(void *arg){
  int* temp = (int *) arg;
  int i;
  for(i=(*temp *numberOfIterations);i<(*temp *numberOfIterations+numberOfIterations);i++){
    //if mutex is set
    if(opt_sync=='m'){
      pthread_mutex_lock(&mutex);
      SortedList_insert(list, (elems + i));
      pthread_mutex_unlock(&mutex);
    }
    //if sync
    else if(opt_sync=='s'){
      while(__sync_lock_test_and_set(&lock, 1));
      SortedList_insert(list, (elems + i));
      __sync_lock_release(&lock);
    }
    //default
    else{
      SortedList_insert(list, (elems+i)); 
    }
  }
  //mutex 
  if(opt_sync == 'm'){
    pthread_mutex_lock(&mutex);
    if(SortedList_length(list) < 0){
      fprintf(stderr, "failed to find the length\n");
      exit(2); 
    }
    pthread_mutex_unlock(&mutex);
  }
  //sync opt 
  else if(opt_sync == 's'){
    while(__sync_lock_test_and_set(&lock, 1) == 1);
    if(SortedList_length(list)< 0){
      fprintf(stderr, "failed to find the length\n");
      exit(2);
    }
    __sync_lock_release(&lock); 
  }
  //default 
  else{
    if(SortedList_length(list)< 0){
      fprintf(stderr, "failed to find the length\n");
      exit(2);
    }
  }
  //The lookup+delete
  SortedListElement_t *visited;
  int deleted;
  for(i=(*temp *numberOfIterations);i<(*temp *numberOfIterations+numberOfIterations);i++){
    // Mutex 
    if (opt_sync == 'm'){
      pthread_mutex_lock(&mutex);
      visited = SortedList_lookup(list, (elems + i)->key);
      pthread_mutex_unlock(&mutex);
      pthread_mutex_lock(&mutex);
      deleted = SortedList_delete(visited);
      pthread_mutex_unlock(&mutex);
    }
    else if (opt_sync == 's'){
      while (__sync_lock_test_and_set(&lock, 1));
      visited = SortedList_lookup(list, (elems + i)->key);
      __sync_lock_release(&lock);
      while (__sync_lock_test_and_set(&lock, 1));
      deleted = SortedList_delete(visited);
      __sync_lock_release(&lock);
    }
    else {
      visited = SortedList_lookup(list, (elems + i)->key);
      deleted = SortedList_delete(visited);
    }
    if(visited == NULL){
      fprintf(stderr, "Looking at the list failed\n");
      exit(2);
    }
    if(deleted){
      fprintf(stderr, "deleting failed\n");
      exit(2);
    }
  }
  return NULL;

}
int main(int argc, char* const argv[]){
  int numberOfThreads = 1;
  int opt_yield = 0;
  int opt_sync = 0; 
  int c = 0;
  int len,i;
  //options to parse 
  static struct option opts[] = {
    {"iterations", required_argument, NULL, 'i'},
    {"threads" , required_argument, NULL, 't'},
    {"yield" , required_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {0,0,0,0}
  };
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
      //see what is the option is passed for --yield
      len= strlen(optarg);
      for(i=0; i<len; i++){
	if(optarg[i]=='i'){
	  opt_yield = opt_yield | INSERT_YIELD;
	}
	else if(optarg[i]=='d'){
	  opt_yield = opt_yield | DELETE_YIELD ;
	}
	else if(optarg[i]=='l'){
	  opt_yield = opt_yield |   LOOKUP_YIELD;
	}
	else{
	  fprintf(stderr, "Invalid yield option\n");
	  break;
	}
      }
      break;
      //sync
    case 's':
      opt_sync = optarg[0];
      break;
    default:
      fprintf(stderr,"argument not found!\n");
      exit(1);
      break;
    }
  }
  //allocate  list
  list = malloc(sizeof(SortedList_t));
  //check if allocation was successful
  if(!list){
    fprintf(stderr, "Linked list memory allocation failed\n");
    exit(2);
  }
  //allocate for elements
  long numberOfElements = numberOfThreads * numberOfIterations;
  elems = malloc(sizeof(SortedListElement_t) * numberOfElements);
  //check if allocation failed for elems
  if(!elems){
    fprintf(stderr, "Elems memory allocation failed\n");
    exit(2);
  }
  long k;
  for (k=0; k< numberOfElements;k++){
    char *newKey = malloc(sizeof(char));
    if (newKey == NULL){
      fprintf(stderr, "creating keys failed.\n");
      exit(1);
    }
    newKey[0] = rand() % 256;
    newKey[1] = rand() % 256;
    newKey[2] = rand() % 256;
    newKey[3] = rand() % 256;
    newKey[4] = rand() % 256;
    newKey[5] = 0;
    (elems + k)->key = newKey;
  }
  //allocate threadnum and see if it failed or not
  int *threadNum = malloc(sizeof(int) * numberOfThreads);
  if (!threadNum){
    fprintf(stderr, "creating thread num failed.\n");
    exit(2);
  }
  int j;
  for (j = 0; j< numberOfThreads; j++)
    threadNum[j] = j;
  //start time
  if (clock_gettime(CLOCK_MONOTONIC, &startTime) == -1){
    fprintf(stderr, "start time failed.\n");
    exit(1);
  }
  //allocate threads array and see whether it fails or not
  pthread_t *threads = malloc(sizeof(pthread_t) * numberOfThreads);
  if (!threads){
    fprintf(stderr, "creating thread array failed.\n");
    exit(1);
  }
  //for mutex
  if (opt_sync == 'm'){
    if (pthread_mutex_init(&mutex, NULL) != 0){
      fprintf(stderr, "Creating mutex failed.\n");
      exit(1);
    }
  }
  //create threads
  for (i = 0; i < numberOfThreads; i++){
    int threadCreateStatus = pthread_create(&threads[i], NULL, &threadFunction, (void *)(threadNum + i));
    if (threadCreateStatus){
      fprintf(stderr, "could not create a thread.\n");
      exit(1);
    }
  }
  //
  for (i = 0; i < numberOfThreads; i++){
    int threadJoinStatus = pthread_join(threads[i], NULL);
    if (threadJoinStatus){
      fprintf(stderr, "thread join failed.\n");
      exit(1);
    }
  }
  //end time
  struct timespec end_time; 
  if (clock_gettime(CLOCK_MONOTONIC, &end_time) == -1){
    fprintf(stderr, "end time failed.\n");
    exit(1);
  }
  //get the length of the list
  long listLength = SortedList_length(list);
  if (listLength != 0){
    fprintf(stderr, "length of the list is not zero.\n");
    exit(1);
  }
  //print the list
  printf( "list");
  //yield options : yieldopts = {none, i,d,l,id,il,dl,idl}
  switch(opt_yield){
  case 0:
    printf( "-none");
    break;
  case 1:
    printf( "-i");
    break;
  case 2:
    printf( "-d");
    break;
  case 3:
    printf("-id");
    break;
  case 4:
    printf( "-l");
    break;
  case 5:
    printf( "-il");
    break;
  case 6:
    printf( "-dl");
    break;
  case 7:
    printf( "-idl");
    break;
  default:
    break;
  }
  //switch cases: syncopts = {none,s,m}
  switch(opt_sync){
  case 0:
    printf( "-none");
    break;
  case 's':
    printf( "-s");
    break;
  case 'm':
    printf( "-m");
    break;
  default:
    break;
  }
  //calculate the needed things to print
  long long totalNumberOfOperations = numberOfThreads * numberOfIterations * 3;
  long long runTime= (endTime.tv_sec = startTime.tv_sec);
  long long avgTime = runTime / totalNumberOfOperations;
  long long numberOfList = 1;
  printf( ",%d,%d,%lld,%lld,%lld,%lld\n", numberOfThreads, numberOfIterations, numberOfList, totalNumberOfOperations, runTime, avgTime);
  //free the allocated memory
  free(threads);
  free(list);
  free(elems);
  free(threadNum);
  exit(0);
}
