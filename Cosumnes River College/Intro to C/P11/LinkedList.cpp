
#include <iostream>


using namespace std;


struct ListNode {
  int data;
  struct ListNode *next;
};


ListNode *addToListEnd(ListNode * , int);
void      printList( ListNode *);
void      addAfterNode (ListNode *, int);
ListNode *copyList(ListNode *);
void 	 destroyList(ListNode *);
ListNode *deleteNode(ListNode *, ListNode *);

int main()
{
	
	ListNode *myList=NULL;
	ListNode *myListCopy=NULL;
	
	//myList= addToListEnd(NULL, 1);  // creates list with 1
	myList=addToListEnd(myList, 3); // appends 3 to list
	myList=addToListEnd(myList, 4); // appends 4 to list
	printList(myList);
	addAfterNode(myList, 2); // adds 2 after 1
	printList(myList);
	myListCopy = copyList(myList);
	printList(myListCopy);
	destroyList(myListCopy);
	myList = deleteNode(myList, myList->next->next); // deletes 3 from list
	printList(myList);
	destroyList(myList);
	
	return 0;
	
}

ListNode *addToListEnd( ListNode *list, int num)
{
	
	ListNode *nodePtr;
	ListNode *newNode;
	
	newNode= new ListNode;
	newNode-> data=num;
	newNode->next=NULL;
	
	if(!list)
		list=newNode;
		
	else	
	{
		nodePtr=list;
		
		while (nodePtr -> next)
			nodePtr=nodePtr ->next;
		
		nodePtr->next=newNode;		
	}
	return list;
	
}
void  printList( ListNode *list)
{
	while (list)
	{
		cout << list->data << " ";
		
		list=list->next;
	}
	cout << endl;
}

void addAfterNode ( ListNode *list, int num)
{
	ListNode *nodePtr;
	ListNode *newNode;
	ListNode *previousNode=NULL;
	
    newNode= new ListNode;
	newNode->data=num;
	
	if (!list)
	{
		list=newNode;
		newNode->next=NULL;
	
	}
	else
	{
		nodePtr=list;
		
		previousNode=NULL;
		
		while( nodePtr!=NULL && nodePtr->data <num)
		{
			previousNode=nodePtr;
			nodePtr=nodePtr->next;
			
		}
		
		if(previousNode==NULL)
		{
			list=newNode;
			newNode->next=nodePtr;
			
		}
		else 
		{
			previousNode->next=newNode;
			newNode->next=nodePtr;
		}
	}
}
ListNode *copyList (ListNode *list)
{
	ListNode *copy=NULL;
	ListNode *nodePtr=NULL;
	
	while(list !=NULL)
	{
		ListNode *temp= (ListNode*) new int [sizeof(ListNode)];
		
		temp->data=list->data;
		temp->next=NULL;
		
		if (copy==NULL)
		{
			copy=temp;
			nodePtr=temp;
		}
		else
		{
			nodePtr->next=temp;
			nodePtr=temp;
		}
		list=list->next;
		
	}
	return copy;
}

void destroyList(ListNode *list)
{
	ListNode *nodePtr;
	ListNode *nextNode;
	nodePtr=list;
	
	while( nodePtr!=NULL)
	{
		nextNode=nodePtr->next;
		
		delete nodePtr;
		
		nodePtr=nextNode;
	}
}
ListNode *deleteNode(ListNode *list, ListNode *toDelete)
{
	ListNode *nodePtr;
	ListNode *previousNode;
	
	if (list==toDelete)
	{
		nodePtr=list->next;
		delete list;
		list=nodePtr;
	}
	
	else 
	{
		nodePtr=list;
		
		while (nodePtr!=NULL && nodePtr !=toDelete)
		{
			previousNode=nodePtr;
			nodePtr=nodePtr->next;
		}
		
		if(nodePtr)
		{
			previousNode->next=nodePtr->next;
			delete nodePtr;
		}
	}
	return list;
	
}

