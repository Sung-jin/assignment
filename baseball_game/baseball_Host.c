#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MEM_SIZE     10    //공유 메모리 크기
#define NUMTHRDS      2


int inputKeyValue();                  //키값 입력, 같은 값을 입력해야 같이 매칭이 됨
char *inputAnswerNum();               //0000~9999 까지 문제를 내는 함수
void enterAnswer(char *otherAnswer);  //정답 입력 쓰레드 함수
void timer(void *arg);                //타이머 쓰레드 함수

pthread_t threads[NUMTHRDS];          //쓰레드 식별자
int endGame = 0;                      //게임이 끝날 경우 1로 변경

int main( void)
{

  int   shm_id_H, shm_id_E, shm_id_T, i;
  // 호스트와 참가자, 턴의 공유 메모리에 대한 아이디 값
  char *myAnswer, *otherAnswer;
  //나의 정답과 상대방의 정답 저장 공간
  char *whosTurn;
  //턴의 저장 공간
  char *hostTurn = {"1"};
  char *entrantTurn = {"0"};

  int th_I, th_T;

  key_t key_H = (key_t)(inputKeyValue() + 60070);
  printf("Host Key : %u\n", key_H);
  key_t key_E = (key_t)(key_H + 1);
  printf("Entrant Key : %u\n", key_E);
  key_t key_T = (key_t)(key_H + 2);
  //7조 이므로 사용 가능한 키 값은 60070~9까지임. 즉, 함수에서 0에서 7까지만 입력 받아 호스트와 참가자에게 키값 부여
  //입력 받은 데이터에서 +2를 통해 누구의 턴인가에 대한 정보를 저장

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
  //공유 메모리 생성

  if (( myAnswer = shmat( shm_id_H, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( otherAnswer = shmat( shm_id_E, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( whosTurn = shmat( shm_id_T, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }
  //공유 메모리를 프로세스에 첨부

  strcpy(myAnswer, inputAnswerNum());
  strcpy(whosTurn, hostTurn);
  //내 정답과 턴 정보에 데이터 입력
  //호스트가 무조건 선턴을 가지므로 턴 공간에 호스트의 턴을 삽입

  while(1){
    if(strlen(otherAnswer) != 4){
      sleep(1);
    }else{
      printf("Your opponent has entered a problem\n");
      break;
    }
  }
  //상대방이 정답을 입력할 때 까지 기다림

  printf("Play the BaseBall!!!\n");

  while(1){
      if(*whosTurn == *hostTurn){
        if((th_I = pthread_create(&threads[0], NULL, enterAnswer, otherAnswer)) < 0){
          perror("thread");
        }
        if((th_T = pthread_create(&threads[1], NULL, timer, NULL)) < 0){
          perror("thread");
        }
        pthread_join(threads[0], NULL);
        //스레드를 생성 한 후, threads[0]이 종료되는 걸 기다린다.
        //타이머가 끝나더라도 정답 입력의 스레드를 캔슬시키고, 정답을 입력시에도
        //타이머가 캔슬 되므로 정답 입력이 종료가 되면 모든 쓰레드는 종료가 된 것임.

        if(endGame == 1){
          strcpy(whosTurn, "2");
          break;
          //정답 입력에서 정답을 입력 시, 전역변수인 endGame을 종료인 1로 변화하므로 이것으로
          //게임이 끝났나 안끝났나 확인
        }else{
          strcpy(whosTurn, entrantTurn);
          //정답이 아닐 경우 턴을 상대방으로 넘김.
        }
      }else if(*whosTurn == *entrantTurn){
        sleep(1);
      }else{
        printf("\n------------------------\n");
        printf("You lose....\n");
        printf("opponent's answer is %s", otherAnswer);
        printf("\n------------------------\n");
        break;
      }//턴이 1이면 호스트턴, 0이면 참가자턴, 둘다 아니면 끝
  }

  if(shmdt(whosTurn) == -1){
    perror("shmdt");
    return -1;
  }

  if(shmdt(myAnswer) == -1){
    perror("shmdt");
    return -1;
  }
  //내가 만든 공유메모리를 프로세스에서 분리

  if(shmctl(shm_id_T, IPC_RMID, 0) == -1){
    perror("shmctl");
    return -1;
  }

  if(shmctl(shm_id_H, IPC_RMID, 0) == -1){
    perror("shmctl");
    return -1;
  }
  //분리 된 메모리를 제거

  return 0;
}

int inputKeyValue(){

  char num[50], c;

  while(1){
    printf("insert Key value between 0 to 7 : ");
    scanf("%s", num);
    while(c = getchar() != '\n' && c != EOF);
    //버퍼에 남은 문자를 제거

    if(strlen(num) != 1){
      printf("invalid value\n");
      return inputKeyValue();
    }
    //문자가 1개가 아니면 잘못 된 입력이므로 함수 재귀호출

    if(isdigit(num[0]) && atoi(num) < 8 && atoi(num) >= 0){
      break;
    }else{
      printf("invalid value\n");
      return inputKeyValue();
    }
    //입력 받은 문자가 숫자가 아니거나 그 범위가 0~7이 아닐 경우 함수 재귀호출
    //0~7까지 입력 받기 위함임
    //사용 가능한 키값이 60070~9까지이고, 키값을 입력 받으면 +1, +2을 해서 두개 더 만드므로
    //isdigit은 문자가 숫자인지 아닌지 판별하는 함수
    //atoi는 문자열을 숫자로 변환시켜주는 함수
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
  //받은 문자열이 4개가 아니라면 잘 못 된 입력이므로 재귀호출

  for(i = 0; i < 4; i++){
    if(!isdigit(num[i])){
      printf("invalid value\n");
      return inputAnswerNum();
    }
  }
  //각각의 문자열에 대해 모두가 숫자가 아니면 잘 못 된 입력이므로 재귀호출

  for(i = 0; i < 4; i++){
    for(j = i + 1; j < 4; j++){
      if(num[i] == num[j]){
        printf("Duplicate numbers are not allowed.\n");
        return inputAnswerNum();
      }
    }
  }
  //각각의 자리를 비교하여 중복 된 숫자가 있으면 잘 못 된 입력이므로 재귀호출

  returnNum = malloc(5);
  strcpy(returnNum, num);
  //0000~9999까지 위에서 필터를 거쳐 알맞은 수를 입력 받으면
  //캐릭터 포인터를 리턴해야 하므로 그에 맞는 형식을 초기화 한 후
  //strcpy로 필터를 거친 문자를 리턴하는 값에 저장
  //malloc을 5를 한 이유는 문자열 맨 마지막에 '\n'이 들어가기 때문

  return returnNum;

}

void enterAnswer(char *otherAnswer){

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  //스레드 캔슬이 올 시, 스레드 종료 하겠다는 인자 = PTHREAD_CANCEL_ASYNCHRONOUS

  char num[50], c, ans[5];
  int i, j;
  int strike = 0, ball = 0;

  strcpy(ans,otherAnswer);
  //상대방의 정답 저장

  printf("\n\nYour turn!\n\n");
  printf("Insert answer : ");

  scanf("%s", num);
  while(c = getchar() != '\n' && c != EOF);

  if(strlen(num) != 4){
    printf("invalid value\n");
    return enterAnswer(otherAnswer);
  }
  //문자가 4개가 아니면 잘 못 된 입력이므로 재귀호출

  for(i = 0; i < 4; i++){
    if(!isdigit(num[i])){
      printf("invalid value\n");
      return enterAnswer(otherAnswer);
    }
  }
  //모든 입력받은 문자가 각각 숫자가 아니면 잘 못 된 입력이므로 재귀호출

  for(i = 0; i < 4; i++){
    for(j = i + 1; j < 4; j++){
      if(num[i] == num[j]){
        printf("Duplicate numbers are not allowed.\n");
        return enterAnswer(otherAnswer);
      }
    }
  }
  //각각의 자리를 비교하여 중복 된 값이 있으면 잘 못 된 입력이므로 재귀호출

  for(i = 0; i < 4; i++){
    for(j = 0 ; j < 4; j++){
      if(i == j && num[i] == otherAnswer[j]){
        strike++;
      }else if(num[i] == otherAnswer[j]){
        ball++;
      }
    }
  }
  //같은 자리에 같은 숫자가 있으면 strike 카운트 증가
  //같은 자리는 아니지만 같은 숫자가 존재하면 ball 카운트 증가

  if(strike == 4){
    printf("\n------------------------\n");
    printf("You Win!!!");
    printf("\n------------------------\n");
    endGame = 1;
    pthread_cancel(threads[1]);
    //strike 카운트가 4면 정답을 입력 했으므로, 승리
    //endGame을 1로 만들어서 게임이 종료 되었다는 것을 알림
    //스레드 캔슬을 통해 타이머 스레드 종료
  }else{
    printf("Strike : %d     Ball : %d\n",strike, ball);
    pthread_cancel(threads[1]);
    //타이머는 끝나지 않았지만 정답을 입력했으면 타이머는 필요 없으므로 스레드 캔슬
  }
}

void timer(void *arg){
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  //스레드 캔슬이 올 시, 스레드 종료 하겠다는 인자 = PTHREAD_CANCEL_ASYNCHRONOUS

  int i;
  int timer = 10;
  //제한시간

  for(i = 0; i < timer; i++){
    if(i == 4){
      printf("\n%d seconds left.\n",timer - i - 1);
    }
    sleep(1);
  }
  //5초마다 몇초 남았는지 알림

  printf("\n\nTime Out!!!\n\n");

  pthread_cancel(threads[0]);
  //타이머가 끝났지만 정답을 입력 못했을 경우 제한시간이 종료되었으므로
  //정답 입력 스레드 캔슬
}
