#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declaring preprocessor directives
#define TRUE 1
#define FALSE 0

// declaring the needed constants and initializing them with needed values
const int N = 100;
const int maxStrLen = 52;

// declaring the needed functions (explanation for each function bellow(where each function written))
void printMainMenu(void);
void addContact(char**, int*);
void clearBuffer(void);
int findContact(char**, int*);
void printContactEmails(char** , int, int);
void deleteEmail(char**, int*);
void backShift(char**, int*, int, int);
void setEmailsNum(char**, int, int);
void deleteContact(char**, int*);
void printAllContacts(char**, int*);

int main(void){

    // declaring and initializing the needed variables
    char* contacts[N];
    int currIndex = 0;
    int optionNum;

    // infinit loop ends when the user choses the number 6 from the main menu
    for(;;){

        // calling the method printMainMenu that prints the main options list for the user
        printMainMenu();
        // scanning the chosen option from the user
        scanf("%d", &optionNum);
        // calling clearBuffer function to get red from all not needed characters and i get red from '\n' that stuck in the buffer
        clearBuffer();

        // if optionNum == 6 that means (Exite) so i break the infinit for loop
        if(optionNum == 6){
            break;
        }

        //This switch is for the options from (1-5)
        switch(optionNum){
            case 1:
                // This option calls the method addContact() and it sends to it the address of the first pointer of the array contacts and the position for the next value/ where to add (currIndex)
                // That adds new contact to the array (contact name, emails number, emails list)
                addContact(contacts, &currIndex);
                break;

            case 2:
                // This option calls the method findContact() and it sends to it the address of the first pointer of the array contacts and the position for the next value/ where to add (currIndex)
                // That searches for a contact by name int the array contacts
                findContact(contacts, &currIndex);
                break;

            case 3:
                // This option calls the method deleteEmail() and it sends to it the address of the first pointer of the array contacts and the position for the next value/ where to add (currIndex)
                // That deletes contact from the array contacts by it's name if it exists
                deleteEmail(contacts, &currIndex);
                break;

            case 4:
                // This option calls the method deleteContact() and it sends to it the address of the first pointer of the array contacts and the position for the next value/ where to add (currIndex)
                // That deletes a contact email form the contact emails list by it's number in the shown list
                deleteContact(contacts, &currIndex);
                break;

            case 5:
                // This option calls the method printAllContacts() and it sends to it the address of the first pointer of the array contacts and the position for the next value/ where to add (currIndex)
                // That prints all the contacts in the array contacts
                printAllContacts(contacts, &currIndex);
                break;
            
        }

    }
    
}

// This method prints the main menu for the user
void printMainMenu(void){
    printf("Choose an option:\n1. Enter new contact.\n2. Search contact.\n3. Delete mail from contact.\n4. Delete Contact.\n5. Print.\n6. Exit.\nPlease Enter The Option Number:\n");
}


