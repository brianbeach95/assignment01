/*
Brian Beach
brian.beach10@okstate.edu
3/8/22
Assignment 01
*/

#include<stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>    

#define size 256

char line[size];


int arrayNum = 10;      //num to initialize array / max amount that can be
int numSearched = 0;    //int to count number of employees searched


//struct for employee
struct employees {
    char empID[16];
    char name[64];  //name of employee
    char title[64]; //title of employee
    int isEmp;      //bool for if is employee
};



//menu to use to ask user if they want to search and to get input details
void menu(char **name, char **title, int length) {

    char search[20];    //string to see if user wants to search for details

    char nameInput[64];
    char titleInput[64];

    printf("Do you want to search for employee details? Yes/no: ");
    scanf("%s",search);

    //while loop to run while the user wants to search for employees
    while(strcasecmp(search, "yes") == 0) {
        //if the number searched is longer than length, multiply length by 2 and realloc the two arrays
       /**
        if(numSearched == length) {
            length *= 2;
            name = realloc(name, length );
            title = realloc(title, length );
        }
        */

        printf("\n");
        printf("Enter employee name: ");
        
        scanf(" %149[^\n]", nameInput);  
        name[numSearched] = malloc(64 * sizeof(char));
        strcpy(name[numSearched], nameInput);
        printf("%s\n", name[numSearched]);


        printf("Enter title: ");
        scanf(" %149[^\n]", titleInput);
        title[numSearched] = malloc(64 * sizeof(char));
        strcpy(title[numSearched], titleInput);   //user input to title
        printf("%s\n", title[numSearched]);

        numSearched++;  //add 1 to num searched to keep track of position in array

        printf("\nDo you want to search for another employees details? Yes/no\t");
        scanf("%s",search);
    }
}


//main thread/method
int main() {
    
    int i;  //used in for loop
    int length = 2;

    char **nameArray= (char**) malloc(length * sizeof(char*));
    char **titleArray = (char**) malloc(length * sizeof(char*));
    
    menu(nameArray, titleArray, length);

    printf("test");

    struct employees empArray[numSearched];

    /*TEST --- for loop to test if menu is working properly
    for (i = 0; i < numSearched; i++) {
        printf("Name: %s\t Title: %s\n", empArray[i].name, empArray[i].title);  
    }
    */

    //employee information file
    FILE *info = fopen("employeeInformation.txt", "r");

    if (info != NULL) {
        char* empID;
        char* empName;
        char* empTitle;


        //for loop to go through each employee in array
        for(i = 0; i < numSearched; i++) {

            //while loop to search the file
            while(fgets(line, sizeof(line), info)){
                empID = malloc(64 * sizeof(char));
                empName = malloc(64 * sizeof(char));
                empTitle = malloc(64 * sizeof(char));    


                sscanf(line, "%149[^\t] %149[^\t] %149[^\t\n]", empID, empName, empTitle);

                if (strcasecmp(empName, nameArray[i]) == 0) {
                    //printf("3\n");
                    empArray[i].isEmp = 1;
                    strcpy(empArray[i].empID, empID);
                    strcpy(empArray[i].name, empName);
                    strcpy(empArray[i].title, empTitle);
                    
                    //empArray[i].empID = empID;  //set empID of employee to empID found in txt
                }

            }

            //frees up malloc given variables for next round
            free(empID);    
            free(empName);
            free(empTitle);

            //rewinds file to start search for next employee
            rewind(info);

        }

    }

    fclose(info);

    //
    //free(titleArray);


    //test to see if empID gets transferred
    for (i = 0; i < numSearched; i++) {
        if (empArray[i].isEmp == 1){
            printf("ID: %s\t Name: %s\t Title: %s\n", empArray[i].empID, empArray[i].name, empArray[i].title);  
        }
    }




    return 0;

}
