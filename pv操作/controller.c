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

void set_sembuf_struct(struct sembuf *sem,int semnum, int semop,int semflg) 
{ 
  /* 设置信号量结构 */ 
  sem->sem_num=semnum; 
  sem->sem_op=semop; 
  sem->sem_flg=semflg; 
} 

main()
{
  char *addr, end;
  int shmid;
  //设置3个信号量  apple苹果、orange橘子、盘子plant
  int semid_apple, semid_orange, semid_mutex;/*信号量id*/
  struct sembuf sem_tmp;
    
  /*开辟共享存储区*/
  if ((shmid = shmget(SHMKEY, BUFF_LEN * PRODUCT_LEN+2, 0777|IPC_CREAT|IPC_EXCL)) == -1)
  {
    if (errno == EEXIST)
    {
      printf("The Buffer Has Existed!\n");
      printf("Do You Want To Delete The Buffer(Y = yes)?\n====:");
      scanf("%c", &end);
      if(end == 'y' || end == 'Y')
      {
      	/* 共享存储区、信号量并不随程序的结束而被删除,如果我们没删除的话， 
        可以用ipcs命令查看,用ipcrm删除 
        */ 
      	/*释放缓冲区*/
      	shmid = shmget(SHMKEY, BUFF_LEN * PRODUCT_LEN+2, 0777);
      	if (shmctl(shmid,IPC_RMID,0) < 0)
      	  perror("shmctl:");
          
      	/*同时释放信号量*/   
        semid_mutex = semget(SEMKEY_MUTEX,1, 0777); 
        semid_orange = semget(SEMKEY_ORANGE,1, 0777); 
        semid_apple = semget(SEMKEY_APPLE,1, 0777);
        semctl(semid_mutex,0,IPC_RMID);
        semctl(semid_orange,0,IPC_RMID);
        semctl(semid_apple,0,IPC_RMID);
      }
    }
    else 
      printf("Fail To Create Buffer!\n");
    return -1;
  }
  
  addr = (char*)shmat(shmid, 0, 0);/*连接缓冲区*/
  memset(addr, 0, BUFF_LEN * PRODUCT_LEN+2);
  shmdt(addr); /*离开缓冲区*/
  /*创建3个信号量:1个用于对缓冲区互斥,2个用于生产者、消费者同步*/
  if((semid_mutex = semget(SEMKEY_MUTEX,1, 0777|IPC_CREAT|IPC_EXCL))==-1) 
  { 
    if (errno == EEXIST)
      printf("The SEMKEY_MUTEX Has Existed!\n");
    else 
      printf("Fail To Create SEMKEY_MUTEX!\n");
    return -1; 
  } 
  
  if((semid_orange = semget(SEMKEY_ORANGE,1, 0777|IPC_CREAT|IPC_EXCL))==-1) 
  { 
    if (errno == EEXIST)
      printf("The SEMKEY_orange Has Existed!\n");
    else 
      printf("Fail To Create SEMKEY_orange!\n");
    return -1; 
  } 
  
  if((semid_apple = semget(SEMKEY_APPLE,1, 0777|IPC_CREAT|IPC_EXCL))==-1) 
  { 
    if (errno == EEXIST)
      printf("The SEM_APPLE Has Existed!\n");
    else 
      printf("Fail To Create SEM_APPLE!\n");
    return -1; 
  } 
  
  /*给信号量赋初值*/
  set_sembuf_struct(&sem_tmp, 0, 0, 0);/*apple-0*/
  semop(semid_apple, &sem_tmp,1);
  
  set_sembuf_struct(&sem_tmp, 0, 0, 0);/*orange-0*/
  semop(semid_orange, &sem_tmp,1);
  
  set_sembuf_struct(&sem_tmp, 0, 1, 0);/*盘子-1*/  
  semop(semid_mutex, &sem_tmp,1);
  
  return 0;
}
