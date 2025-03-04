#include "../incs/main.h"

int main(int argc, char *argv[])
{
	pthread_mutex_t mutx;
	char dest[18] = "98:DA:60:0D:B3:F3";	//iot12
	// char dest[18] = "98:DA:60:0D:A7:E0";	//iot94
	t_time_info *time_info = 0;

	time_info = initMain(argc, argv, &mutx);
	serverOpen(argv, mutx, time_info);
	bluetoothConnect(dest, time_info);

	while (1)
	{
		time_info = updateTime(time_info);
		printTime(time_info);
		sleep(1);
	}
	return 0;
}
