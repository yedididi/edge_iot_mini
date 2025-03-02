#include "../incs/main.h"

void *bluetoothThread(void *arg)  //server --> bluetooth
{
	t_bluetooth_info *bluetooth_info = (t_bluetooth_info *)arg;
	t_time_info *t = bluetooth_info->time_info;
	char name_msg[NAME_SIZE + BUF_SIZE + 1];

	while (1) 
	{
		memset(name_msg, 0x0, sizeof(name_msg));
		sprintf(name_msg, "%d:%d:%d\n", t->hour, t->minute, t->sec);
		write(bluetooth_info->btfd, name_msg, strlen(name_msg)); //읽은 메시지 블루투스로 전송
		printf("bluetooth fd %d write ok\n", bluetooth_info->btfd);
		sleep(1);
	}
}