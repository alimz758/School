#define _GNU_SOURCE
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

int opt_sync;
int numberOfIterations = 1;
//for lab2b to keep track of number of lists
int numberOfLists = 0; 
int* lock = 0; 
pthread_mutex_t* mutex; 
SortedList_t *top;
SortedListElement_t *elems; 
//a simple hash function
unsigned int hashFunction(const char *s){
  int hash = 53;
  while(*s){
    hash = hash * 28 + *s;
    s++;
  }
  return hash;
}
//the thread function to controll the critical section
void* threadFunction(void *arg){
  //variables to be used later
  int* temp = (int *) arg; 
  int temp2 = *temp;
  long start = temp2 * numberOfIterations;
  long i;
  long end = start + numberOfIterations;
  long mutexTime = 0;
  struct timespec startingTime;
  struct timespec endingTime;
  for (i = start; i < end; i++){
    //mutex lock, using hash index corresponsing to its 'i'
    unsigned int hashIndex = hashFunction((elems+i)->key) % numberOfLists; 
    if(opt_sync == 'm'){
      clock_gettime(CLOCK_MONOTONIC, &startingTime); 
      pthread_mutex_lock(mutex + hashIndex); 
      clock_gettime(CLOCK_MONOTONIC, &endingTime);
      mutexTime += 1000000000L * (endingTime.tv_sec - startingTime.tv_sec) + endingTime.tv_nsec - startingTime.tv_nsec;
    }
    //sync set
    else if(opt_sync == 's'){
      while(__sync_lock_test_and_set(lock + hashIndex, 1));
    }
    //insert
    SortedList_insert(top + hashIndex, (elems + i));
    //mutex unlock
    if(opt_sync == 'm'){
      pthread_mutex_unlock(mutex + hashIndex);
    }
    //sync release
    else if(opt_sync == 's'){
      __sync_lock_release(lock + hashIndex);
    }
  }
  //locks for lists
  for(i = 0; i < numberOfLists; i++){
    //mutex lock
    if(opt_sync == 'm'){
      clock_gettime(CLOCK_MONOTONIC,&startingTime);
      pthread_mutex_lock(mutex + i);
      clock_gettime(CLOCK_MONOTONIC,&endingTime);
      mutexTime += 1000000000L * (endingTime.tv_sec - startingTime.tv_sec) + endingTime.tv_nsec - startingTime.tv_nsec;
    }
    //sync set
    else if(opt_sync == 's'){
      while(__sync_lock_test_and_set(lock + i, 1));
    }
  }
  long list_length = 0;  
  for(i = 0; i < numberOfLists; i++){
    list_length += SortedList_length(top + i);
  }
  for(i = 0; i < numberOfLists; i++){
    //mutex unlocking
    if(opt_sync == 'm'){
      pthread_mutex_unlock(mutex + i);
    }
    //sync release
    else if(opt_sync == 's'){
      __sync_lock_release(lock + i);
    }
  }
  char* checker = malloc(sizeof(char) * 6); 
  //lookup  & delete
  SortedListElement_t *visited;
  int deleted;
  for (i = temp2 * numberOfIterations; i < end; i++){
    unsigned int hashIndex = hashFunction((elems+i)->key) % numberOfLists;
    strcpy(checker, (elems+i)->key);
    // mutex lock
    if (opt_sync == 'm'){
      clock_gettime(CLOCK_MONOTONIC, &startingTime);
      pthread_mutex_lock(mutex + hashIndex);
      clock_gettime(CLOCK_MONOTONIC, &endingTime);
      mutexTime += 1000000000L * (endingTime.tv_sec - startingTime.tv_sec) + endingTime.tv_nsec - startingTime.tv_nsec;
    }
    //sync set
    else if (opt_sync == 's'){
      while (__sync_lock_test_and_set(lock + hashIndex, 1));
    }
    //lookep to store in visited
    visited = SortedList_lookup(top + hashIndex, checker);
    if(opt_sync == 'm'){
      pthread_mutex_unlock(mutex + hashIndex);
    }
    //sync release
    else if(opt_sync == 's'){
      __sync_lock_release(lock + hashIndex);
    }
    //if visited null, the stderr
    if(visited == NULL){
      fprintf(stderr, "Visiting node failed in the list\n");
      exit(2);
    }
    //again check for mutex
    if(opt_sync == 'm'){
      clock_gettime(CLOCK_MONOTONIC, &startingTime);
      pthread_mutex_lock(mutex + hashIndex);
      clock_gettime(CLOCK_MONOTONIC, &endingTime);
      mutexTime += 1000000000L * (endingTime.tv_sec - startingTime.tv_sec) + endingTime.tv_nsec - startingTime.tv_nsec;
    }
    //sync set
    else if(opt_sync == 's'){
      while(__sync_lock_test_and_set(lock + hashIndex, 1));
    }
    //store the deleted
    deleted = SortedList_delete(visited);
    //mutex unlock
    if(opt_sync == 'm'){
      pthread_mutex_unlock(mutex + hashIndex);
    }
    // sync release
    else if(opt_sync == 's'){
      __sync_lock_release(lock + hashIndex);
    }
    if(deleted){
      fprintf(stderr, "Deleting an item failed\n");
      exit(2);
    }
  }
  //return the pointer 
  return (void *) mutexTime;
}

