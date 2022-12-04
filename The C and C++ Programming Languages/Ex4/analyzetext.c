// included the wordlist header and other needed headers
#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>

// declaring the needed preproccesor 
#define MIN_OCCURENCES 2

// declarring the needed private methods
// This function goes through all the word in the given file path and it adds each one to a list
// in sorted way by callling the method addSorted
WordList* convertToWordsList(char*);
// This function removes all the words with occurence < MIN_OCCURENCES from the given list
void removeMinLimWords(WordList*);

int main(int argc, char* argv[]){

     printf("\n******************* ALL THE WORDS *******************\n");
    // creating a new list with the words of the file that given his path
    WordList* newWordList = convertToWordsList(argv[1]);

    printList(newWordList);

    printf("\n******************* NO WORDS UNDER MIN_OCCURENCES *******************\n");
    // removing the words with occurensse < MIN_OCCURENCES
    removeMinLimWords(newWordList);

    printList(newWordList);

    printf("\n******************* NO STOP WORDS *******************\n");
    // removing the stop words from the list (all the lists from the file that given his path)
    removeList(newWordList, argv[2]);
    
    printList(newWordList);

}


// This function recives a file path which we want to conbvert to words list
WordList* convertToWordsList(char* filePath){

    // inisializing a file pointer to the fiven path in reading mode
    FILE* filePointer = fopen(filePath, "r");
    // creating a new empty list 
    WordList* newWordList = createList();

    // checking if there is any error in opening the file or in creating the list 
    if((filePointer == NULL) || (newWordList == NULL)){
        printf("IO Error");
        exit(EXIT_FAILURE);
    }

    // declaring an array to hold the words from the file to rmove them from the list
    char data[50];

     // while we didint reach the end of the file
    while(!feof(filePointer)){
        // reading a word from the file and saving it in data
        fscanf(filePointer, "%s", data);

        // add the word to the list
        addSorted(newWordList, data);
    }

    // closing the file
    fclose(filePointer);
    return newWordList;
}


// This function removes all the words with occurence < MIN_OCCURENCES from the list
void removeMinLimWords(WordList* list){
    
    // declaring to pointers to wordNode for going thruogh the list
    WordNode* currWNP = list -> head;
    WordNode* prevWNP = NULL;

    // go through the list
    while(currWNP != NULL){
        
        // if the occurence is < MIN_OCCURENCES
        if(currWNP -> wordData.occurrences < MIN_OCCURENCES){

            // if the prevWNP != NULL which means that we have to connect the prev to the next of the currWNP
            if(prevWNP != NULL){
               
                prevWNP -> next = currWNP -> next;

                // free the word dynamicly allocated memory
                free(currWNP -> wordData.data);
                currWNP -> next = NULL;

                // free the wordNode dynamicly allocated memory
                free(currWNP);
                currWNP = prevWNP -> next;
            } else {
                // in this case the word to be removed is the head so (prevWNP == NULL)
                // the same free method as above 
                list -> head = currWNP -> next;
                
                free(currWNP -> wordData.data);
                currWNP -> next = NULL;
                
                free(currWNP);
                currWNP = list -> head;
            }

            continue;
        }

        // if the word occurence is >= MIN_OCCURENCES then just take a step forward
        prevWNP = currWNP;
        currWNP = currWNP -> next;  
          
    } 

}