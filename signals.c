#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#define MAX_PIDS 32

volatile pid_t *pids;
int k=0;
int j=4;

void do_child(void)
{
  if(k<6)
  {
        printf("pid %d ready\n", getpid());
  }
  if(k==6)
  {
   for(int i=4; i>=0; i--)
   {
        kill(pids[i], SIGHUP);  
        printf("pid %d caught one   \n",pids[i]); 
   }
   for(int i=4; i>=0; i--)
   {
        kill(pids[i], SIGINT);
        
   }
   
   for(int i=0; i<5; i++)
   {
        
        printf("%d is death \n",pids[i]);
        kill(pids[i], SIGKILL);
   }
   
   
  }
  
}

void sigint() 
{ 
    signal(SIGINT, sigint); 
} 
  
void sighup() 
{ 
    signal(SIGHUP, sighup); 
} 
void sigquit() 
{ 
    
    exit(0); 
}
int main(int argc, char **argv)
{
  pid_t pid;

  pids = mmap(0, MAX_PIDS*sizeof(pid_t), PROT_READ|PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  memset((void *)pids, 0, MAX_PIDS*sizeof(pid_t));

  for(int i=0; i<=5; i++)
  {
    k++;
    pid=fork();
    if(pid==0)
    {
      do_child();
       signal(SIGHUP, sighup); 
        signal(SIGINT, sigint); 
        signal(SIGQUIT, sigquit); 
      for (;;) 
            ;
    } else if (pid<0)
    {
      perror("fork failed");
    }
    else
    {
      pids[i] = pid;
      sleep(1);
    }
  }
  
  
  exit(0);
}
