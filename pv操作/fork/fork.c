#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 

int  main(void)
{
  pid_t pid;
  printf("start\n");
  /*在fork之前的代码只执行一次*/
  pid = fork();/*孵化一个新的进程*/
  /*
  switch (pid) {
  case -1: printf("failure!\n"); break;//孵化失败
  
  case 0:  printf("I am child!\n"); break;//这是子进程的执行代码
  
  default:  printf("my child is %d\n",pid); break;//这是父进程的执行代码
  }
  */

  for (;;) {//do something here  }/*这段代码父,子进程都会执行*/
  printf("sasa");
  }
    
  return 1;
}
