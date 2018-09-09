#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>

#define NUMTHRDS 5
#define MEM_SIZE 1000

struct horseInfo{
    char line[110];
    int horse;
};

struct horseInfo user1={"1 : ",0};
struct horseInfo user2={"2 : ",0};
struct horseInfo user3={"3 : ",0};
struct horseInfo user4={"4 : ",0};
//init

int ranking = 0;
int horseNum = 0;
//rank value & Turn to print horse 

double totaltime;
//check to process time

pthread_t threads[NUMTHRDS];
pthread_mutex_t mutx=PTHREAD_MUTEX_INITIALIZER;

void *totalView();
void *horseRunning(void *user);
void gotoxy(int x, int y);

int main(int argc,char **argv)
{
  system("clear");

  int i,state;
  int th_1, th_2, th_3, th_4,th_V;

  int shm_answer;
  char *buf_answer;
  key_t key_answer = 60076;

  if (( shm_answer = shmget( key_answer, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( buf_answer = shmat( shm_answer, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }
  //create shared memory for answer value

  printf("Wait for entrant's answer......\n");

  while(1){
    if(strlen(buf_answer) != 4){
      sleep(1);
    }else{
      printf("All entrant enter the answer!!!\n");
      break;
    }
  }

  for (i = 3; i>0; i--)      // count three seconds
  {
	  printf("%d . . .\n", i);
	  sleep(1);
  }

  system("clear");

  gotoxy(0,0);

  printf("welcome to the horse game!\n");
  printf("running start!\n");

  if(state=pthread_mutex_init(&mutx,NULL)){
    puts("mutex init fail...");
    exit(1);
  }

  if((th_1 = pthread_create(&threads[0], NULL, horseRunning, (void*)&user1)) <0){
    perror("thread");
  }

  if((th_2 = pthread_create(&threads[1], NULL, horseRunning, (void*)&user2)) <0){
    perror("thread");
  }

  if((th_3 = pthread_create(&threads[2], NULL, horseRunning, (void*)&user3)) <0){
    perror("thread");
  }

  if((th_4 = pthread_create(&threads[3], NULL, horseRunning, (void*)&user4)) <0){
    perror("thread");
  }

  if((th_V = pthread_create(&threads[4], NULL, totalView, NULL)) <0){
    perror("thread");
  }
  //create thread horse & view

  pthread_join(threads[4],NULL);

  sleep(5);
  //wait to show result

  pthread_mutex_destroy(&mutx);

  system("clear");

  gotoxy(0, 0);
  printf("\t\t\t\t\t");
  gotoxy(0,0);
  //clear monitor

  return 0;
}



void *horseRunning(void *user){

  struct horseInfo *client = user;
  struct timeval start_point, end_point;

  int th = 0;
  int len = 0;
  int n = 0;
  char Buffer[512];
  char tmp[1];

  int shm_id;
  char *buf;
  key_t key = 60070 + atoi(&client->line[0]);

  int shm_val;
  key_t key_val = 60075;
  char *val;

  if (( shm_id = shmget( key, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return (void *)-1;
  }

  if (( buf = shmat( shm_id, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return (void *)-1;
  }

  if (( shm_val = shmget( key_val, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return (void *)-1;
  }

  if (( val = shmat( shm_val, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return (void *)-1;
  }

  while(len<60){

    client->horse+=rand()%100+1;

    if(len<(client->horse/3000000)){
      strcat(client->line,">");
      len++;
	  gettimeofday(&start_point, NULL);
      strcat(buf,">");
	  gettimeofday(&end_point, NULL);
	  totaltime += (double)(end_point.tv_sec) + (double)(end_point.tv_usec) / 1000000.0 - (double)(start_point.tv_sec) - (double)(start_point.tv_usec) / 1000000.0;
	  //plus processing time
	}

  }
  //The process of randomizing the position of a horse

  if(shmdt(buf) == -1){
    perror("shmdt");
    return (void *)-1;
  }

  if(shmctl(shm_id, IPC_RMID, 0) == -1){
    perror("shmctl");
    return (void *)-1;
  }

  strncpy(tmp,client->line,1);

  tmp[1]='\0';
  n=atoi(tmp);

  pthread_mutex_lock(&mutx);
  strcat(val, &tmp[0]);
  ranking++;
  gotoxy(0,10+ranking);

  printf("%d horse has arrived!!!!", n);

  gotoxy(0,4+horseNum);
  pthread_mutex_unlock(&mutx);
  //Walking mutexes for global variable references
  if(shmdt(val) == -1){
    perror("shmdt");
    return (void *)-1;
  }

  if(shmctl(shm_val, IPC_RMID, 0) == -1){
    return (void *)-1;
  }

  pthread_exit( (void*)1 );
}
//monitor func
void *totalView(){
	gotoxy(0, 3);
	printf("=================================================================\n");
	printf("                                                                |\n");
	printf("                                                                |\n");
	printf("                                                                |\n");
	printf("                                                                |\n");
	printf("=================================================================\n");
	do {


		gotoxy(0, 4);

		pthread_mutex_lock(&mutx);
		horseNum++;
		printf("%s\n", user1.line);
		pthread_mutex_unlock(&mutx);

		pthread_mutex_lock(&mutx);
		horseNum++;
		printf("%s\n", user2.line);
		pthread_mutex_unlock(&mutx);

		pthread_mutex_lock(&mutx);
		horseNum++;
		printf("%s\n", user3.line);
		pthread_mutex_unlock(&mutx);

		pthread_mutex_lock(&mutx);
		horseNum++;
		printf("%s", user4.line);
		pthread_mutex_unlock(&mutx);

		horseNum = 0;

		usleep(100);
	} while (ranking<4);
	gotoxy(25, 0);
	printf("Total Time : %f\n", totaltime);

	pthread_exit((void*)1);
}

void gotoxy(int x, int y){
  printf("\033[%d;%df",y,x);
  fflush(stdout);
}
