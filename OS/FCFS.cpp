#include <stdio.h>
#include <stdlib.h>

#define time_quantum 20
#define MAX 255

typedef struct fcfs_in {
	int type;
	int process_id;
	int priorty;
	int computing_time;
} fcfs_in;

typedef struct fcfs {
	int process_id;
	int priorty;
	int computing_time;
	int remain_tiem;
	int turn_arround_time;
	int computing_start_time;
	struct fcfs *prev;
	struct fcfs *next;
} fcfs;

void insert_fcfs(fcfs_in *data, int cnt);
void init_dlist(fcfs *head, fcfs *tail);
void print_all();
void scheduling();

fcfs *head, *tail;

int m_start_time = 0;


void main() {
	fcfs_in fi[MAX];
	int i = 0, cnt = 0;

	FILE *f;
	f = fopen("Data.txt", "r");

	for (i = 0; i < MAX; i++) {
		fscanf(f, "%d %d %d %d", &fi[i].type, &fi[i].process_id, &fi[i].priorty, &fi[i].computing_time);
		if (fi[i].type == -1) {
			cnt = i;
			break;
		}
	}

	head = (fcfs*)malloc(sizeof(fcfs));
	tail = (fcfs*)malloc(sizeof(fcfs));

	init_dlist(head,tail);

	insert_fcfs(fi, cnt);

	while (1) {
		scheduling();
		if (tail->prev->remain_tiem == 0)
			break;
	}

	print_all();

}

void insert_fcfs(fcfs_in *data, int cnt) {
	int i;
	fcfs *temp;

	for (i = 0; i < cnt; i++) {
		if (data[i].type == 1) {
			scheduling();
		}
		else {
			temp = (fcfs*)malloc(sizeof(fcfs));
			temp->process_id = data[i].process_id;
			temp->priorty = data[i].priorty;
			temp->computing_time = data[i].computing_time;
			temp->remain_tiem = data[i].computing_time;
			temp->computing_start_time = m_start_time;
			tail->prev->next = temp;
			temp->prev = tail->prev;
			tail->prev = temp;
			temp->next = tail;
		}
	}

}

void init_dlist(fcfs *head, fcfs *tail) {	//해드와 테일 노드 초기화
	head->next = tail;
	head->prev = head;
	tail->next = tail;
	tail->prev = head;
}

void print_all() {
	fcfs *i;
	int cnt = 1;
	i = head->next;
	while (i != tail) {
		printf("%d번째 노드 process_id : %d \t priority : %d \t computing_time : %d \t turn_arround_time : %d\n", cnt, i->process_id, i->priorty, i->computing_time, i->turn_arround_time);
		i = i->next;
		cnt++;
	}
}

void scheduling() {
	fcfs *temp;
	int cnt_temp = time_quantum;
	temp = (fcfs*)malloc(sizeof(fcfs));
	temp = head->next;
	
	while (1) {
		while (1) {
			if (temp->remain_tiem == 0)
				temp = temp->next;
			else break;
		}
		if (temp->remain_tiem > cnt_temp) {
			temp->remain_tiem -= cnt_temp;
			m_start_time += cnt_temp;
			break;
		}
		else {
			cnt_temp -= temp->remain_tiem;
			m_start_time += temp->remain_tiem;
			temp->remain_tiem = 0;
			temp->turn_arround_time = m_start_time - temp->computing_start_time;
			temp = temp->next;
			if (temp == tail)
				break;
		}
	}

}