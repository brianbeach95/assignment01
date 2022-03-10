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


pthread_mutex_t thread_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_cond = PTHREAD_COND_INITIALIZER;
int thread_in = 0;


//struct for employee
typedef struct{
    char *empID;
    char *name;  //name of employee
    char *title; //title of employee
    int isEmp;      //bool for if is employee
    
}employees;


//struct for threadA to return
typedef struct  {
    char *basePay;
    char *otPay;
    char *otherPay;
    char *benefits;
    char *totalPay;
    char *totalPayBenefits;
}structA;


//struct for threadB to return
typedef struct  {
    char *year;
    char *agency;
    char *status;
}structB;


//thread a function
void  *threadA(void * arg) {
    int i;

    char **input = (char**) arg;

    int num = 0;
    while (input[num] != NULL) {
        ++num;
    }
    
   
    
    structA *sA = (structA *) malloc(num *sizeof(structA));

     //printf("A1");

    //employee information file
    FILE *salary = fopen("employeeSalary.txt", "r");

   

    if (salary != NULL) {
        char* empID;    //employee id
        char* bp;       //basepay
        char* ot;       //ot pay
        char* other;    //other
        char* ben;      //benfits
        char* tp;       //total pay
        char* tpben;    //total pay benefits


        //for loop to go through each employee in array
        for(i = 0; i < num; i++) {

            //while loop to search the file
            while(fgets(line, sizeof(line), salary)){
                //printf("A2");

                empID = malloc(32 * sizeof(char));
                bp = malloc(32 * sizeof(char));
                ot = malloc(32 * sizeof(char));
                other = malloc(32 * sizeof(char)); 
                ben = malloc(32 * sizeof(char));   
                tp = malloc(32 * sizeof(char)); 
                tpben = malloc(32 * sizeof(char)); 


                sscanf(line, "%149[^\t] %149[^\t] %149[^\t] %149[^\t] %149[^\t] %149[^\t] %149[^\t\n]", empID, bp, ot, other, ben, tp, tpben);

                if (strcasecmp(empID, input[i]) == 0) {
                    //printf("HELLO THERE");

                   sA[i].basePay = malloc(32 * sizeof(char));
                   sA[i].otPay = malloc(32 * sizeof(char));
                   sA[i].otherPay = malloc(32 * sizeof(char));
                   sA[i].benefits = malloc(32 * sizeof(char));
                   sA[i].totalPay = malloc(32 * sizeof(char));
                   sA[i].totalPayBenefits = malloc(32 * sizeof(char));



                    strcpy(sA[i].basePay, bp);
                    strcpy(sA[i].otPay, ot);
                    strcpy(sA[i].otherPay, other);
                    strcpy(sA[i].benefits, ben);
                    strcpy(sA[i].totalPay, tp);
                    strcpy(sA[i].totalPayBenefits, tpben);
                }

            }

            //frees up malloc given variables for next round
            free(empID);    
            free(bp);
            free(ot);
            free(other);    
            free(ben);
            free(tp);
            free(tpben);

            //rewinds file to start search for next employee
            rewind(salary);

        }

    }
    
    //printf("A4");
    fclose(salary);

    pthread_mutex_lock(&thread_lock);
    thread_in = 1;
    pthread_cond_signal(&thread_cond);
    pthread_mutex_unlock(&thread_lock);

    return (void*) sA;
    
}


