#include "../incs/main.h"

int clnt_cnt = 0;
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	int sock_option  = 1;
	pthread_t t_id[MAX_CLNT] = {0};
	int str_len = 0;
	int i;
	char idpasswd[(ID_SIZE*2)+3];
	char *pToken;
	char *pArray[ARR_CNT]={0};
	char msg[BUF_SIZE];

	CLIENT_INFO client_info[MAX_CLNT] = 
	{
		{0,-1,"","1","PASSWD"}, \
		{0,-1,"","2","PASSWD"},  {0,-1,"","3","PASSWD"}, \
		{0,-1,"","4","PASSWD"},  {0,-1,"","5","PASSWD"}, \
		{0,-1,"","6","PASSWD"},  {0,-1,"","7","PASSWD"}, \
		{0,-1,"","8","PASSWD"},  {0,-1,"","9","PASSWD"}, \
		{0,-1,"","10","PASSWD"},  {0,-1,"","11","PASSWD"}, \
		{0,-1,"","12","PASSWD"},  {0,-1,"","13","PASSWD"}, \
		{0,-1,"","14","PASSWD"},  {0,-1,"","15","PASSWD"}, \
		{0,-1,"","16","PASSWD"},  {0,-1,"","17","PASSWD"}, \
		{0,-1,"","18","PASSWD"},  {0,-1,"","19","PASSWD"}, \
		{0,-1,"","20","PASSWD"},  {0,-1,"","21","PASSWD"}, \
		{0,-1,"","22","PASSWD"},  {0,-1,"","23","PASSWD"}, \
		{0,-1,"","24","PASSWD"},  {0,-1,"","25","PASSWD"}, \
		{0,-1,"","LYJ_SQL","PASSWD"},  {0,-1,"","LYJ_BT","PASSWD"}, \
		{0,-1,"","LYJ_STM32","PASSWD"},  {0,-1,"","LYJ_LIN","PASSWD"}, \
		{0,-1,"","LYJ_AND","PASSWD"},  {0,-1,"","LYJ_ARD","PASSWD"}, \
		{0,-1,"","HM_CON","PASSWD"}
	};

	if (argc != 2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	fputs("IoT Server Start!!\n", stdout);

	if (pthread_mutex_init(&mutx, NULL))
		error_handling("mutex init error");


	//socket connetion setting
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&sock_option, sizeof(sock_option));
	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");



	//start connection with accept, never ending loop
	while (1) 
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
		if (clnt_cnt >= MAX_CLNT) //when socket is full
		{
			printf("socket full\n");
			shutdown(clnt_sock, SHUT_WR);
			continue;
		}
		else if (clnt_sock < 0) //when accept returns wrong things -> accept error
		{
			perror("accept()");
			continue;
		}

		str_len = read(clnt_sock, idpasswd, sizeof(idpasswd));
		idpasswd[str_len] = '\0';

		if (str_len > 0) //read ok
		{
			i = 0;
			pToken = strtok(idpasswd,"[:]");

			while (pToken != NULL) //split[:] read buffer into pArray
			{
				pArray[i] =  pToken;
				if (i++ >= ARR_CNT)
					break;	
				pToken = strtok(NULL,"[:]");
			}

			for (i = 0; i < MAX_CLNT; i++)
			{
				if (!strcmp(client_info[i].id, pArray[0])) //find matching client_info id
				{
					if (client_info[i].fd != -1) //if it is already logged in
					{
						sprintf(msg, "[%s] Already logged!\n", pArray[0]);
						write(clnt_sock, msg, strlen(msg));
						log_file(msg);
						shutdown(clnt_sock, SHUT_WR);
#if 1   //for MCU
						client_info[i].fd = -1;
#endif  
						break;
					}
					if (!strcmp(client_info[i].pw, pArray[1])) //if password matches
					{
						strcpy(client_info[i].ip, inet_ntoa(clnt_adr.sin_addr));
						pthread_mutex_lock(&mutx);
						client_info[i].index = i; 
						client_info[i].fd = clnt_sock;
						clnt_cnt++;
						pthread_mutex_unlock(&mutx);
						sprintf(msg,"[%s] New connected! (ip:%s,fd:%d,sockcnt:%d)\n", pArray[0], inet_ntoa(clnt_adr.sin_addr), clnt_sock, clnt_cnt);
						log_file(msg);
						write(clnt_sock, msg, strlen(msg));

						pthread_create(t_id + i, NULL, clnt_connection, (void *)(client_info + i)); //start thread
						pthread_detach(t_id[i]);
						break;
					}
				}
			}
			if (i == MAX_CLNT) //위 for 문 다 돌은 경우 = 저장되어 있는 id랑 일치하는 것이 없는 경우
			{
				sprintf(msg,"[%s] Authentication Error!\n", pArray[0]);
				write(clnt_sock, msg,strlen(msg));
				log_file(msg);
				shutdown(clnt_sock, SHUT_WR);
			}
		}
		else //read error
			shutdown(clnt_sock, SHUT_WR);
	}
	return 0;
}

