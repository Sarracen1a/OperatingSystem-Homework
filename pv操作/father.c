/*
father
P（盘子）
放苹果
V(苹果)
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#define SHMKEY 9075 /*共享存储区的键*/
#define SEMKEY_APPLE 9085 
#define SEMKEY_ORANGE 9086 
#define SEMKEY_MUTEX 9087 /*信号量数组的键*//*注意:上面的键在系统中必须唯一*/
#define BUFF_LEN 1  /*缓冲区可以存放10个产品*/
#define PRODUCT_LEN 32 /*每个产品是一个字符串:<=32字符*/

/*下面的P,V是对系统调用的简单封装*/
int P(int semid)
{
  struct sembuf p_buf;
  p_buf.sem_num = 0;
  p_buf.sem_op = -1;
  p_buf.sem_flg = 0;
  if(semop(semid, &p_buf, 1)==-1)/*semop参见课件ppt*/
  {
    perror ("p (semid) falsed");
    exit (1);
  }
  else
    return (0);
}

int V(int semid)
{
  struct sembuf v_buf;/*struct sembuf 参见课件ppt*/
  v_buf.sem_num = 0;
  v_buf.sem_op = 1;
  v_buf.sem_flg = 0;
  if(semop(semid, &v_buf, 1)==-1)
  {
    perror (" v (semid) failed");
    exit (1);
  } 
  else
    return (0);
}

main()
{
  char *p_buffer;/*共享存储区地址*/
  unsigned char in;/*生产者存放产品的指针:它的值存放在全局缓冲区第一个字节*/
  char product[128]="apple";/*事实只使用32B,128为了避免屏幕输入超过32*/
  int shmid;/*共享存储区id*/
  int semid_apple, semid_orange, semid_mutex;/*信号量集合id*/

  shmid = shmget(SHMKEY, BUFF_LEN * PRODUCT_LEN+2, 0777);/*连接共享存储区:2 存放in,out的值*/
  p_buffer = (char*)shmat(shmid, 0, 0);/*取共享存储区地址*/

  semid_mutex = semget(SEMKEY_MUTEX,1, 0777);/*获取全局信号量id*/
  semid_apple = semget(SEMKEY_APPLE,1, 0777);
  semid_orange = semget(SEMKEY_ORANGE,1, 0777); 

  /*从屏幕接收产品*/
  //gets(product);/*128的意义在此,此函数不检查字符串长度*/
  
  /*进入临界区*/
  P(semid_mutex);/*对公有信号量作P操作*/

  printf("apple in\n====:");
  in = (unsigned char)(*p_buffer);
  strncpy(p_buffer + 2 + in * PRODUCT_LEN, product, PRODUCT_LEN);
  in = (in + 1) % BUFF_LEN;
  *p_buffer = (char)in;
  shmdt(p_buffer); /*离开缓冲区*/
  
  /*离开临界区*/
  V(semid_apple);
}

