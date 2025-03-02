#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <stdbool.h>
#include <time.h>

#define BUF_SIZE 100
#define MAX_CLNT 32
#define ID_SIZE 10
#define ARR_CNT 5
#define NAME_SIZE 20

#define DEBUG

typedef struct s_time_info
{
	int alarm_hour;			//0~23
	int alarm_minute;		//0~59
	int hour;				//0~23
	int minute;				//0~59
	int sec;				//0~59
	int hour_until_alarm; 	//0~23
	int minute_until_alarm; //0~59
	int led;				//0~10
	int buzzer;			//0 or 1
	int motor;				//0 or 1
	int buttonPressed;
	time_t timer;
	struct tm *t;
} t_time_info;

typedef struct s_msg_info
{
	char fd;
	char *msg;
	int len;
} t_msg_info;

typedef struct s_stm_info
{
	int fd;
	char ip[20];
	pthread_mutex_t *mutx;
	t_time_info *time_info;
} t_stm_info;

typedef struct s_bluetooth_info
{
	int btfd;	
	char sendid[NAME_SIZE];
	t_time_info *time_info;
} t_bluetooth_info;

//main.c
void	error_handling(char *msg);
void	log_file(char *msgstr);
void	getlocaltime(char *buf);

//socket.c
void 	serverOpen(char *argv[], pthread_mutex_t mutx, t_time_info *time_info);
void	bluetoothConnect(char dest[18], t_time_info *time_info);

//stmThread.c
void	*stm_connection(void *arg);

//bluetoothThread.c
void	*bluetoothThread(void *arg);

//utils.c
void	error_handling(char *msg);
void	log_file(char * msgstr);
void	getlocaltime(char * buf);

//init.c
t_time_info *initMain(int argc, char *argv[], pthread_mutex_t *mutx);

//time.c
t_time_info *updateTime(t_time_info *time_info);
void printTime(t_time_info *t);

#endif