void * clnt_connection(void *arg)
{
	CLIENT_INFO * client_info = (CLIENT_INFO *)arg;
	int str_len = 0;
	int index = client_info->index;
	char msg[BUF_SIZE];
	char to_msg[MAX_CLNT * ID_SIZE + 1];
	int i = 0;
	char *pToken;
	char *pArray[ARR_CNT] = {0};
	char strBuff[130] = {0};

	MSG_INFO msg_info;
	CLIENT_INFO	*first_client_info;

	first_client_info = (CLIENT_INFO *)((void *)client_info - (void *)(sizeof(CLIENT_INFO) * index));
	while (1)
	{
		memset(msg, 0x0, sizeof(msg));
		str_len = read(client_info->fd, msg, sizeof(msg) - 1); //연결이 완료된 소켓으로부터 새로운 메시지를 기다리고, 받음
		if (str_len <= 0) //read error
			break;

		msg[str_len] = '\0';
		pToken = strtok(msg, "[:]");

		i = 0; 
		while (pToken != NULL) //pArray에 읽어온 값을 : 으로 split해서 넣는다
		{
			pArray[i] =  pToken;
			if (i++ >= ARR_CNT)
				break;	
			pToken = strtok(NULL,"[:]");
		}

		//보낼 값들 집어넣기
		msg_info.fd = client_info->fd;
		msg_info.from = client_info->id;
		msg_info.to = pArray[0];
		sprintf(to_msg, "[%s]%s", msg_info.from, pArray[1]);
		msg_info.msg = to_msg;
		msg_info.len = strlen(to_msg);

		sprintf(strBuff, "msg : [%s->%s] %s", msg_info.from, msg_info.to, pArray[1]);
		log_file(strBuff);
		send_msg(&msg_info, first_client_info);
	}

	close(client_info->fd);

	sprintf(strBuff, "Disconnect ID:%s (ip:%s,fd:%d,sockcnt:%d)\n", client_info->id, client_info->ip, client_info->fd, clnt_cnt - 1);
	log_file(strBuff);

	pthread_mutex_lock(&mutx);
	clnt_cnt--;
	client_info->fd = -1;
	pthread_mutex_unlock(&mutx);

	return 0;
}

void send_msg(MSG_INFO *msg_info, CLIENT_INFO *first_client_info)
{
	int i = 0;

	if (!strcmp(msg_info->to, "ALLMSG")) //보내는 대상이 ALLMSG
	{
		for (i = 0; i < MAX_CLNT; i++)
		{
			if ((first_client_info + i)->fd != -1)	
				write((first_client_info + i)->fd, msg_info->msg, msg_info->len);
		}
	}
	else if (!strcmp(msg_info->to, "IDLIST")) //보내는 대상이 IDLIST
	{
		char* idlist = (char *)malloc(ID_SIZE * MAX_CLNT);
		msg_info->msg[strlen(msg_info->msg) - 1] = '\0';
		strcpy(idlist, msg_info->msg);

		for (i = 0; i < MAX_CLNT; i++)
		{
			if ((first_client_info+i)->fd != -1)	
			{
				strcat(idlist, (first_client_info + i)->id);
				strcat(idlist, " ");
			}
		}
		strcat(idlist, "\n");
		write(msg_info->fd, idlist, strlen(idlist));
		free(idlist);
	}
	else if (!strcmp(msg_info->to, "GETTIME")) //보내는 대상이 GETTIME
	{
		sleep(1);
		getlocaltime(msg_info->msg);
		write(msg_info->fd, msg_info->msg, strlen(msg_info->msg));
	}
	else //보내는 대상이 특정 클라이언트, 가장 일반적
	{
		for (i = 0; i < MAX_CLNT; i++)
		{
			if ((first_client_info + i)->fd != -1)	//보내려는 클라이언트를 찾는다
			{
				if(!strcmp(msg_info->to, (first_client_info+i)->id))
					write((first_client_info + i)->fd, msg_info->msg, msg_info->len);
			}
		}
	}
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void log_file(char * msgstr)
{
	fputs(msgstr, stdout);
}
void  getlocaltime(char * buf)
{
	struct tm *t;
	time_t tt;
	char wday[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	tt = time(NULL);
	if (errno == EFAULT)
		perror("time()");
	t = localtime(&tt);
	sprintf(buf, "[GETTIME]%02d.%02d.%02d %02d:%02d:%02d %s\n", t->tm_year + 1900 - 2000, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, wday[t->tm_wday]);
	return;
}

