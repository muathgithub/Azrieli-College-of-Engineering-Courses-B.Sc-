// Program Name: Tweets Generator [Markov Chains] [NLP]
// Program Description: This program is a Tweets/Sentences generator that learns the order that the words come in, one after the other
// from a large tweets file (text file), and it starts to generate a new Tweets/Sentences according to Markov Chains method

// Student Name: Muath Abu Jamal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declaring preprocessor directives
#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define MIN_ARGS_COUNT 4
#define MAX_ARGS_COUNT 5
#define READ_ALL (-1)
#define TRUE 1
#define FALSE 0


// These are the given data structures from the question
// to build the markov chains
typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;


    //... Add your own fields here

    // counter for the number of words occurrences
    int wordOccurrencesNum;
    // a variable to save the length of the probability list
    int probArrayLen;

} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;

    //... Add your own fields here

    // counter for the number of the words occurrences after a previous word
    int probabilityOccurrencesNum;
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;


// declaring the needed functions (explanation for each function bellow(where each function written))
int strLenNoNewLine(char *);

int wordsCount(char *);

char *getWordStr(const char *, const int *, const int *);

char **toWordsArray(char *);

void freeAll(char **, int);

WordStruct *isInDictionary(LinkList *, char *);

WordStruct *buildWordStruct(char *);

WordProbability *isInProbabilityList(WordStruct *, WordStruct *);

void generateNSentences(LinkList *, int);


