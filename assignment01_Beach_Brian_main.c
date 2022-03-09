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
};



//menu to use to ask user if they want to search and to get input details
void menu(struct employees empArray[arrayNum]) {

    char search[20];    //string to see if user wants to search for details
    
    printf("Do you want to search for employee details? Yes/no: ");
    scanf("%s",search);

    //while loop to run while the user wants to search for employees
    while((strcasecmp(search, "yes") == 0) && (numSearched < arrayNum)) {
        printf("\n");
        printf("Enter employee name: ");
        
        scanf(" %149[^\n]", empArray[numSearched].name);    //user input to name
        printf("\n");
        printf("Enter title: ");
        
        scanf(" %149[^\n]", empArray[numSearched].title);   //user input to title

        numSearched++;  //add 1 to num searched to keep track of position in array

        printf("\nDo you want to search for another employees details? Yes/no\t");
        scanf("%s",search);
    }
}


//main thread/method
int main() {
    
    int i;  //used in for loop

    //make struct array of 10. Only allows 10 max searches in one go
    struct employees empArray[arrayNum];
    menu(empArray);

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

                if (strcasecmp(empName, empArray[i].name) == 0) {
                    //printf("3\n");
                    strcpy(empArray[i].empID, empID);
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


    //test to see if empID gets transferred
    for (i = 0; i < numSearched; i++) {
        printf("ID: %s\t Name: %s\t Title: %s\n", empArray[i].empID, empArray[i].name, empArray[i].title);  
    }




    return 0;

}