//thread b function
void  *threadB(void * arg) {
    pthread_mutex_lock(&thread_lock);
    while(thread_in == 0) {
        pthread_cond_wait(&thread_cond, &thread_lock);
    }
    pthread_mutex_unlock(&thread_lock);

    //printf("B1");

    int i;
    
    char **input = (char**) arg;
    int num = 0;
    while (input[num] != NULL) {
        ++num;
    }
    

    structB* sB = (structB *) malloc(num *sizeof(structB));


    FILE *detail = fopen("employementDetail.txt", "r");

    if (detail != NULL) {
        
        char* empID;
        char* yr;
        char* agc;
        char* stat;

        
        //for loop to go through each employee in array
        for(i = 0; i < num; i++) {

            //while loop to search the file
            while(fgets(line, sizeof(line), detail)){

               // printf("B2");
                
                empID = malloc(32 * sizeof(char));
                yr = malloc(32 * sizeof(char));
                agc = malloc(32 * sizeof(char));
                stat =  malloc(32 * sizeof(char));   


                sscanf(line, "%149[^\t] %149[^\t] %149[^\t] %149[^\t\n]", empID, yr, agc, stat);

                if (strcasecmp(empID, input[i]) == 0) {

                   // printf("HELLO B2");
                   sB[i].year = malloc(32 * sizeof(char));
                   sB[i].agency = malloc(32 * sizeof(char));
                   sB[i].status = malloc(32 * sizeof(char));
                    
                   strcpy(sB[i].year, yr);
                    strcpy(sB[i].agency, agc);
                    strcpy(sB[i].status, stat);
                }

            }

         

            //frees up malloc given variables for next round
            free(empID);    
            free(yr);
            free(agc);
            free(stat);

            //rewinds file to start search for next employee
            rewind(detail);

        }

    }

   //printf("B3");
    fclose(detail);


    return (void*) sB;

    
}





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
    int length = 10;
    int idTracker = 0;

    char **nameArray= (char**) malloc(length * sizeof(char*));
    char **titleArray = (char**) malloc(length * sizeof(char*));

    char **IDArray = (char**) malloc(length * sizeof(char*));
    
    menu(nameArray, titleArray, length);

    //printf("AFTER MENU ");


    employees* empArray = (employees *) malloc(numSearched *sizeof(employees));
    //struct employees empArray[numSearched];
    //printf("AFTER EMPLOYEE STRUCT");


    //employee information file
    FILE *info = fopen("employeeInformation.txt", "r");

    if (info != NULL) {
        char* empID;
        char* empName;
        char* empTitle;

       //printf("M1");


        //for loop to go through each employee in array
        for(i = 0; i < numSearched; i++) {

            //while loop to search the file
            while(fgets(line, sizeof(line), info)){
                //printf("M2");

                empID = malloc(64 * sizeof(char));
                empName = malloc(64 * sizeof(char));
                empTitle = malloc(64 * sizeof(char));    


                sscanf(line, "%149[^\t] %149[^\t] %149[^\t\n]", empID, empName, empTitle);

                if (strcasecmp(empName, nameArray[i]) == 0) {
                    //printf("HELLO M3 ");
                    empArray[idTracker].isEmp = 1;
                    //id to pass through to threads/////////////////////
                    IDArray[idTracker] = malloc(64 * sizeof(char));
                    strcpy(IDArray[idTracker], empID);
                    
                    ///////////////////////////////////////////////////
                    //employee struct stuff
                    empArray[idTracker].empID= malloc(32 * sizeof(char));
                    empArray[idTracker].name = malloc(32 * sizeof(char));
                    empArray[idTracker].title = malloc(32 * sizeof(char));


                    strcpy(empArray[idTracker].empID, empID);
                    strcpy(empArray[idTracker].name, empName);
                    strcpy(empArray[idTracker].title, empTitle);
                    ++idTracker;
                    
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
   
   // printf("M4");

    pthread_t tidA;
    pthread_t tidB;

    structA* a;
    structB* b;




for(i = 0; i < idTracker; i++) {
    //printf("%s ", IDArray[i]);
}



    pthread_create(&tidA, NULL, threadA, (void *)IDArray);
    pthread_create(&tidB, NULL, threadB, (void *)IDArray);

    pthread_join(tidA, (void**) &a);
    pthread_join(tidB, (void**) &b);


    
    /*
    for(i = 0; i < idTracker; i++) {
        //printf("IN LOOP");
        printf("%s\t %s\t %s\n ", empArray[i].empID, empArray[i].name, empArray[i].title);
        printf("%s\t %s\t %s\t %s\t %s\t %s\n", a[i].basePay, a[i].otPay, a[i].otherPay, a[i].benefits, a[i].totalPay, a[i].totalPayBenefits);
        printf("%s\t %s\t %s\n", b[i].year, b[i].agency, b[i].status);
    }
    */

   
   
    
    

   /frees up allocated spaces for a and b structs
    free(a);
    free(b);
    free(empArray);
    

    //pthread_exit(0);
    return 0;

}
