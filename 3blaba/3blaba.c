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
#define DATASIZE_TIME sizeof(time_t)

char * settime(struct tm *u)
{
  char s[40];
  char *tmp;
  for (int i = 0; i<40; i++) s[i] = 0;
  int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);
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
    printf("Time = %s\n", f);
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
        // Parent pid
        printf("It's parent process\n");
        printf("\tPPID: %d\n\tPID: %d\n",getppid(), getpid());
        lt = time(NULL); // Update time before writing to pipe
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        if(fifo_fd == -1)
        {
            printf("Can't open FIFO %x\n", fifo_fd);
            int res = 0;
            kill(child_pid, SIGINT);
            waitpid(child_pid, &res, 0);
            unlink(FIFO_PATH);
            exit(1);
        }
        write(fifo_fd, &lt, DATASIZE_TIME);
        u = localtime(&lt);
        f = settime(u);
        printf("\tWritten time is %s\n", f);
        int res = 0;
        close(fifo_fd);
        sleep(1);
        waitpid(child_pid, &res, 0);
        unlink(FIFO_PATH);
    }
    else
    {
        // Forked pid
        printf("It's child process\n");
        printf("\tPPID: %d\n\tPID: %d\n",getppid(), getpid());
        u = localtime(&lt);
        f = settime(u);
        printf("\tBase time was %s\n", f);
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        if(fifo_fd == -1)
        {
            printf("Can't open FIFO %x\n", fifo_fd);            
            exit(1);
        }
        read(fifo_fd, &lt, DATASIZE_TIME);
        close(fifo_fd);
        u = localtime(&lt);
        f = settime(u);
        printf("\tRead time is %s\n", f);
        sleep(1);
        lt = time(NULL);
        u = localtime(&lt);
        f = settime(u);
        printf("\tNow time is %s\n", f);
    }
    return 0;
}