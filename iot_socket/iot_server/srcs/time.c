#include "../incs/main.h"

int isCurrentTimeBig(t_time_info *time_info)
{
    if (time_info->hour > time_info->alarm_hour)
        return (1);
    else if (time_info->hour < time_info->alarm_hour)
        return (-1);
    else //hour is same
    {
        if (time_info->minute > time_info->alarm_minute)
            return (1);
        else if (time_info->minute < time_info->alarm_minute)
            return (-1);
        else
            return (0);
    }
}

t_time_info *updateTime(t_time_info *time_info)
{
    //hour, min, sec update
    time_info->timer = time(NULL);
    time_info->t = localtime(&time_info->timer);
    time_info->hour = time_info->t->tm_hour;
	time_info->minute = time_info->t->tm_min;
	time_info->sec = time_info->t->tm_sec;

    //until_alarm update
    int timeDifference = isCurrentTimeBig(time_info);
    if (timeDifference == 1) //현재시간이 더 큰 경우
    {
        time_info->hour_until_alarm = 23 - time_info->hour + time_info->alarm_hour;
        time_info->minute_until_alarm = 60 - time_info->minute + time_info->alarm_minute;

        if (time_info->minute_until_alarm > 59)
        {
            time_info->minute_until_alarm -= 60;
            time_info->hour_until_alarm += 1;
        }
    }
    else if (timeDifference == -1) //현재시간이 더 작은 경우
    {
        time_info->hour_until_alarm = time_info->alarm_hour - time_info->hour;
        time_info->minute_until_alarm = time_info->alarm_minute - time_info->minute;

        if (time_info->minute_until_alarm < 0)
        {
            time_info->minute_until_alarm += 60;
            time_info->hour_until_alarm--;
        }
    }
    else //같은 경우! 울려야함
    {
        time_info->hour_until_alarm = 0;
        time_info->minute_until_alarm = 0;
    }

    //led, buzzer, motor update

    return (time_info);
}

void printTime(t_time_info *t)
{
    printf("current time is------(%d:%d:%d), remaining time until alarm------(%d:%d)\n", t->hour, t->minute, t->sec, t->hour_until_alarm, t->minute_until_alarm);
}