#include "../incs/main.h"

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