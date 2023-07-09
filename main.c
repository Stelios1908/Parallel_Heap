#include <unistd.h>
#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h> 
#include <semaphore.h>   
#include <fcntl.h> 
#include <sys/wait.h>
#include <time.h>   
#define  proc 40
#define  sorossize 10*proc+1  

void swap(int *soros,int n, int j)
{
    int temp = soros[n];
    soros[n] = soros[j];
    soros[j] = temp;
}
//void heapf(int n,int p)
void heapf(int n,int soros[])
{
   int j=0;
  if(n>1)
  {     
    int   j=n/2;
    if(soros[n]<soros[j])
     {
       swap(soros,n,j);
       heapf(j,soros);  
     }
  } 
}

void heapd(int n,int soros[])
{ 
   int left = 2*n;
   int right = 2*n + 1;

  if(n<sorossize)
  { 

    if(soros[left]<soros[right])
     {
       if(soros[left]<soros[n])
       {swap(soros,left,n);
       heapd(left,soros); }
     }
     else 
     {
       if(soros[right]<soros[n])
       {swap(soros,right,n);
       heapd(right,soros); }
     }
  } 
}

void printheap(int *soros)
{
  printf("==========Soros============\n");
  int i =0;
  for( i=1;i<=sorossize-1;i++)
  {
    
    printf("i thesi %d exei mesa : %d\n",i,soros[i]);
    
  } 
  printf("==========Soros============\n");

}


void heapinit(int *soros)
{
  int i =0;
  for(i=1; i<=sorossize-1;i++)
   {
      soros[i]=i;
      heapf(i,soros);
   }
}

void extract_root_and_sum(int *soros,int *diamoirazomenh)    // sinarthsh 
//poy pernei thn riza toy sorou
{       
   *diamoirazomenh +=soros[1];
   soros[1] = sorossize;
   heapd(1,soros);
}

       
int main()
{
 
 typedef  sem_t  semaphore;
 semaphore *mutex1;
 semaphore *mutex2;
 mutex1 = sem_open("sem1",O_CREAT|O_EXCL,0644,1);
 mutex2 =sem_open("sem2",O_CREAT|O_EXCL,0644,1);
 key_t key;
 key_t key2;
 key_t key3;
 key = ftok("/dev/null",7);
 key2 = ftok("/dev/null",7);
 key3 = ftok("/dev/null",5);
 printf("%d",sorossize);
 int shmid =  shmget(27374643,sorossize,IPC_CREAT|0644);
 int shmid2 = shmget(36736373,sizeof(int),IPC_CREAT|0644);
 int shmid3 = shmget(73726373,sizeof(int),IPC_CREAT|0644);
    
  if( shmid<0)
    perror("probljma mnimis\n");   //elenxos sosti desmmnimis
  else 
    printf("sosti deysmeysi mnimis\n");


 int *root =(int*) shmat(shmid,NULL,0); //o soros
 int *p = (int *) shmat(shmid2,NULL,0); //h koinh metablhth
 int *k = (int *) shmat(shmid3,NULL,0); 
 *p = 0;
 *k = 1;
  
int i;
pid_t pid[proc];
int  soros[sorossize];
  
heapinit(root);

printheap(root);

  
printf("%d\n",*p);
 clock_t start=clock();
  for(i=1;i<=proc;i++)
  {
      pid[i] = fork();
      if(pid[i]==0)
       {
       while(1)
       {
        
       sem_wait(mutex2);
      // sem_wait(mutex1);
       if(*k>=sorossize)
        {
          printf("\n diergasia %d BYE!!!\n",i);
        //  sem_post(mutex1);
          sem_post(mutex2);
          exit(0);
        }
        *k-*k+1;
      //  sem_post(mutex1);
        printf("\naritmos metrhth %d --diergasia %d",*k,i);
        *k=*k+1;
        extract_root_and_sum(root,p);
        sem_post(mutex2);
      //  nanosleep(2);
       }
       //exit(0);
       }
  }

  if(pid>0)
   {
   int status;
   wait(&status);
   sleep(1);
   sem_unlink("sem1");
   sem_close(mutex1);
   sem_unlink("sem2");
   sem_close(mutex2);
   printf("\neimai o pateras kai leo:\n ");
   printf("bye bye!!!!!!!!!\n");
   printf("telikh timh toy p = %d\n",*p);
   shmdt(root);
   shmdt(p);
   shmdt(k);
   }
    
  clock_t end=clock(); 
  long  double  sinolo=(long double)(end-start)/CLOCKS_PER_SEC;
  printf("o xronos synolo einai : %Lf\n",sinolo);
}