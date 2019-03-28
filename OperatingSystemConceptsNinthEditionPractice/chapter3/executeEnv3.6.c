#include<sys/types.h>
#include <unistd.h>
#include<stdio.h>

int main() 
{
    pid_t pid;
    /*fork a child process*/
    pid = fork();

    if(pid < 0)
    {/*error occurred*/
        fprintf(stderr, "Fork Failed");
        return 1;
    } 
    else if(pid == 0)
    {/*child process*/
        execlp("/bin/ls", "ls", NULL);
        print("LINE J");
    }
    else 
    {/* parent process*/
        /* parent will wait for the child to complete*/
        wait(NULL);
        printf("Child Complete");
    }

    return 0;
}