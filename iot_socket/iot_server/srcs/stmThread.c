#include "main.h"

void	*stm_connection(void *arg)
{
	(void)arg;
	return (0);
	// t_client_info *client_info = (t_client_info *)arg;
	// int str_len = 0;
	// char msg[BUF_SIZE];
	// char to_msg[MAX_CLNT * ID_SIZE + 1];
	// int i = 0;
	// char *pToken;
	// char *pArray[ARR_CNT] = {0};
	// char strBuff[130] = {0};

	// t_msg_info msg_info;
	
	// while (1)
	// {
	// 	memset(msg, 0x0, sizeof(msg));
	// 	str_len = read(client_info->fd, msg, sizeof(msg) - 1); //연결이 완료된 소켓으로부터 새로운 메시지를 기다리고, 받음
	// 	if (str_len <= 0) //read error
	// 		break;

	// 	msg[str_len] = '\0';
	// 	pToken = strtok(msg, "[:]");

	// 	i = 0; 
	// 	while (pToken != NULL) //pArray에 읽어온 값을 : 으로 split해서 넣는다
	// 	{
	// 		pArray[i] =  pToken;
	// 		if (i++ >= ARR_CNT)
	// 			break;	
	// 		pToken = strtok(NULL,"[:]");
	// 	}

	// 	//보낼 값들 집어넣기
	// 	msg_info.fd = client_info->fd;
	// 	sprintf(to_msg, "%s", pArray[1]);
	// 	msg_info.msg = to_msg;
	// 	msg_info.len = strlen(to_msg);

	// 	sprintf(strBuff, "msg : %s", pArray[1]);
	// 	log_file(strBuff);
	// 	// send_msg(&msg_info, first_client_info);
	// 	write(client_info->fd, msg_info.msg, msg_info.len);
	// }

	// close(client_info->fd);

	// sprintf(strBuff, "Disconnect (ip:%s,fd:%d)\n", client_info->ip, client_info->fd);
	// log_file(strBuff);

	// pthread_mutex_lock(client_info->mutx);
	// client_info->fd = -1;
	// pthread_mutex_unlock(client_info->mutx);

	// return 0;
}


// void send_msg(t_msg_info *msg_info, t_client_info *first_client_info)
// {
// 	int i = 0;

// 	if (!strcmp(msg_info->to, "ALLMSG")) //보내는 대상이 ALLMSG
// 	{
// 		for (i = 0; i < MAX_CLNT; i++)
// 		{
// 			if ((first_client_info + i)->fd != -1)	
// 				write((first_client_info + i)->fd, msg_info->msg, msg_info->len);
// 		}
// 	}
// 	else if (!strcmp(msg_info->to, "IDLIST")) //보내는 대상이 IDLIST
// 	{
// 		char* idlist = (char *)malloc(ID_SIZE * MAX_CLNT);
// 		msg_info->msg[strlen(msg_info->msg) - 1] = '\0';
// 		strcpy(idlist, msg_info->msg);

// 		for (i = 0; i < MAX_CLNT; i++)
// 		{
// 			if ((first_client_info+i)->fd != -1)	
// 			{
// 				strcat(idlist, (first_client_info + i)->id);
// 				strcat(idlist, " ");
// 			}
// 		}
// 		strcat(idlist, "\n");
// 		write(msg_info->fd, idlist, strlen(idlist));
// 		free(idlist);
// 	}
// 	else if (!strcmp(msg_info->to, "GETTIME")) //보내는 대상이 GETTIME
// 	{
// 		sleep(1);
// 		getlocaltime(msg_info->msg);
// 		write(msg_info->fd, msg_info->msg, strlen(msg_info->msg));
// 	}
// 	else //보내는 대상이 특정 클라이언트, 가장 일반적
// 	{
// 		for (i = 0; i < MAX_CLNT; i++)
// 		{
// 			if ((first_client_info + i)->fd != -1)	//보내려는 클라이언트를 찾는다
// 			{
// 				if(!strcmp(msg_info->to, (first_client_info+i)->id))
// 					write((first_client_info + i)->fd, msg_info->msg, msg_info->len);
// 			}
// 		}
// 	}
// }