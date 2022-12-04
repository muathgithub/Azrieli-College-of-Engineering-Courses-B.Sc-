// included the wordlist header and other needed headers
#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declaring a needed private function that creates a wordNode
WordNode* createWordNode(char*, WordNode*);

// this function creates a new wordList with a NULL head
WordList* createList(){

    WordList* wordListPointer = (WordList*) malloc(sizeof(WordList));

    if(wordListPointer == NULL){
        return NULL;
    }

    wordListPointer -> head = NULL;

    return wordListPointer;
}

// This function adds a word to a list in a sorted way
void addSorted(WordList* list, char* data){
    // This id checks if the list is empty ( the head is null ) if true it adds a new node to the begining of the list
    if(list -> head == NULL){
        list -> head = createWordNode(data, NULL);
        return;
    } 

    // declaring to pointers to wordNode for going thruogh the list
    WordNode* currWNP;
    WordNode* prevWNP = NULL;

    // going through the list from the start till the end or till the node that holds the same word or till the first bigger word
    for(currWNP = list -> head; currWNP != NULL && strcmp(data, currWNP -> wordData.data) > 0; prevWNP = currWNP, currWNP = currWNP -> next);

    //checking if the currWNP is not equals to NULL which means that the node holds the word or a the bigger first word
    if(currWNP != NULL){

        //chrcking if the word are equal if true we just increasing the occurrences by one
        if(strcmp(currWNP -> wordData.data, data) == 0){
            currWNP -> wordData.occurrences = (currWNP -> wordData.occurrences) + 1;
        } 
        
        // if the prevWNP == NULL that means that the new word has to be the head of the list
        else if(prevWNP == NULL) {
            list -> head = createWordNode(data, currWNP);
        }
        
        // else we add the node to the next of the prevWNP and make her next the currWNP
        else{
            prevWNP -> next = createWordNode(data, currWNP);
        }

    // else we add a new node to the end of the list
    } else {

        prevWNP -> next = createWordNode(data, NULL);
    }
}

// this function removes all the word that appers in the folder that his path is given 
void removeList(WordList* from, char* toRemove){
    
    // if the list is empty there is nothing to remove so the function returns
    if(from -> head == NULL){
        return;
    }

    // inisializing a file pointer to the fiven path in reading mode
    FILE* filePointer = fopen(toRemove, "r");

    // checking that the oppinging of the fille succeeded if not the function prints "IO Error" and the program ends
    if(filePointer == NULL){
        printf("IO Error");
        exit(EXIT_FAILURE);
    }

    // declaring to pointers to wordNode for going thruogh the list
    WordNode* currWNP;
    WordNode* prevWNP;

    // declaring an array to hold the words from the file to rmove them from the list
    char data[50];

    // while we didint reach the end of the file
    while(!feof(filePointer)){
        
        prevWNP = NULL;

        // reading a word from the file and saving it in data
        fscanf(filePointer, "%s", data);

        // searcing if the word is exists by going through the list
        for(currWNP = from -> head; currWNP != NULL && strcmp(data, currWNP -> wordData.data) > 0; prevWNP = currWNP, currWNP = currWNP -> next);

        // if the node that the for reached and stoped at is equals to the words that have to be deleted
        // we delete the word according to the possible statuses in linked list
        // and we free the word node that we clear and the word that it points to
        if((currWNP != NULL) && (strcmp(currWNP -> wordData.data, data) == 0)){

            if(prevWNP != NULL){
                prevWNP -> next = currWNP -> next;
            } else {
                from -> head = currWNP -> next;
            }

            free(currWNP -> wordData.data);
            currWNP -> next = NULL;

            free(currWNP);
        }  

    }

    fclose(filePointer);
}

// This function goes through all the nodes in the list and for each one it prints the word and the occurrences of it
void printList(WordList* list){

    if(list -> head == NULL){
        printf("** This List Is Empty **\n");
        return;
    }

    WordNode* currWNP;

    for(currWNP = list -> head; currWNP != NULL; currWNP = currWNP -> next){
        printf("%s, %d\n", currWNP -> wordData.data, currWNP -> wordData.occurrences);
    }
}

// This function distroies the list which means that it deletes all the nodes and make all the needed free operation then it free the list itself 
void destroyList(WordList* list){

    if(list -> head == NULL){
        return;
    }

    // declaring to pointers to wordNode for going thruogh the list
    WordNode* currWNP = list -> head;
    WordNode* prevWNP;

    // This while goes through the lsit and deletes all the nodes (free)
    while(currWNP != NULL){
        
        prevWNP = currWNP;

        free(currWNP -> wordData.data);
        //free(&(currWNP -> wordData));
        currWNP = currWNP -> next;

        prevWNP -> next = NULL;
        free(prevWNP);
    }

    list -> head = NULL;
    // free the list it self
    free(list);
    
}

// This function creates a new node by dynamiclly allocating the needed memory space 
// and it puts the given string char data in the word with occurrences equals to one
WordNode* createWordNode(char* data, WordNode* nextWNP){
    WordNode* wordNodePointer = (WordNode*) malloc(sizeof(WordNode));

    wordNodePointer -> wordData = *((Word*) malloc(sizeof(Word)));

    wordNodePointer -> wordData.data  = (char*) malloc(sizeof(char) * (strlen(data) + 1));
    strcpy(wordNodePointer -> wordData.data, data);

    wordNodePointer -> wordData.occurrences = 1;

    wordNodePointer -> next = nextWNP;

    return wordNodePointer;
}