// This method adds a new contact for the array contacts by reciving the address for the first pointer in the array and the position for the next value/ where to add
void addContact(char** contactsArr, int* currIndex){

    // This if checks if the array is full and we cant add more contacts (contact name, emails number, contact emails)
    // and it prints a message for the user then it return
    if(*currIndex + 3 == N){
        printf("**The List Is Full You Can't Add More Contact**\n");
        return;
    }

    // declaring a temp array to scan the user inputs
    char strArr[maxStrLen];
    // emails counter in order to know how many pointers to book
    int emailsCounter = 0;

    // scanning the contact name from the user
    printf("Enter The Contact Name:\n");
    fgets(strArr, maxStrLen, stdin);
    strArr[strlen(strArr) - 1] = '\0';

    // allocating exactly the needed memory for the name according to the cahrs number
    contactsArr[*currIndex] = (char*) malloc(sizeof(char) * (strlen(strArr) + 1));

    // checking if the pointer is null which means that the allocation falied and there is no enough memeory
    // if true it prints a message end ends the program
    if(contactsArr[*currIndex] == NULL){
        printf("** Failed To Allocate Enough Memory **");
        exit(1);
    }

    // copying the name string that we scanned form the user to the string that we dynamically allocate
    strcpy(contactsArr[*currIndex], strArr);

    // increasing the currIndex value by one so we point to the next position where we can add the next data
    *currIndex = *currIndex + 1;

    // asking the user to enter the emails of the contact each one spreated whith new line and to enter the word "end" if he finishes
    printf("Please Enter The Contact Emails Each In New Line:\nAfter You Finish Enter The Word ( \"end\" ):\n");
    for(;;){
        //checking if there is no space for another email if true it prints a message and breaks the infinit loop
        if(*currIndex + emailsCounter >= N - 1){
            printf("**The List Is Full You Can't Add More Emails For This Contact**\n");
            break;
        }

        // scanning the emails from th euser
        fgets(strArr, maxStrLen, stdin);
        strArr[strlen(strArr) - 1] = '\0';

        // chekcing if the scanned string is the word "end"
        // if true that means that the user finished adds the emails so we break the infinit loop
        if(strcmp(strArr, "end") == 0){
            break;
        }

        // increasing the emails counter by one
        emailsCounter++;
        // allocating exactly the needed memory for each email according to the cahrs number for each email
        contactsArr[*currIndex + emailsCounter] = (char*) malloc(sizeof(char) * (strlen(strArr) + 1));

        // checking if the pointer is null which means that the allocation falied and there is no enough memeory
        // if true it prints a message end ends the program 
        if(contactsArr[*currIndex + emailsCounter] == NULL){
            printf("** Failed To Allocate Enough Memory **");
            exit(1);
        }

        // copying the name string that we scanned form the user to the string that we dynamically allocate
        strcpy(contactsArr[*currIndex + emailsCounter], strArr);
    }

    // This method saves the emails num in the proper index
    setEmailsNum(contactsArr, *currIndex, emailsCounter);

    // increasing the currIndex value by emails counter + one so we point to the next position where we can add the next data
    *currIndex += emailsCounter + 1;
    
}

// This method checks if the contact exists in the contacts array or not
int findContact(char** contactsArr, int* currIndex){

    // declaring a temp array to scan the user inputs
    char strArr[maxStrLen];
    int i;

    // scanning the contact name from the user to apply the search 
    printf("Enter The Contact Name:\n");
    fgets(strArr, maxStrLen, stdin);
    strArr[strlen(strArr) - 1] = '\0';

    // searching for the contact name in the array
    for(i = 0; i < *currIndex; i++){
        // if the contact found it prints ameesage that the contact is found and it prints it's emails list
        if(strcmp(strArr, contactsArr[i]) == 0){
            printf("The Contact Is ( Found ).\n");
            printContactEmails(contactsArr, i + 2, atoi(contactsArr[i + 1]));
            return i;
        }

        // increasing the i by the current contsct emails number + one then the for increses it by one, so we point to the next contact name
        i += atoi(contactsArr[i + 1]) + 1;
    }

    printf("The Contact Is ( Not Found ).\n");
    
    return -1;
}

// This method delets a mail from the chosen contact mails list
void deleteEmail(char** contactsArr, int* currIndex){
    // calling the method find contact to check if the pearson are exist or not
    int contactNameIndex = findContact(contactsArr, currIndex);

    // if the returned indes is (-1) that means the the contact are not found
    if(contactNameIndex == -1){
        return;
    }

    //checking if the contact has just one email that means that we can't delete it (contact has at least one email)
    //if true it prents a message end it returns
    if(atoi(contactsArr[contactNameIndex + 1]) == 1){
        printf("** ERROR: The Contact Has Only One Email [You Can't Delete It]**\n");
        return;
    }

    int emailNum;

    // scannign the chosen email nubmer form the user
    printf("Enter The Number Of The Email That You Want To Delete:\n");
    scanf("%d", &emailNum);
    // calling clearBuffer method to get red from all not needed characters and i get red from '\n' that stuck in the buffer
    clearBuffer();

    //cheking if the chosen email number is in range
    // if true we apply the deleting proccess 
    if(emailNum > 0 && emailNum <= atoi(contactsArr[contactNameIndex + 1])){
        // calculating the email addres to delete
        int emailIndex = contactNameIndex + emailNum + 1;
        // free that address (delete)
        free(contactsArr[emailIndex]);
        // making the pointer == NULL
        contactsArr[emailIndex] = NULL;
        // making one back shift for the data in the array
        backShift(contactsArr, currIndex, emailIndex, 1);
        
        // calculating the new emails num for the user
        int newEmailsNum = atoi(contactsArr[contactNameIndex + 1]) - 1;
        // free the emails num string
        free(contactsArr[contactNameIndex + 1]);

        // stting the new user's emails num 
        setEmailsNum(contactsArr, contactNameIndex + 1, newEmailsNum);

        // printning a mesage that the delete proccess done successfuly
        printf("<- The Email Have Been Deleted Successfully ->\n");
        return;
    }

    // if the email number are not in the valid range it print a message that the number is invalid and it returns
    printf("** Invalid Email Number **\n");

}