//////////////////////////////////////////////////  Required Functions  //////////////////////////////////////////////////

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add(LinkList *link_list, WordStruct *data) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return 1;
    }
    *new_node = (Node) {data, NULL};

    if (link_list->first == NULL) {
        link_list->first = new_node;
        link_list->last = new_node;
    } else {
        link_list->last->next = new_node;
        link_list->last = new_node;
    }

    link_list->size++;
    return 0;
}
/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
// This function receives one argument: 1- the max_number to generate a random number according to it.
int get_random_number(int max_number) {

    return rand() % max_number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */

// This function receives one argument: 1- a pointer to a LinkList which is the dictionary
// the function chose a random number from 0 --> dictionary->size and it returns the word struct
// of the word which is in the node at the random chosen number
// if the word ends with a dot '.' it redoes the process again
WordStruct *get_first_random_word(LinkList *dictionary) {

    Node *currNode;

    int randChoice;
    int i;

    do {

        randChoice = get_random_number(dictionary->size);

        for (i = 0, currNode = dictionary->first; (i < randChoice) && currNode != NULL; i++, currNode = currNode->next);

    } while (currNode->data->word[strLenNoNewLine(currNode->data->word) - 1] == '.');


    return currNode->data;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */

// This function receives one argument: 1- a pointer to a word struct
// the function chose a random word from the prob_list of the given word struct
// according to the probabilities of the words in the prob_list, and it returns a pointer to the word struct of that word
WordStruct *get_next_random_word(WordStruct *word_struct_ptr) {

    int randChoice = get_random_number(word_struct_ptr->wordOccurrencesNum);
    int occurrencesSum = 0;

    WordProbability *probability = word_struct_ptr->prob_list;

    int i;
    for (i = 0; i < word_struct_ptr->probArrayLen; i++) {

        occurrencesSum += probability[i].probabilityOccurrencesNum;

        if (randChoice < occurrencesSum) {

            return probability[i].word_struct_ptr;

        }

    }

    return NULL;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
// This function receives one argument: 1- a pointer tho the dictionary
// The function generates a sentences from the dictionary by the help of the asked function of the question
// and by the help of the private functions, the first word chosen randomly with equal probability for each word
// and the other words are chosen randomly according to their probabilities
// the function prints the sentence that it generates
int generate_sentence(LinkList *dictionary) {

    char generatedSentence[MAX_SENTENCE_LENGTH] = "";
    char currWord[MAX_WORD_LENGTH] = "";
    int wordsCounter = 0;

    WordStruct *currWordStruct = get_first_random_word(dictionary);

    strcpy(currWord, currWordStruct->word);
    strcat(generatedSentence, currWord);
    wordsCounter++;


    do {

        currWordStruct = get_next_random_word(currWordStruct);

        if (currWordStruct == NULL) {
            break;
        }

        strcpy(currWord, (char *) " ");
        strcat(currWord, currWordStruct->word);
        strcat(generatedSentence, currWord);

        wordsCounter++;

    } while (currWordStruct->prob_list != NULL && wordsCounter < MAX_WORDS_IN_SENTENCE_GENERATION);

    printf("%s", generatedSentence);

    return wordsCounter;

}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
// This function receives two arguments: 1- for the previous word struct, 2- for the current word struct
// the function creates and adds a new Word Probability struct to the prob_list of the previous word struct
// which pints to the current word struct, if there is a struct it increases the occurrences number of that current word struct by one
// at the end it 0 if the item is in the list otherwise it returns 1
int add_word_to_probability_list(WordStruct *first_word, WordStruct *second_word) {

    WordProbability *firstWordProbability;

    firstWordProbability = isInProbabilityList(first_word, second_word);

    if (firstWordProbability != NULL) {
        firstWordProbability->probabilityOccurrencesNum += 1;
        return 0;
    }

    first_word->probArrayLen += 1;

    first_word->prob_list = (WordProbability *) realloc(first_word->prob_list,
                                                        sizeof(WordProbability) * first_word->probArrayLen);

    // checking if the memory allocation failed
    // if true print a proper message and exit/end the program
    if (first_word->prob_list == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    first_word->prob_list[first_word->probArrayLen - 1].word_struct_ptr = second_word;


    first_word->prob_list[first_word->probArrayLen - 1].probabilityOccurrencesNum = 1;

    return 1;

}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
// This function receives three arguments: 1- a pointer for the file which we will read the tweets from, 2- the number of words to read, 3- a pointer to the dictionary
// the function starts reading sentence by sentence from the file using the private function that make all the needed string processing,
// and it fells the dictionary with these sentences, and it cares also to fill the prob_list for each word struct by the WordProbability structs for the next words
void fill_dictionary(FILE *fp, int words_to_read, LinkList *dictionary) {

    dictionary->first = NULL;
    dictionary->last = NULL;
    dictionary->size = 0;

    char currSentence[MAX_SENTENCE_LENGTH];
    char **wordsArray;
    int wordsArrayLen;
    int readWordsCounter = 0;
    int i;

    WordStruct *prevWordStruct;
    WordStruct *currWordStruct;


    while ((fgets(currSentence, MAX_SENTENCE_LENGTH, fp) != NULL) &&
           ((readWordsCounter < words_to_read) || (words_to_read == READ_ALL))) {

        prevWordStruct = NULL;
        currWordStruct = NULL;

        wordsArray = toWordsArray(currSentence);
        wordsArrayLen = wordsCount(currSentence);


        for (i = 0; (i < wordsArrayLen) && ((readWordsCounter < words_to_read) || (words_to_read == READ_ALL)); i++) {

            currWordStruct = isInDictionary(dictionary, wordsArray[i]);

            if (currWordStruct == NULL) {
                add(dictionary, buildWordStruct(wordsArray[i]));

                // I call this function again to get the currWordStruct pointer after I added it to the dictionary
                currWordStruct = isInDictionary(dictionary, wordsArray[i]);

            } else {
                currWordStruct->wordOccurrencesNum += 1;
            }

            if ((prevWordStruct != NULL) && (prevWordStruct->word[strLenNoNewLine(prevWordStruct->word) - 1] != '.')) {
                add_word_to_probability_list(prevWordStruct, currWordStruct);
            }

            prevWordStruct = currWordStruct;

            readWordsCounter++;
        }

        freeAll(wordsArray, wordsArrayLen);

    }

}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
// This function receives one argument: 1- a pointer for the dictionary
// the function frees all the manually allocated memories of the dictionary
void free_dictionary(LinkList *dictionary) {

    Node *prevNode = NULL;
    Node *currNode;

    for (currNode = dictionary->first; currNode != NULL; prevNode = currNode, currNode = currNode->next) {

        if (prevNode != NULL) {
            free(prevNode);
        }

        free(currNode->data->word);
        free(currNode->data->prob_list);
        free(currNode->data);
    }

    if (prevNode != NULL) {
        free(prevNode);
    }

    free(dictionary);
}

//////////////////////////////////////////////////  Main Function  //////////////////////////////////////////////////

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main(int argc, char *argv[]) {

    // this if checks, if the number of argument is valid
    // if False it prints a proper message, and it exit()
    if (argc < MIN_ARGS_COUNT || argc > MAX_ARGS_COUNT) {
        fprintf(stdout, "Usage: <Seed Num> <Sentences Num> <Text Croups Path> <Optional: Max Num Of Words To Read>\n");
        exit(EXIT_FAILURE);
    }

    // converting the intString to integer and initializing the seed with it
    srand((int) strtol(argv[1], (char **) NULL, 10));

    // converting the intString to integer and initializing the variable numOfSentencesToGenerate with
    int numOfSentencesToGenerate = (int) strtol(argv[2], (char **) NULL, 10);

    // opening the file of the given path in reading mode
    FILE *textCorpusFilePointer = fopen(argv[3], "r");
    // this if checks, if the file opened successfully
    if (textCorpusFilePointer == NULL) {
        fprintf(stdout, "Error: Failed To Open Text Croups File\n");
        exit(EXIT_FAILURE);
    }

    // initializing the variable maxWordsNumToREad with READ_ALL = -1 (it's the same as entering -1 as argument which is meaning to rad all the file)
    int maxWordsNumToRead = READ_ALL;
    // checking if the user entered a number of words to read
    // if true then modify the maxWordsNumToRead with that value
    if (argc == MAX_ARGS_COUNT) {
        maxWordsNumToRead = (int) strtol(argv[4], (char **) NULL, 10);
    }

    // manually allocating a link list for the dictionary
    LinkList *dictionary = (LinkList *) malloc(sizeof(LinkList) * 1);

    // checking if the memory allocation failed
    // if true print a proper message and exit/end the program
    if (dictionary == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    // calling the function fill_dictionary() to fill the dictionary with the words from the file
    fill_dictionary(textCorpusFilePointer, maxWordsNumToRead, dictionary);

    // generating the N sentences that the user asked for from the dictionary words
    generateNSentences(dictionary, numOfSentencesToGenerate);

    // freeing all the manually allocated memories of the dictionary by calling the function free_dictionary()
    free_dictionary(dictionary);

    // classing the file at the end of the program
    fclose(textCorpusFilePointer);

    return 0;
}

//////////////////////////////////////////////////  My/Private Functions  //////////////////////////////////////////////////

// This function receives two arguments: 1- a pointer for the dictionary, 2- a pointer to a word
// the function loops through the dictionary, and it checks if the word exists there
// if true it returns the Word Struct of that word
WordStruct *isInDictionary(LinkList *dictionary, char *word) {

    Node *currNode;

    for (currNode = dictionary->first; currNode != NULL; currNode = currNode->next) {

        if (strcmp(currNode->data->word, word) == 0) {

            return currNode->data;
        }
    }

    return NULL;
}

// This function receives one argument: 1- a pointer for a word
// the function builds a new word struct for the given word, and it returns that struct
WordStruct *buildWordStruct(char *word) {

    WordStruct *newWordStruct = (WordStruct *) malloc(sizeof(WordStruct) * 1);

    newWordStruct->word = (char *) malloc((sizeof(char) * strLenNoNewLine(word)) + 1);

    // checking if the memory allocation failed
    // if true print a proper message and exit/end the program
    if (newWordStruct->word == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(newWordStruct->word, word);

    newWordStruct->prob_list = NULL;

    newWordStruct->wordOccurrencesNum = 1;

    newWordStruct->probArrayLen = 0;

    return newWordStruct;

}

// This function receives two arguments: 1- a pointer for the previous word struct, 2- a pointer for the current word struct
// the function loops through the previous word struct prob_list, and it checks if there is a wordProbability struct that points to
// the current word struct if true it returns a pointer to that wordProbability struct
WordProbability *isInProbabilityList(WordStruct *prevWordStruct, WordStruct *currWordStruct) {

    WordProbability *currWordProbability = prevWordStruct->prob_list;

    int i;
    for (i = 0; i < prevWordStruct->probArrayLen; i++) {

        if (currWordProbability[i].word_struct_ptr == currWordStruct) {

            return (currWordProbability + i);

        }

    }

    return NULL;
}

// This function receives two arguments: 1- a pointer for the dictionary, 2- the number of sentences to generate
// the function calls the method generate_sentence() N time and each time it decorate the sentence as asked in the question
void generateNSentences(LinkList *dictionary, int N) {

    int i;
    for (i = 1; i <= N; i++) {

        printf("Tweet %d: ", i);
        generate_sentence(dictionary);
        printf("\n");
    }

}


//////////////////////////////////////////////////  String Processing  //////////////////////////////////////////////////


// This function receives four pointers: 1- for the sentence
// the function finds the start and end indexes for each word in the sentence, and it sends
// them each time to the function getWordStr() that allocates the needed memory for each argument/word,
// and it fills it in that memory then it returns a pointer for it
char **toWordsArray(char *sentence) {

    // this variable used as a flag to know if we are in a word or not (in the sentence)
    int isInWord = FALSE;

    // calculating the length of the sentence after removing the '\n' character if it exists
    // by calling the function strLenNoNewLine()
    int sentenceLen = strLenNoNewLine(sentence);

    int wordsArrayLen = wordsCount(sentence);

    // manually allocating memories for the args pointers
    char **wordsArray = (char **) malloc(sizeof(char *) * (wordsArrayLen));

    // checking if the memory allocation failed
    // if true print a proper message and exit/end the program
    if (wordsArray == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    int wordStartInd = 0, wordEndInd = 0;

    int i;
    // a pointer to the next pointer memory
    int wordsArrCurrIndex = 0;
    // this for loop goes through the sentence and for each word it saves the start and end character indexes
    // after reaching the end index for each word it calls the function getWordStr() that allocates the needed memory for each argument/word,
    // and it fills it in that memory then it returns a pointer for it which the wordsArray will hold in the proper index (wordsArrCurrIndex)
    for (i = 0; i < (sentenceLen); i++) {
        if (!(isInWord) && (sentence[i] != ' ')) {
            isInWord = TRUE;
            wordStartInd = i;
        } else if ((isInWord) && sentence[i] == ' ') {
            isInWord = FALSE;
            wordEndInd = i - 1;
            wordsArray[wordsArrCurrIndex] = getWordStr(sentence, &wordStartInd, &wordEndInd);
            wordsArrCurrIndex++;
        }
    }

    // checking the special case of finishing the for loop while we are still in a word that will not have a pointer
    // so here if we are still in word we make a pointer for it as before
    if (isInWord) {
        wordEndInd = i - 1;
        wordsArray[wordsArrCurrIndex] = getWordStr(sentence, &wordStartInd, &wordEndInd);
    }

    // returning the arguments array
    return wordsArray;
}

// This function receives three pointers: 1- for the sentence, 2- for the first index in the word (according to the indexes of the sentence), 3- for the last index in the word (according to the indexes of the sentence)
// the function manually allocates the needed memory for the word according to the given indexes, and it returns a pointer char* for it
char *getWordStr(const char *sentence, const int *wordStartInd, const int *wordEndInd) {
    int i, j;

    int wordLen = ((*(wordEndInd) - *(wordStartInd)) + 1);

    // allocating exactly needed memory for the characters of the word + 1 for the '\0' character
    char *currWordPointer = (char *) malloc(sizeof(char) * (wordLen + 1));

    // checking if the memory allocation goes wrong / failed
    // if true I print a message for the user then the program exits with EXIT_FAILURE code (1)
    if (currWordPointer == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        exit(EXIT_FAILURE);
    }


    // going through the sentence according to the indexes of the word for copying in to the new allocated memory
    for (i = *(wordStartInd), j = 0; i <= *(wordEndInd); i++, j++) {
        currWordPointer[j] = sentence[i];
    }

    // finishing the word by putting the '\0' character at the end of the word
    currWordPointer[wordLen] = '\0';

    return currWordPointer;
}

// This function receives three pointers: 1- for the sentence, 2- for the bool isInWord (Used As Global), 3- for the sentenceLen (Used As Global)
// the function goes through the sentence, and it counts the number of words in it using the variable isInWord
// which considered as a flag to know if we are in a word or not (in the sentence)
// at the end the function returns a value of type int which is the number of words in the sentence
int wordsCount(char *sentence) {
    int wordsCounter = 0;
    // this variable used as a flag to know if we are in a word or not (in the sentence)
    int isInWord = FALSE;

    // calculating the length of the sentence after removing the '\n' character if it exists
    // by calling the function strLenNoNewLine()
    int sentenceLen = strLenNoNewLine(sentence);

    int i;
    // this for that goes through the hole sentence and each time it finds character while
    // the variable isInWord is false it increases the wordsCounter by one
    for (i = 0; i < sentenceLen; i++) {
        if (!(isInWord) && (sentence[i] != ' ')) {
            isInWord = TRUE;
            wordsCounter = wordsCounter + 1;
        } else if ((isInWord) && sentence[i] == ' ') {
            isInWord = FALSE;
        }
    }

    return wordsCounter;
}

// This function receives a pointer for the sentence,
// and it calculates the length of the sentence by calling the function strlen()
// then it checks it the last character is '\n', if true it changes it to '\0'
// and it decreases the length by one then it returns the length
int strLenNoNewLine(char *sentence) {
    int sentenceLen = (int) strlen(sentence);

    if (sentenceLen != 0 && sentence[sentenceLen - 1] == '\n') {
        sentence[sentenceLen - 1] = '\0';
        sentenceLen = sentenceLen - 1;
    }

    return sentenceLen;
}

// This method receives two pointers: 1- for the arguments array, 2- for the number of the arguments
// it frees all the manually allocated memory by going through each pointer in the array of args pointers
// then it frees the array of pointers itself
void freeAll(char **wordsArrayPointer, int wordsArrayLen) {

    if (wordsArrayPointer == NULL) {
        return;
    }

    int i;
    for (i = 0; i < wordsArrayLen; i++) {
        free(wordsArrayPointer[i]);
    }

    free(wordsArrayPointer);
}