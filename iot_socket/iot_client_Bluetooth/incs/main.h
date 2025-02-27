#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define ARR_CNT 5

typedef struct 
{
	int sockfd;	
	int btfd;	
	char sendid[NAME_SIZE];
} DEV_FD;

void *send_msg(void * arg);
void *recv_msg(void * arg);
void error_handling(char * msg);

#endif