// This method delets a contact by reciving it's name from the user if it's exists (it delets the all the contact data[ name, emailsNum, emailsList])
void deleteContact(char** contactsArr, int* currIndex){
    // calling the method find contact to check if the pearson are exist or not
    int contactNameIndex = findContact(contactsArr, currIndex);

    // if the returned indes is (-1) that means the the contact are not found
    if(contactNameIndex == -1){
        return;
    }

    // calculating the contact data quantity (name + emailsNumString + emails Number)
    int dataQuantity = 2 + atoi(contactsArr[contactNameIndex + 1]);
    int i;

    // free all the strings for the user
    for(i = 0; i < dataQuantity; i++){
        free(contactsArr[contactNameIndex + i]);
    }

    // applying back shift for dataQuantity that we delete
    backShift(contactsArr, currIndex, contactNameIndex, dataQuantity);

    // printing  a message that the deleteing proccess done successfully
    printf("<- The Contact Have Been Deleted Successfully ->\n");

}

// This method prints all the contacts in the array contacts [(names, emails list) for each contact]
void printAllContacts(char** contactsArr, int* currIndex){

    int emailsNumber;
    int i;

    printf("The Contacts List Is:\n");
    printf("--------------------------\n");

    // printing a message if the array is empty
    if(*currIndex == 0){
        printf("( NO CONTACTS )\n");
    }

    // going through each contact in the list and printing it's data
    // it's name and it'semails list(by calling the method printContactEmails())
    for(i = 0; i < *currIndex; i++){
        emailsNumber = atoi(contactsArr[i + 1]);
        printf("Contact Name: ( %s )\n", contactsArr[i]);
        printContactEmails(contactsArr, i + 2, atoi(contactsArr[i + 1]));

        // increasing the i by the current contsct emails number + one then the for increses it by one, so we point to the next contact name
        i += emailsNumber + 1;
    }
}

// This mehtod making a backshift in the array according to the given shiftsAmount
void backShift(char** contactsArr, int* currIndex, int emptyIndex, int shiftsAmount){

    int i;

    // making the shifts
    for(i = 0; i < *currIndex - shiftsAmount; i++){
        contactsArr[emptyIndex + i] = contactsArr[emptyIndex + i + shiftsAmount]; 
    }

    // increasing the curr index by shiftsAmount so next time when we add data we point to the correct position
    *currIndex = *currIndex - shiftsAmount;
}


// This mehtod setes the emails number for the user
void setEmailsNum(char** contactsArr, int index, int emailsNum){
    
    char numStr[3];
    // converting the emails num into a string using the method sprintf()
    sprintf(numStr, "%d", emailsNum);

    // allocating exactly the needed memory for the name according to the cahrs number 
    contactsArr[index] = (char*) malloc(sizeof(char) * (strlen(numStr) + 1));


    // checking if the pointer is null which means that the allocation falied and there is no enough memeory
    // if true it prints a message end ends the program 
    if(contactsArr[index] == NULL){
        printf("** Failed To Allocate Enough Memory **");
        exit(1);
    }
    
    // copying the emailsNum string that we calculated and converted to the string that we dynamically allocate
    strcpy(contactsArr[index], numStr);
}

// This method goes through all the contact emails and it prints them each by it's number
void printContactEmails(char** contactsArr, int index, int emailsNum){
    
    int i;

    printf("Contact Emails List:\n");

    for(i = 0; i < emailsNum; i++){
        printf("%d. %s\n", i + 1, contactsArr[index + i]);
    }

    printf("--------------------------\n");
}

// This function clearBuffer(void) keeps clearing/redaing from buffer till it's reads/clears a newLine charachter ('\n')
// So the next methods that wants to recive/ scan/ get inputs from the user like (fgets(), scanf()) didit't make an unexpexted/ weird behavior
void clearBuffer(void){
    while(getchar() != '\n');
}