#include "../incs/main.h"

t_time_info *initMain(int argc, char *argv[], pthread_mutex_t *mutx)
{
    if (argc != 2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	fputs("IoT Server Start!!\n", stdout);

	if (pthread_mutex_init(mutx, NULL))
		error_handling("mutex init error");

	t_time_info *time_info = (t_time_info *)malloc(sizeof(t_time_info));
	if (time_info == NULL)
	{
		printf("malloc error\n");
		return NULL;
	}
	
	time_info->timer = time(NULL);
	time_info->t = localtime(&time_info->timer);
	time_info->hour = time_info->t->tm_hour;
	time_info->minute = time_info->t->tm_min;
	time_info->sec = time_info->t->tm_sec;
	time_info->alarm_hour = time_info->t->tm_hour;
	time_info->alarm_minute = time_info->t->tm_min + 1;
	time_info->hour_until_alarm = 0;
	time_info->minute_until_alarm = 0;
	time_info->led = 0;
	time_info->buzzer = 0;
	time_info->motor = 0;
	time_info->buttonPressed = 0;
	return (time_info);
}