#include "main.h"

void	*stm_connection(void *arg)
{
	char msg[BUF_SIZE];
	printf("start stm thread\n");
	t_stm_info *stm_info = (t_stm_info *)arg;
	printf("stm thread, first fd:%d\n", stm_info->fd);

	while (1)
	{
		// int retWrite = write(stm_info->fd, "[LYJ]LED@ON", strlen("[LYJ]LED@ON"));
		// if (retWrite)
		// 	printf("write ok, wrote %d, [LYJ]LED@ON\n", retWrite);
		// sleep(5);
		// retWrite = write(stm_info->fd, "[LYJ]LED@OFF", strlen("[LYJ]LED@OFF"));
		// if (retWrite)
		// 	printf("write ok, wrote %d, [LYJ]LED@OFF\n", retWrite);
		// sleep(5);

		if (stm_info->time_info->hour_until_alarm == 0 && stm_info->time_info->minute_until_alarm <= 10)
		{
			// char msg[BUF_SIZE];
			pthread_t buttonId;
			pthread_create(&buttonId, NULL, buttonThread, (void *)stm_info);
			pthread_detach(buttonId);

			while (1)
			{
				memset(msg, 0x0, BUF_SIZE);
				if (stm_info->time_info->buttonPressed)
				{
					stm_info->time_info->led = 0;
					stm_info->time_info->buzzer = 0;
					stm_info->time_info->motor = 0;
					sprintf(msg, "LED@%d@buzzer@%d@motor@%d\n", 0, 0, 0);
					write(stm_info->fd, msg, strlen(msg));
					break;
				}

				stm_info->time_info->led = 10 - stm_info->time_info->minute_until_alarm;
				if (stm_info->time_info->led)
					stm_info->time_info->led = 1;
				else if (stm_info->time_info->minute_until_alarm == 0)
					stm_info->time_info->led = 1;

				if (stm_info->time_info->buzzer)
					stm_info->time_info->buzzer = 1;
				else if (stm_info->time_info->minute_until_alarm == 0)
					stm_info->time_info->buzzer = 1;

				if (stm_info->time_info->motor)
					stm_info->time_info->motor = 1;
				else if (stm_info->time_info->minute_until_alarm == 0)
					stm_info->time_info->motor = 1;

				sprintf(msg, "LED@%d@buzzer@%d@motor@%d\n", stm_info->time_info->led, stm_info->time_info->buzzer, stm_info->time_info->motor);
				write(stm_info->fd, msg, strlen(msg));
				sleep(1);
			}
		}
		memset(msg, 0x0, BUF_SIZE);
		sprintf(msg, "LED@%d@buzzer@%d@motor@%d\n", 0, 0, 0);
		write(stm_info->fd, msg, strlen(msg));
		sleep(1);
	}
	return (0);
}

void *buttonThread(void *arg)
{
	t_stm_info *stm_info = (t_stm_info *)arg;
	char readBuf[BUF_SIZE];
	
	while (1)
	{
		int readRet = read(stm_info->fd, readBuf, BUF_SIZE);
		if (readRet > 0)
		{
			if (strncmp(readBuf, "BUTTON_PRESSED", readRet) == 0)
			{
				stm_info->time_info->buttonPressed = 1;
				break;
			}
		}	
		else
			printf("read error\n");
	}
	return (NULL);
}
