#include "../incs/main.h"

void serverOpen(char *argv[], pthread_mutex_t mutx, t_time_info *time_info)
{
	t_stm_info		stm_info;
    int serv_sock;
	pthread_t *t_id = 0;
    struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
    int sock_option = 1;
	char msg[BUF_SIZE];
    
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

    printf("server socket is listening...\n");

	socklen_t clnt_adr_sz = sizeof(clnt_adr);
	int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

	pthread_mutex_lock(&mutx);
	stm_info.fd = clnt_sock;
	strcpy(stm_info.ip, inet_ntoa(clnt_adr.sin_addr));
	stm_info.mutx = &mutx;
	stm_info.time_info = time_info;
	pthread_mutex_unlock(&mutx);
	sprintf(msg,"New connected! (ip:%s,fd:%d)\n", inet_ntoa(clnt_adr.sin_addr), clnt_sock);
	log_file(msg);
	write(clnt_sock, msg, strlen(msg));
	pthread_create(t_id, NULL, stm_connection, (void *)&stm_info); //start thread
	pthread_detach(*t_id);
}

void bluetoothConnect(char dest[18], t_time_info *time_info)
{
	t_bluetooth_info *bluetooth_info = (t_bluetooth_info *)malloc(sizeof(t_bluetooth_info));
	pthread_t bluetoothId;
    struct sockaddr_rc addr = { 0 };

    //bluetooth socket generate
	int btfd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(btfd == -1)
	{
		perror("socket()");
		exit(1);
	}

	printf("bluetooth socket ok\n");
	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t)1;
	str2ba(dest, &addr.rc_bdaddr);

	int ret = connect(btfd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret == -1){
		perror("connect()");
		exit(1);
	}

	printf("connect ok\n");
	bluetooth_info->btfd = btfd;
	bluetooth_info->time_info = time_info;

	printf("before bt thread\n");
	pthread_create(&bluetoothId, NULL, bluetoothThread, (void *)bluetooth_info);
	pthread_detach(bluetoothId);

	printf("bluetooth done\n");
}