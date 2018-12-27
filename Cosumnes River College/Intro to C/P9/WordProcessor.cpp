#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

//print number of words, lines, and char
void documentReport(char  *input)
{
	
	int sub=0, count=0;
	int word_count=0,line_count=0,Char_count=0,index=0;
	
	//counting #occ. of white spaces
	for (int i=0; i <strlen(input); i++)
	{
		if(isspace(input[i]))
			count++;
	}
	
	int pos[count]; // to place the location where there is whitespace
	 
	while(input[index]!= '\0')
    {
	 	if(isspace(input[index])) // finding where there is whitespace
	 	{	
	 		if(sub<count)
	 		{	
	 			pos[sub++]=index;
	 	
	 		}
		}
	 		
	
	 	 //counting lines
		if(!strchr(input,'\n'))
		{
			line_count=1;
		}
		else if(input[index]=='\n')	
		{
			
			line_count++;
		}	
			
   
		Char_count++; 
		index++;
	   	
    }
    
    //COUNTING WORDS
    for(int j=0; j<count;j++)
    {
    	
    	if (((pos[j+1]-pos[j])!=1) || (isalnum(pos[sub]-1)))// if WHITESPACES ARE NOT NEXT TO EACH OTHER OR THERE IS NUMBER OR ALPHABETIC BEFORE EACH WHITESPACE
	 			word_count++;
	}
    
    
    cout << "Words     : " << word_count<< endl;
	cout << "Lines     : " << line_count <<  endl;
	cout << "Characters: " << Char_count << endl << endl;
	 
}

//return the updated sentence
char *replaceWord(const char *old_sentence, const char *oldString, const char *newString, int length)	
{
	char *new_sentence;
	int sub=0;
	
	// to allocate enough bytes of memory for the updated string
	
	new_sentence = (char *)new char[length];

	while (*old_sentence!='\0') 
	{
		if (strstr(old_sentence, oldString) == old_sentence) //if found the old string
		{
			strcpy(&new_sentence[sub], newString); // copy new string where found the old string
			sub += strlen(newString); //update sub based on length of newstring
			old_sentence += strlen(oldString); //  go to the point after oldstring in sentence
		}
		else
			new_sentence[sub++] = *old_sentence++; // if did not find old string, put char of old_sentence in new_sentence
	}

	new_sentence[sub] = '\0'; 
	return new_sentence;
}

//print the result of replaceWord
void printScreen( char *print, int length)
{
	
	for (int index=0; index<length;index++) // print char at a time
	{
		cout << *(print+index);
	}
	cout << endl;
}


int main()
{
	char document[] = "We\'re not laughing at you, we\'re laughing with you.",
		 oldString[] = "you",
		 newString[] = "them",
		 input[]= "A man\na plan\na canal\nPanama\n\n";
	char *print = NULL;
	int lengthOfNewSentence=0;
	
	//counting # of occ.
	int count=0;	
	for (int sub= 0; document[sub] != '\0'; sub++) 
	{
		if (strncmp(&document[sub], oldString, strlen(oldString)) == 0)
		{
			count++;
		}
	}
	
	lengthOfNewSentence=strlen(document)+ count * (strlen(newString)-strlen(oldString)); //length of the new sentence; used both in printScreen and memeory allocation in replaceWord
	
	documentReport(input); // document report function
	print = replaceWord(document, oldString, newString, lengthOfNewSentence); // get the update sentence
	printScreen(print, lengthOfNewSentence ); 

	delete [] print;
	print=NULL; //free the bytes of memory allocated
	return 0;
}

