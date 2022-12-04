Program Name: String Parser [ex1]

Student Name: Muath Abu Jamal

==Program Description ==
This program receives a sentence from the user, and it parses/analysis it by counting the number of
it's words then it prints the number of the words and for each word it prints the word with its length beside.


== Important Variables ==
All these three variables are declared in the main function, which I use them as a global variables by passing 
their addresses to the other functions, these variables are:

1. char sentence []: a chars array to hold the user input(text/sentence)
2. int isInWord: this variable used as a flag to know if we are in a word or not (in the sentence)
3. sentenceLen: this variable holds the length of the sentence after removing the char '\n' if it exists by calling the function strLenNoNewLine()


== functions ==
1. int strLenNoNewLine(char*) - This function receives a pointer for the sentence that the user entered,
   	it removes the '\n' from the end of the sentence if it exists then it returns the length of the sentence.

2. int wordsCount(const char*, int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the bool isInWord (Used As Global Variable), 
	c- for the sentenceLen (Used As Global Variable)

	the function goes through the hole sentence and it counts the number of words then it returns that number.

3. void printWordsList(const char*, int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the bool isInWord (Used As Global Variable), 
	c- for the sentenceLen (Used As Global Variable).

	the function finds the start and end indexes for each word in the sentence, and it sends 
    	them each time to the function printWord() that prints each word and its length beside.

4. void printWord(const char*, const int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the first index in the word (according to the indexes of the sentence), 
	c- for the last index in the word (according to the indexes of the sentence).

	the function prints the word from the given wordStartInd to wordEndInd after copying it to a new 
	allocated memory then it frees the allocated memory.


== Program Files ==
1. ex1.c


== How To Compile? ==
compile: gcc ex1.c -o ex1
run: ./ex1


== Input ==
This program receives input from the user, which is a Text, for example:
"Muath Abu Jamal Is The Best Programmer"

For exit/ending the program the user must enter the word: "exit" (Only).


== Output ==
For example the program’s output of the above sentence is:
7 Words
Muath: 5
Abu: 3
Jamal: 5
Is: 2
The: 3
Best: 4
Programmer: 10
