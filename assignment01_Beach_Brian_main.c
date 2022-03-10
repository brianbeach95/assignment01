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
struct employees {
    char empID[16];
    char name[64];  //name of employee
    char title[64]; //title of employee
    int isEmp;      //bool for if is employee
};


//struct for threadA to return
typedef struct  {
    char basePay[32];
    char otPay[32];
    char otherPay[32];
    char benefits[32];
    char totalPay[32];
    char totalPayBenefits[32];
}structA;


//struct for threadB to return
typedef struct  {
    int y;
}structB;


//thread a function
void  *threadA(void * arg) {
    int i;

    char **input = (char**) arg;
    int num = sizeof(input) / sizeof(input[0]);
    
    structA* sA = (structA *) malloc(sizeof(structA));

     

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

                empID = malloc(32 * sizeof(char));
                bp = malloc(32 * sizeof(char));
                ot = malloc(32 * sizeof(char));
                other = malloc(32 * sizeof(char)); 
                ben = malloc(32 * sizeof(char));   
                tp = malloc(32 * sizeof(char)); 
                tpben = malloc(32 * sizeof(char)); 


                sscanf(line, "%149[^\t] %149[^\t] %149[^\t] %149[^\t] %149[^\t] %149[^\t] %149[^\t\n]", empID, bp, ot, other, ben, tp, tpben);

                if (strcasecmp(empID, input[i]) == 0) {
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

    int i;
    
    char **input = (char**) arg;
    int num = sizeof(input) / sizeof(input[0]);

    structB* sB = (structB *) malloc(sizeof(structB));

    for(i = 0; i < 5; ++i) {
        sB[i].y = i;
    }

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


    struct employees empArray[numSearched];


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
                    //id to pass through to threads/////////////////////
                    IDArray[idTracker] = malloc(64 * sizeof(char));
                    strcpy(IDArray[idTracker], empID);
                    ++idTracker;
                    ///////////////////////////////////////////////////
                    //struct stuff
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
   


    

    pthread_t tidA;
    pthread_t tidB;

    structA* a;
    structB* b;

    pthread_create(&tidA, NULL, threadA, (void *)IDArray);
    pthread_create(&tidB, NULL, threadB, (void *)IDArray);

    pthread_join(tidA, (void**) &a);
    pthread_join(tidB, (void**) &b);

    
    

    for(i = 0; i < idTracker; i++) {
        
        printf("%s\t %s\t %s\t %s\t %s\t %s\n", a[i].basePay, a[i].otPay, a[i].otherPay, a[i].benefits, a[i].totalPay, a[i].totalPayBenefits);
    }

    
    //printf("Result from A thread: %s\n", a[0].x);
    //printf("Result from B thread: %d", b[0].y);

    pthread_exit(0);
    return 0;

}
