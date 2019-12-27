#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>

#define TIME_BUFFER_SIZE_STR 25

typedef struct _sh_value
{
    char str[TIME_BUFFER_SIZE_STR];
    pid_t pid;
} sh_value;

#define SH_FILE "shmfile"
  
int main() 
{ 
    
    key_t key = ftok(SH_FILE, 65); 
  
    
    int shmid = shmget(key,sizeof(sh_value),0666|IPC_CREAT); 
  
    
    sh_value *str;
  
    sh_value value;
    time_t t;
            str = (sh_value*) shmat(shmid,(void*)0,0); 
    while(1)
    {
        t = time(NULL);
        strncpy((char*)value.str, ctime(&t), TIME_BUFFER_SIZE_STR-1);
        value.str[TIME_BUFFER_SIZE_STR-1]='\0';
        value.pid = getpid();
        printf("%s %d\n", value.str, value.pid);

        *str = value;
        sleep(1);

    }  

} 