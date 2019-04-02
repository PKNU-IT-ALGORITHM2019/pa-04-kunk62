#pragma warning (disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define buffer_len 30
#define line_len 3000
#define N 15789

typedef struct category {
	char *IP;
	char *SortTime; //정렬을 위한 시간 요소를 따로 정의
	char *Time;
	char *URL;
	int status;
}Category;

char *month(char *token); //영어로 된 월별 문자열을 숫자 문자열로 치환
void read(char *s);
void sort(int mode);
int read_line(FILE * fp, char str[], int n);
int compare1(const void *v1, const void *v2); //시간 기준으로 비교
int compare2(const void *v1, const void *v2); //IP 기준으로 비교
void print();
Category list[N];
int status; //출력 상태 설정

int main() {
	char command_line[buffer_len];
	char *cmd, *arg1, *arg2;

	while (1) {
		printf("$ ");
		if (read_line(stdin, command_line, buffer_len) <= 0)
			continue;
		cmd = strtok(command_line, " ");
		if (strcmp(cmd, "read") == 0) {
			arg1 = strtok(NULL, " ");
			if (arg1 == NULL) continue;
			read(arg1);
		}
		else if (strcmp(cmd, "print") == 0)
			print();
		else if (strcmp(cmd, "exit") == 0)
			exit(1);
		else if (strcmp(cmd, "sort") == 0) {
			arg2 = strtok(NULL, " ");
			if (arg2 == NULL) continue;
			else if (strcmp(arg2, "-t") == 0) { status = 1; sort(1); }
			else if (strcmp(arg2, "-ip") == 0) { status = 2; sort(2); }
		}
		else printf("Command do not exist.\n\n");
	}
	return 0;
}

int read_line(FILE * fp, char str[], int n)
{
	int ch, i = 0;
	while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		if (i < n)
			str[i++] = ch;
	str[i] = '\0';
	return i;
}

void read(char *s) {
	FILE *fp;
	char buffer[line_len];
	char now[20];
	char mon[3];
	char *token, *timetoken;
	int i = 0;

	fp = fopen(s, "r");
	if (fp == NULL) {
		printf("Error!\n\n");
		exit(1);
	}

	while (!feof(fp)) {
		read_line(fp, buffer, line_len);
		if (strcmp(_strdup(buffer), "IP,Time,URL,Status") == 0) continue;
		else {
			token = strtok(_strdup(buffer), ",");
			list[i].IP = token;
			token = strtok(NULL, ",[/");
			now[6] = token[0];
			now[7] = token[1];
			token = strtok(NULL, "/");
			strcpy(mon, month(token));
			now[4] = mon[0];
			now[5] = mon[1];
			token = strtok(NULL, "/:");
			now[0] = token[0];
			now[1] = token[1];
			now[2] = token[2];
			now[3] = token[3];
			now[8] = '\0';
			token = strtok(NULL, ",");
			strcat(now, _strdup(token));
			list[i].SortTime = (char *)malloc(sizeof(char) * 20);
			strcpy(list[i].SortTime, now);
			token = strtok(NULL, ",");
			list[i].URL = token;
			token = strtok(NULL, ",");
			list[i].status = atoi(token);
			timetoken = strtok(_strdup(buffer), ",");
			timetoken = strtok(NULL, ",[");
			list[i].Time = timetoken;
			i++;
		}
	}
	fclose(fp);
}

void sort(int mode) {
	if (mode == 1) //time
		qsort(list, N, sizeof(Category), compare1);

	else if (mode == 2) { //ip
		qsort(list, N, sizeof(Category), compare2);
	}
}

void print(){
	if (status == 1) { //time
		for (int i = 0; i < N; i++)
			printf("%s\n\tIP: %s\n\tURL: %s\n\tStatus: %d\n", list[i].Time, list[i].IP, list[i].URL, list[i].status);
	}
	if (status == 2) { //ip
		for (int i = 0; i < N; i++)
			printf("%s\n\tTime: %s\n\tURL: %s\n\tStatus: %d\n", list[i].IP, list[i].Time, list[i].URL, list[i].status);
	}
}

char *month(char *token) {
	if (strcmp(token, "Jan")==0)
		return "01";
	else if (strcmp(token, "Feb")==0)
		return "02";
	else if (strcmp(token, "Mar") == 0)
		return "03";
	else if (strcmp(token, "Apr") == 0)
		return "04";
	else if (strcmp(token, "May") == 0)
		return "05";
	else if (strcmp(token, "Jun") == 0)
		return "06";
	else if (strcmp(token, "Jul") == 0)
		return "07";
	else if (strcmp(token, "Aug") == 0)
		return "08";
	else if (strcmp(token, "Sep") == 0)
		return "09";
	else if (strcmp(token, "Oct") == 0)
		return "10";
	else if (strcmp(token, "Nov") == 0)
		return "11";
	else return "12";
}

int compare1(const void *v1, const void *v2) {
	const Category *c1 = (const Category *)v1;
	const Category *c2 = (const Category *)v2;
	if (strcmp(c1->SortTime, c2->SortTime) > 0) return 1;
	else if (strcmp(c1->SortTime, c2->SortTime) == 0) return 0;
	else return -1;
}

int compare2(const void *v1, const void *v2) {
	const Category *c1 = (const Category *)v1;
	const Category *c2 = (const Category *)v2;
	if (strcmp(c1->IP, c2->IP) > 0) return 1;
	else if (strcmp(c1->IP, c2->IP) == 0)
		return (c1->Time) - (c2->Time);
	else return -1;
}