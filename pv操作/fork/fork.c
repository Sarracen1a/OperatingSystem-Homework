#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 

int  main(void)
{
  pid_t pid;
  printf("start\n");
  /*��fork֮ǰ�Ĵ���ִֻ��һ��*/
  pid = fork();/*����һ���µĽ���*/
  /*
  switch (pid) {
  case -1: printf("failure!\n"); break;//����ʧ��
  
  case 0:  printf("I am child!\n"); break;//�����ӽ��̵�ִ�д���
  
  default:  printf("my child is %d\n",pid); break;//���Ǹ����̵�ִ�д���
  }
  */

  for (;;) {//do something here  }/*��δ��븸,�ӽ��̶���ִ��*/
  printf("sasa");
  }
    
  return 1;
}
