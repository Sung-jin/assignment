#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MEM_SIZE 1000

void gotoxy(int x, int y);
char *inputHorseNum();

int main(){
  int i, j, count = 0;

  system("clear");

  char rankingHorse[5] = {'\0'};

  int shm_id_1, shm_id_2, shm_id_3, shm_id_4;
  char *buf_1, *buf_2, *buf_3, *buf_4;
  int h_1 = 0, h_2 = 0, h_3 = 0, h_4 = 0, rank = 0;
  char horse_1[110] = {'\0'}, horse_2[110] = {'\0'}, horse_3[110] = {'\0'}, horse_4[110] = {'\0'};
  key_t key_1 = 60071, key_2 = 60072, key_3 = 60073, key_4 = 60074;

  int shm_val;
  char *val;
  key_t key_val = 60075;
  char buf_rank[5] = {'\0'};

  int shm_answer;
  char *answer;
  key_t key_answer = 60076;


  if (( shm_id_1 = shmget( key_1, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_id_2 = shmget( key_2, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_id_3 = shmget( key_3, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_id_4 = shmget( key_4, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_val = shmget( key_val, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }

  if (( shm_answer = shmget( key_answer, MEM_SIZE, IPC_CREAT|0666)) == -1){
    perror( "shmget");
    return -1;
  }
  //create shared memory
////////////////////////////////////////////////////////////////////////////////

  if (( buf_1 = shmat( shm_id_1, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( buf_2 = shmat( shm_id_2, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( buf_3 = shmat( shm_id_3, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( buf_4 = shmat( shm_id_4, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( val = shmat( shm_val, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }

  if (( answer = shmat( shm_answer, NULL, 0)) == (char *)-1){
    perror( "shmat");
    return -1;
  }
  //match shared memory to value

  strcpy(answer,inputHorseNum());
  system("clear");

////////////////////////////////////////////////////////////////////////////////

  //showing

  printf("=================================================================\n");
  printf("                                                                |\n");
  printf("                                                                |\n");
  printf("                                                                |\n");
  printf("                                                                |\n");
  printf("=================================================================\n");

  while(atoi(buf_rank) < 1000){
gotoxy(0, 1);


gotoxy(0, 2);
printf("1 : %s\n", horse_1);
if (strlen(horse_1) != strlen(buf_1)) {
	strcat(horse_1, ">");
}

printf("2 : %s\n", horse_2);
if (strlen(horse_2) != strlen(buf_2)) {
	strcat(horse_2, ">");
}

printf("3 : %s\n", horse_3);
if (strlen(horse_3) != strlen(buf_3)) {
	strcat(horse_3, ">");
}

printf("4 : %s\n", horse_4);
if (strlen(horse_4) != strlen(buf_4)) {
	strcat(horse_4, ">");
}

if (strlen(buf_rank) != strlen(val)) {
	for (i = 0; i < strlen(val); i++) {
		buf_rank[i] = val[i];
	}
}
  }

  for (i = 0; i < 4; i++) {
	if (answer[i] == buf_rank[i])
		count++;
  }

  gotoxy(0, 7);
  printf("End the game!\n");
  sleep(2);
  gotoxy(0, 8);
  printf("The victory horse is %c !!!!!!\n", buf_rank[0]);
  printf("The second horse is %c !!!!!!\n", buf_rank[1]);
  printf("The third horse is %c !!!!!!\n", buf_rank[2]);
  printf("The fourth horse is %c !!!!!!\n", buf_rank[3]);
  printf("The answer you entered is %s\n", answer);
  if (count == 4) {
	  printf("You win!!!!!!\n");
  }
  else {
	  printf("You lose.....\n");
	  printf("You %d points earned\n", count);
  }
  sleep(5);
  //result view
  system("clear");
  gotoxy(0, 0);

  if(shmdt(answer) == -1){
    perror("shmdt");
    return -1;
  }

  if(shmctl(shm_answer, IPC_RMID, 0) == -1){
    perror("shmctl");
    return -1;
  }

  return 0;
}

void gotoxy(int x, int y) {
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}

char *inputHorseNum() {

	//input answer func

	char num[50], c;
	char *returnNum;
	int i, j;

	printf("Please enter your estimated ranking : ");

	scanf("%s", num);
	while (c = getchar() != '\n' && c != EOF);

	if (strlen(num) != 4) {
		printf("invalid value\n");
		return inputHorseNum();
	}

	for (i = 0; i < 4; i++) {
		for (j = i + 1; j < 4; j++) {
			if (num[i] == num[j]) {
				printf("Duplicate numbers are not allowed.\n");
				return inputHorseNum();
			}
		}
	}
	for (i = 0; i < 4; i++) {
		if (num[i] != '1' && num[i] != '2' && num[i] == '3' && num[i] == '4') {
			printf("invalid value\n");
			return inputHorseNum();
		}
	}

	returnNum = malloc(5);
	strcpy(returnNum, num);

	return returnNum;
}
