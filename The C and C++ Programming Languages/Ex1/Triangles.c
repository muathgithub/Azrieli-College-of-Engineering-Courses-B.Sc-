#include <stdio.h>

void clearBuffer(void);

int main(void) {

    //declaring the needed variables
    char firstChar, secondChar;
    int size;
    int i, j;

    //scanning the first char of the user input
    printf("Please enter the first character:\n");
    firstChar = getchar();

    //clearing the buffer from any garbage input or any special characters like '\n'(new Line)
    //By calling the method clearBuffer()
    //Explination for the private mehtod bellow
    clearBuffer();

    //scanning the second char of the user input
    printf("Please enter the second character:\n");
    secondChar = getchar();

    //clearing the buffer from any garbage input or any special characters like '\n'(new Line)
    //By calling the method clearBuffer()
    //Explination for the private mehtod bellow
    clearBuffer();

    //scanning the size from the user input
    printf("Please enter the edge size:\n");
    scanf("%d", &size);

    //printing the characters as the question need\asks
    //By using double for loops to make triangle
    for(i = 0; i < size; i++){
        for(j = 0; j < size + 1; j++){
            if(j < (size - i ))
                printf("%c", firstChar);
            else
                printf("%c", secondChar);
        }
        printf("\n");
    }


    return 0;
}

//This methods cleares the buffer from any garbage value or '\n'(newLine)
//And it stops after clearing the first '\n' it faces and thats what we need for
//reading/canning correct values from the user to get the job Done
void clearBuffer(void){
    while(getchar() != '\n');
}
