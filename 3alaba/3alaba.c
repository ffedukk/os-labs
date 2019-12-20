#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char *settime(struct tm *u)
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

    int time_pipe[2];
    if(pipe(time_pipe) < 0)
    {
        printf("Error - Pipe < 0\n");
        return 1;
    }

    pid_t child_pid;
    if((child_pid = fork()) != 0)
    {
        // Parent pid
        printf("It's parent process\n");
        printf("\tPPID: %d\n\tPID: %d\n",getppid(), getpid());
        lt = time(NULL); // Update time
        write(time_pipe[1], &lt, sizeof(time_t));
        u = localtime(&lt);
        f = settime(u);
        printf("\tWritten time is %s\n", f);
        int res = 0;
        sleep(1);
        waitpid(child_pid, &res, 0);
    }
    else
    {
        // child pid
        printf("It's child process\n");
        printf("\tPPID: %d\n\tPID: %d\n",getppid(), getpid());
        u = localtime(&lt);
        f = settime(u);
        printf("\tBase time was %s\n", f);
        read(time_pipe[0], &lt, sizeof(time_t));
        u = localtime(&lt);
        f = settime(u);
        printf("\tReat time is %s\n", f);
        sleep(1);
        lt = time(NULL);
        u = localtime(&lt);
        f = settime(u);
        printf("\tNow time is %s\n", f);
    }
    return 0;
}