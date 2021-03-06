#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PATH "/tmp/time_fifo"

char * settime(struct tm *u)
{
  char s[40];
  char *tmp;
  int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);
  memset(s,40,0);
  tmp = (char*)malloc(sizeof(s));
  strcpy(tmp, s);
  return(tmp);
}

int main()
{
    struct tm *u;
    char *f;
    time_t lt;
    lt = time(NULL);
    u = localtime(&lt);
    f = settime(u);
    printf("Initial time : %s\n", f);
    sleep(1); 

    if(mkfifo(FIFO_PATH, 0666) == -1)
    {
        printf("Can't make FIFO with name %s\n", FIFO_PATH);
        exit(1);
    }
    else
    {
        printf("FIFO with name %s created successfully.\n", FIFO_PATH);
    }
    
    pid_t child_pid;
    int fifo_fd;
    if((child_pid = fork()) != 0)
    {
        // Parent 
        printf("Parent process:\n");
        lt = time(NULL); // Update time
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        if(fifo_fd == -1)
        {
            printf("Can't open FIFO %x\n", fifo_fd);
            kill(child_pid, SIGINT);
            waitpid(child_pid, NULL, 0);
            unlink(FIFO_PATH);
            exit(1);
        }
        write(fifo_fd, &lt, sizeof(time_t));
        u = localtime(&lt);
        f = settime(u);
        printf("\tWritten time : %s\n", f);
        int res = 0;
        close(fifo_fd);
        sleep(1);
        waitpid(child_pid, &res, 0);
        unlink(FIFO_PATH);
    }
    else
    {
        // Child prosses
        printf("Child process:\n");
        u = localtime(&lt);
        f = settime(u);
        printf("\tInitial time : %s\n", f);
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        if(fifo_fd == -1)
        {
            printf("Can't open FIFO %x\n", fifo_fd);            
            exit(1);
        }
        read(fifo_fd, &lt, sizeof(time_t));
        close(fifo_fd);
        u = localtime(&lt);
        f = settime(u);
        printf("\tRead time : %s\n", f);
        sleep(1);
        lt = time(NULL);
        u = localtime(&lt);
        f = settime(u);
        printf("\tNow time : %s\n", f);
    }
    return 0;
}