int main (int argc, char** argv)
{
  int c = 0;    // Stores the options
  int numberOfThreads = 1;
  struct timespec start, end;
  numberOfIterations = 1;
  numberOfLists = 1;
  opt_sync = 0;
  opt_yield = 0;
  static struct option long_options[] = {
    {"iterations",  required_argument, NULL, 'i'},
    {"lists",       required_argument, NULL, 'l'},
    {"sync",        required_argument, NULL, 's'},
    {"threads",     required_argument, NULL, 't'},
    {"yield",       required_argument, NULL, 'y'},
    {0, 0, 0, 0}
  };
  //read the options
  while ((c = getopt_long(argc, argv, "", long_options, NULL)) != -1){
    switch (c){
      //iteration
    case 'i':
      numberOfIterations = atoi(optarg);
      break;
      //sync
    case 's':
      opt_sync = optarg[0];
      break;
      //thread
    case 't':
      numberOfThreads = atoi(optarg);
      break;
      //list
    case 'l':
      numberOfLists = atoi(optarg);
      break;
      //yield
    case 'y':{
      unsigned int flag;
      for (flag = 0; flag < strlen(optarg); flag++){
	if (optarg[flag] == 'i')
	  opt_yield = opt_yield | INSERT_YIELD;
	else if (optarg[flag] == 'd')
	  opt_yield = opt_yield | DELETE_YIELD;
	else if (optarg[flag] == 'l')
	  opt_yield = opt_yield | LOOKUP_YIELD;
      }
      break;
    }
    default:
      fprintf(stderr, "Invalid argument\n");
      exit(1);
      break;
    }
  }
  //allocate mem for the list 
  top = malloc(sizeof(SortedList_t) * numberOfLists);
  // If error in allocation
  if (!top){
    fprintf(stderr, "Allocating mem for list failed\n");
    exit(2);
  }
  //calculate number of elems
  long numberElems = numberOfThreads * numberOfIterations;
  //then allocate mem for it
  elems = malloc(sizeof(SortedListElement_t) * numberElems);
  // If error in allocation
  if (!elems){
    fprintf(stderr, "allocating mem failed for elems\n");
    exit(2);
  }
  long itr;
  char* newKey;
  for (itr = 0; itr < numberElems; itr++){
    //allocate mem for the new key
    newKey = malloc(sizeof(char) * 6);
    //if failed, report to stderr
    if (newKey == NULL){
      fprintf(stderr, "Error: unable to create keys.\n");
      exit(2);
    }
    newKey[0] = rand() % 254 + 1;
    newKey[1] = rand() % 254 + 1;
    newKey[2] = rand() % 254 + 1;
    newKey[3] = rand() % 254 + 1;
    newKey[4] = rand() % 254 + 1;
    newKey[5] = 0;
    // Copying the key to elem
    (elems + itr)->key = newKey;
  }
  //allocate mem for threadnum
  int* threadNum = malloc(sizeof(int) * numberOfThreads);
  if (!threadNum){
    fprintf(stderr, "allocatinf mem for threads failed\n");
    exit(2);
  }
  // Creating thread numbers to be passed as arguments in the thread function
  int i;
  for (i = 0; i < numberOfThreads; i++){
    threadNum[i] = i;
  }
  //allocate mem for threads
  pthread_t *threads = malloc(sizeof(pthread_t) * numberOfThreads);
  if (!threads){
    fprintf(stderr, "Creating threads failed\n");
    exit(2);
  }
  // mutex lock
  if (opt_sync == 'm'){
    //allocate mem for mutex
    mutex = malloc(sizeof(pthread_mutex_t) * numberOfLists);
    if (!mutex){
      fprintf(stderr, "allocating memory failed for mutex\n");
      exit(2);
    }
    long m = 0;
    for (m = 0; m < numberOfLists; m++){
      // check if mutex init failed
      if (pthread_mutex_init((mutex + m), NULL) != 0){
	fprintf(stderr, "mutex init failed\n");
	exit(1);
      }
    }
  }
  // spin lock
  else if (opt_sync == 's'){
    lock = malloc(sizeof(int) * numberOfLists);
    // If error in allocation
    if (!lock){
      fprintf(stderr, "allocating mem for spin locks failed\n");
      exit(2);
    }
    long t;
    //set all lock to zero
    for (t = 0; t < numberOfLists; t++)
      lock[t] = 0;
  }
  //clock start time
  clock_gettime(CLOCK_MONOTONIC, &start);
  // creating threaf
  for (i = 0; i < numberOfThreads; i++){
    int threadStatus = pthread_create(&threads[i], NULL, &threadFunction, (void *)(threadNum + i));
    if (threadStatus){
      fprintf(stderr, "Creating Threads failed.\n");
      exit(1);
    }
  }
  // mutex timing
  long total_mutex_wait_time = 0;
  void* mutex_wait_time;
  // joing threads
  for (i = 0; i < numberOfThreads; i++){
    int threadStatus = pthread_join(threads[i], &mutex_wait_time);
    // If error joining a thread
    if (threadStatus) {
      fprintf(stderr, "Joining threads failed\n");
      exit(1);
    }
    // Add to total mutex wait time
    total_mutex_wait_time += (long) mutex_wait_time;
  }
  //ending the timer
  clock_gettime(CLOCK_MONOTONIC, &end);
  long list_length = 0;
  // get the length of the list
  for (i = 0; i < numberOfLists; i++){
    list_length += SortedList_length(top + i);
  }
  //check if the length's list is not zero
  if (list_length != 0){
    fprintf(stderr, "List length is not zero, failed\n");
    exit(2);
  }
  printf( "list");
  //STDOUT CASES
  switch(opt_yield){
  case 0:
    printf("-none");
    break;
  case 1:
    printf( "-i");
    break;
  case 2:
    printf( "-d");
    break;
  case 3:
    printf( "-id");
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
  //sync options for stdout
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

  // Data used for CSV files
  long operations = numberOfThreads * numberOfIterations * 3;
  long run_time = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  long avg_time_ops = run_time / operations;
  long avg_mutex_wait = total_mutex_wait_time / operations;
  // Printing for mutex wait times
  if (opt_sync == 'm' || opt_sync == 's'){
    printf( ",%d,%d,%d,%ld,%ld,%ld,%ld\n", numberOfThreads, numberOfIterations, numberOfLists, operations, run_time, avg_time_ops, avg_mutex_wait);
  }
  // Printing for regular data
  else        {
    printf(",%d,%d,%d,%ld,%ld,%ld\n", numberOfThreads, numberOfIterations, numberOfLists, operations, run_time, avg_time_ops);
  }

  // free the allocated memory
  free(threads);
  free(top);
  free(elems);
  free(threadNum);
  exit(0);
}
