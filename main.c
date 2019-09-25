#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<dirent.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<limits.h>
#include<string.h>
#include<sys/wait.h>
#include<math.h>
#include<time.h>
#include<signal.h>

extern char **getln();
int executeCustomCommands(char **args);
int countArgs(char **args);
int findGCDOfTwoNumbers(int a, int b);
int findGCD( char **args);
void countAndDisplayArgs(char **args);
void openFileToRead(char **args);
int executeSystemCommands(char **args);
void openFileToWrite(char **args);
void dice();

int main(){
    
    char **args;
    struct passwd *password;
    char hostName[1000];
    gethostname(hostName, 1000);
    password = getpwuid(getuid());
    
    printf("[%s@%s]$: ", password->pw_name, hostName);   

    /*The method to implement this while loop was understood from the slides posted for the lab for week-3 */
    while(  strcmp(args[0], "exit") != 0  ){
    
        args=getln();

            if( ( strcmp(args[0], "exit") == 0 )){
            
            exit(1);
        }

            if( (strcmp(args[0], "gcd") == 0) || (strcmp(args[0], "args") == 0) || (strcmp(args[0], "dice") == 0) ){
                executeCustomCommands(args);
            }
            
            else{
  
            executeSystemCommands(args);
            }
             printf("[%s@%s]$ ", password->pw_name, hostName);   
            
        }
    
    return 0;
}

int executeCustomCommands(char **args){

    int f = 0;
    int argsNo = countArgs(args);
    FILE *fp;
    char *fileName = NULL;
    int fileOpen = 0;

    if( (argsNo >= 2) && (strcmp(args[argsNo - 2],"<") == 0) ){
                fileName = args[countArgs(args) -1];
                fp = freopen(fileName, "r+", stdin);
                fileOpen = 1;
               
            }
            else if( (argsNo >= 2) && (strcmp(args[argsNo - 1],">") == 0) ){
                
                fileName = args[countArgs(args) -1];
                fp = freopen(fileName, "w+", stdout);
                fileOpen = 1;
                
            }

    if(f == 0){
        if( strcmp(args[0], "gcd") == 0){
            
                if(argsNo == 3){
                    printf("Greatest Common Denominator is: %d\n", findGCDOfTwoNumbers(atoi(args[1]), atoi(args[2])) );
                }
                else{
                    fprintf(stderr, "Usage: gcd first_number second_number\n");
                }
               
            
        }
        else if( strcmp(args[0], "args") == 0){
            countAndDisplayArgs(args);
            
        }
        else if( strcmp(args[0], "dice") == 0){
            if( argsNo == 1){
                dice();
            }
            else{
                printf("Usage: dice\n");
            }
        }
        else if(strcmp(args[0], "exit") == 0){
            
        }
        
        if(fileOpen) fclose(fp);
        
    }
    
    return 1;
}

int executeSystemCommands(char **args){
/*This function was also implemented using the methods taught in lab-3*/
    int f = fork();
    
    if(f == 0){

        int argsNo = countArgs(args);
        FILE *fp;
        char *fileName;
        int fileOpen = 0;
        
        if( (argsNo >= 2) && (strcmp(args[argsNo - 2],"<") == 0) ){
                
                fileName = args[countArgs(args) -1];
                fp = freopen(fileName, "r+", stdin);
                args[argsNo - 2] = NULL;
                args[argsNo - 1] = NULL;
                
                fileOpen = 1;
               
            }
            else if( (argsNo >= 2) && (strcmp(args[argsNo - 2],">") == 0) ){
             
                fileName = args[countArgs(args) -1];
                fp = freopen(fileName, "w+", stdout);
                args[argsNo - 2] = NULL;
                args[argsNo - 1] = NULL;
                fileOpen = 1;
                
            }


        if(execvp(args[0], args) < 0){
            fprintf(stderr, "%s: Invalid command!\n", args[0]);
            exit(0);
        }
        if(fileOpen){
            fclose(fp);
        }
    }
    else{
        wait(NULL);
    }

        return 1;
}

int countArgs(char **args){
    int argsNo = 0;
    while(args[argsNo] != NULL){
        argsNo++;
    }
    return argsNo;
}

int findGCDOfTwoNumbers(int a, int b){
    if(a == b){
        return a;
    }

    int gcd = abs(a - b);

    while( (a%gcd != 0) || (b%gcd != 0) ){
        gcd--;
    }

    return gcd;

}

int findGCD(char **args){
    int num_1 = 0;
    int num_2 = 0;
    char *ptr;

    if( args[1][0] == '0' && ((args[1][1] == 'x') || (args[1][1] == 'X')) ){
        num_1 = (int) strtol(args[1], &ptr, 16);
    }
    else
    {
        num_1 = atoi(args[1]);
    }

    if( args[2][0] == '0' && ((args[2][1] == 'x') || (args[2][1] == 'X')) ){
        num_2= (int) strtol(args[2], &ptr, 16);
    }
    else
    {
        num_2 = atoi(args[2]);
    }
    
    return findGCDOfTwoNumbers(num_1, num_2);
}

void countAndDisplayArgs(char **args){
    int i = 1;
    printf("argc = %d, args = ", countArgs(args) - 1);
/*Implemented using the method taught in lab-3*/
    while(args[i] != NULL){
        printf("%s ", args[i]);
        i++;
    }
    printf("\n");
}


void dice(){//My own command to implement for the assignment

    srand(time(NULL));
    printf("Dice gives: %d\n", 1+ rand()%6);
}