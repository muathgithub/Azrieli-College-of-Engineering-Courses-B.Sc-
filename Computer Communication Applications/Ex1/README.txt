== Student Details ==

Student Name: Muath Abu Jamal

== Program Name & Discription ==

Program Name: Tweets Generator [Markov Chains] [NLP] [Ex1]

Program Description: This program is a Tweets/Sentences generator that learns the order that the words come in, one after the other
from a large tweets file (text file), and it starts to generate a new Tweets/Sentences according to Markov Chains method.


== Program Files ==

1. tweetsGenerator.c - This file contains the hole program code which makes all the string processing, 
		       build the Markov chains and generates a new sentences.

2. README.txt - This file contains the student and program details.


== My/Private Functions ==

A-) I used some functions for string processing from a previous c code that we wrote in the Operating Systems Course,
these functions are:

1. char **toWordsArray(char *) - This function receives four pointers: 
	1- for the sentence.

	the function finds the start and end indexes for each word in the sentence, and it sends
	them each time to the function getWordStr() that allocates the needed memory for each argument/word,
	and it fills it in that memory then it returns a pointer for it.

2. char *getWordStr(const char*, const int*, const int*) - This function receives three pointers: 
	1- for the sentence, 
	2- for the first index in the word (according to the indexes of the sentence), 
	3- for the last index in the word (according to the indexes of the sentence).

	the function manually allocates the needed memory for the word according to the given indexes, 
	and it returns a pointer char* for it.

3. int wordsCount(char*) - This function receives three pointers: 
	1- for the sentence, 
	2- for the bool isInWord (Used As Global), 
	3- for the sentenceLen (Used As Global).

	the function goes through the sentence, and it counts the number of words in it using the variable isInWord
	which considered as a flag to know if we are in a word or not (in the sentence)
	at the end the function returns a value of type int which is the number of words in the sentence.

4. int strLenNoNewLine(char*) - This function receives one pointer:
	1- for the sentence.

	and it calculates the length of the sentence by calling the function strlen()
	then it checks it the last character is '\n', if true it changes it to '\0'
	and it decreases the length by one then it returns the length.

5. void freeAll(char **, int) - This method receives two pointers: 
	1- for the arguments array, 
	2- for the number of the arguments.

	it frees all the manually allocated memory by going through each pointer in the array of args pointers
	then it frees the array of pointers itself.


B-) I also write some private function for the purpose of making the code more modular and easy to modify,
these functions are:

1. WordStruct *isInDictionary(LinkList *dictionary, char *word) - This function receives two arguments: 
	1- a pointer for the dictionary, 
	2- a pointer to a word.

	the function loops through the dictionary, and it checks if the word exists there
	if true it returns the Word Struct of that word.

2. WordStruct *buildWordStruct(char*) - This function receives one argument: 
	1- a pointer for a word.

	the function builds a new word struct for the given word, and it returns that struct.

3. WordProbability *isInProbabilityList(WordStruct*, WordStruct*) - This function receives two arguments: 
	1- a pointer for the previous word struct, 
	2- a pointer for the current word struct.

	the function loops through the previous word struct prob_list, and it checks if there is a wordProbability struct that points to
	the current word struct if true it returns a pointer to that wordProbability struct.

4. void generateNSentences(LinkList *, int) - This function receives two arguments: 
	1- a pointer for the dictionary, 
	2- the number of sentences to generate.

	the function calls the method generate_sentence() N time and each time it decorate the sentence as asked in the question.


== How To Compile? ==

compile: gcc -Wall -Wextra -Wvla -std=c99 tweetsGenerator.c -o tweetsGenerator
run: ./tweetsGenerator


== How To Exit/End? ==

the program will end by itself after generating the asked nubmer sentences.


