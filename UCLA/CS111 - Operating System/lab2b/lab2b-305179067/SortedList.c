#include "SortedList.h"
#include <sched.h>
#include <stdlib.h>
#include <string.h>


/**
 * SortedList_insert ... insert an element into a sorted list
 *
 *The specified element will be inserted in to
 *the specified list, which will be kept sorted
 *in ascending order based on associated keys
 *
 * @param SortedList_t *list ... header for the list
 * @param SortedListElement_t *element ... element to be added to the list
 */

int opt_yield = 0;

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
  //first check if the header or the element is emptyis empty
  if(!list || !element){
    return;
  }
  //then check if there is any element in the list, if not element would become the first element
  if(!(list->next)){
    //check for opt_yield and insert to to use sched_yield
    if(opt_yield & INSERT_YIELD ){
      sched_yield();
    }
    //critical section
    list->next=element;
    element->prev=list;
    element->next=NULL;
    return;
  }
  //create temp list to iterate though the list and add element to its appropriate spot
  SortedListElement_t *temp = list->next;
  SortedListElement_t *p = list;
  while (temp && strcmp(element->key, temp->key) >= 0){
    p = temp;
    temp = temp->next;
  }
  if (opt_yield & INSERT_YIELD){
    sched_yield();
  }
  if (!temp){
    p->next = element;
    element->prev = p;
    element->next = NULL;
  }
  else{
    p->next = element;
    temp->prev = element;
    element->next = temp;
    element->prev = p;
  }
}

/**
 * SortedList_delete ... remove an element from a sorted list
 *
 *The specified element will be removed from whatever
 *list it is currently in.
 *
 *Before doing the deletion, we check to make sure that
 *next->prev and prev->next both point to this node
 *
 * @param SortedListElement_t *element ... element to be removed
 *
 * @return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 */
int SortedList_delete( SortedListElement_t *element){
  //if element doesn't exist
  if(!element)
    return 1;
  //if the flaged for sched are set then, do sched_yield();
  if (opt_yield & DELETE_YIELD)
    sched_yield();
  //if element->next exist
  if(element->next){
    //but next->prev is not element
    if((element->next)->prev!= element)
      return 1;
    else
      (element->next)->prev = element->prev;
  }
  //if element->prev exist
  if(element->prev){
    //but next->prev is not element
    if((element->prev)->next!= element)
      return 1;
    else
      (element->prev)->next = element->next;
  }
  return 0;
}

/**
 * SortedList_lookup ... search sorted list for a key
 *
 *The specified list will be searched for an
 *element with the specified key.
 *
 * @param SortedList_t *list ... header for the list
 * @param const char * key ... the desired key
 *
 * @return pointer to matching element, or NULL if none is found
 */
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key){
  //if list is empty return NULL
  if(!list)
    return NULL;
  //create a temp var to lookup
  SortedListElement_t *temp = list->next;
  while(temp){
    //if the flags are set for Lookup, do sched_yield
    if(opt_yield & LOOKUP_YIELD)
      sched_yield();
    //if found the jey in list, return the node
    if(strcmp(key,temp->key)==0)
      return temp;
    temp=temp->next;
  }
  //didn't find the key in list, so return 1
  return NULL;
}
/**
 * SortedList_length ... count elements in a sorted list
 *While enumeratign list, it checks all prev/next pointers
 *
 * @param SortedList_t *list ... header for the list
 *
 * @return int number of elements in list (excluding head)
 *   -1 if the list is corrupted
 */
int SortedList_length(SortedList_t *list){
  //if list is empty return NULL
  if(!list)
    return -1;
  //create a temp var to lookup
  int count=0;
  SortedListElement_t *temp = list->next;
  while(temp){
    //if the flags are set for Lookup, do sched_yield
    if(opt_yield & LOOKUP_YIELD)
      sched_yield();
    //check for last and first node
    if (temp->prev != NULL && (temp->prev)->next != temp)
      return -1;
    if (temp->next != NULL && (temp->next)->prev != temp)
      return -1;
    count++;
    temp=temp->next;
  }
  //return the counter
  return count;
}
