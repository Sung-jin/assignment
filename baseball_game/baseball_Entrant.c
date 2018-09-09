#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MEM_SIZE     10
#define NUMTHREDS     2


int inputKeyValue();
char *inputAnswerNum();
void enterAnswer(char *otherAnswer);
void timer(void *arg);

pthread_t threads[NUMTHREDS];
int endGame = 0;

int main( void)
{

  int   shm_id_H, shm_id_E, shm_id_T, i;
  char *myAnswer, *otherAnswer;

  char *whosTurn;

  char *hostTurn = {"1"};
  char *entrantTurn = {"0"};

  int th_I, th_T;

  key_t key_H = (key_t)(inputKeyValue() + 60070);
  printf("Host Key : %u\n", key_H);
  key_t key_E = (key_t)(key_H + 1);
  printf("Entrant Key : %u\n", key_E);
  key_t key_T = (key_t)(key_H + 2);

  if (( shm_id_H = shmget( key_H, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_id_E = shmget( key_E, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_id_T = shmget( key_T, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  //create shared memory

  if (( otherAnswer = shmat( shm_id_H, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( myAnswer = shmat( shm_id_E, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( whosTurn = shmat( shm_id_T, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  //Attach shared memory to process

  strcpy(myAnswer, inputAnswerNum());
  strcpy(whosTurn, hostTurn);

  while(1){
    if(strlen(otherAnswer) != 4){
      sleep(1);
    }else{
      printf("Your opponent has entered a problem\n");
      break;
    }
  }

  printf("Play the BaseBall!!!\n");

  while(1){
      if(*whosTurn == *entrantTurn){
        if((th_I = pthread_create(&threads[0], NULL, enterAnswer, otherAnswer)) < 0){
          perror("thread");
        }
        if((th_T = pthread_create(&threads[1], NULL, timer, NULL)) < 0){
          perror("thread");
        }
        pthread_join(threads[0], NULL);

        if(endGame == 1){
          strcpy(whosTurn, "2");
          break;
        }else{
          strcpy(whosTurn, "1");
        }
      }else if(*whosTurn == *hostTurn){
        sleep(1);
      }else{
        printf("\n------------------------\n");
        printf("You lose....\n");
        printf("opponent's answer is %s", otherAnswer);
        printf("\n------------------------\n");
        break;
      }
  }

  if(shmdt(myAnswer) == -1){
    perror("shmdt");
    return -1;
  }

  //separate the shared memory I created from the process

  if(shmctl(shm_id_E, IPC_RMID, 0) == -1){
    perror("shmctl");
    return -1;
  }

  //Remove Detached Memory

  return 0;
}

int inputKeyValue(){

  char num[50], c;

  while(1){
    printf("insert Key value between 0 to 7 : ");
    scanf("%s", num);
    while(c = getchar() != '\n' && c != EOF);

    if(strlen(num) != 1){
      printf("invalid value\n");
      return inputKeyValue();
    }

    if(isdigit(num[0]) && atoi(num) < 8 && atoi(num) >= 0){
      break;
    }else{
      printf("invalid value\n");
      return inputKeyValue();
    }
  }

  return atoi(num);

}

char *inputAnswerNum(){

  char num[50], *returnNum, c;
  int i, j;

  printf("insert your answer number between 0000 to 9999 : ");

  scanf("%s", num);
  while(c = getchar() != '\n' && c != EOF);

  if(strlen(num) != 4){
    printf("invalid value\n");
    return inputAnswerNum();
  }

  for(i = 0; i < 4; i++){
    if(!isdigit(num[i])){
      printf("invalid value\n");
      return inputAnswerNum();
    }
  }

  for(i = 0; i < 4; i++){
    for(j = i + 1; j < 4; j++){
      if(num[i] == num[j]){
        printf("Duplicate numbers are not allowed.\n");
        return inputAnswerNum();
      }
    }
  }

  returnNum = malloc(5);
  strcpy(returnNum, num);

  return returnNum;

}

void enterAnswer(char *otherAnswer){

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  char num[50], c, ans[5];
  int i ,j;
  int strike = 0, ball = 0;

  strcpy(ans,otherAnswer);

  printf("\n\nYour turn!\n\n");
  printf("Insert answer : ");

  scanf("%s", num);
  while(c = getchar() != '\n' && c != EOF);

  if(strlen(num) != 4){
    printf("invalid value\n");
    return enterAnswer(otherAnswer);
  }

  for(i = 0; i < 4; i++){
    if(!isdigit(num[i])){
      printf("invalid value\n");
      return enterAnswer(otherAnswer);
    }
  }


  for(i = 0; i < 4; i++){
    for(j = i + 1; j < 4; j++){
      if(num[i] == num[j]){
        printf("Duplicate numbers are not allowed.\n");
        return enterAnswer(otherAnswer);
      }
    }
  }

  for(i = 0; i < 4; i++){
    for(j = 0 ; j < 4; j++){
      if(i == j && num[i] == otherAnswer[j]){
        strike++;
      }else if(num[i] == otherAnswer[j]){
        ball++;
      }
    }
  }

  if(strike == 4){
    printf("\n------------------------\n");
    printf("You Win!!!");
    printf("\n------------------------\n");
    endGame = 1;
    pthread_cancel(threads[1]);
  }else{
    printf("Strike : %d     Ball : %d\n",strike, ball);
    pthread_cancel(threads[1]);
  }

}

void timer(void *arg){
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  int i;
  int timer = 10;

  for(i = 0; i < timer; i++){
    if(i == 4){
      printf("\n%d seconds left.\n",timer - i - 1);
    }
    sleep(1);
  }

  printf("\n\nTime Out!!!\n\n");

  pthread_cancel(threads[0]);
}
