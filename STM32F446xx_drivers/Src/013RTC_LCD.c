#include<stdio.h>
#include"DS1307.h"
#include"LCD.h"

#define SYSTICK_TIM_CLK 16000000UL

void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //do some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter
}

char* get_day_of_week(uint8_t i)
{
	char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

	return days[i-1];
}

void number_to_string(uint8_t num, char* buf)
{
	if (num < 10)
	{
		buf[0] = '0';
		buf[1] = num + 48;
	}else if (num >= 10 && num < 99)
	{
		buf[0] = (num / 10) + 48;
		buf[1] = (num % 10) + 48;
	}
}

char* time_to_string(RTC_TIME *rtc_time)
{
	static char buf[9];

	buf[2] = ':';
	buf[5] = ':';

	number_to_string(rtc_time->hours, buf);
	number_to_string(rtc_time->minutes, &buf[3]);
	number_to_string(rtc_time->seconds, &buf[6]);

	buf[8] = '\0';

	return buf;
}

char* date_to_string(RTC_DATE *rtc_date)
{
	static char buf[9];

	buf[2] = '/';
	buf[5] = '/';

	number_to_string(rtc_date->date, buf);
	number_to_string(rtc_date->month, &buf[3]);
	number_to_string(rtc_date->year, &buf[6]);

	buf[8] = '\0';

	return buf;
}

int main(void)
{
	RTC_TIME time;
	RTC_DATE date;


	printf("RTC test\n");

	lcd_init();

	lcd_print_string("RTC Test...");

	if (ds1307_init())
	{
		printf("RTC Init has failed\n");
		while(1);
	}

	init_systick_timer(1);

	date.day = MONDAY;
	date.date = 14;
	date.month = 3;
	date.year = 22;

	time.hours = 3;
	time.minutes = 51;
	time.seconds = 41;
	time.format = TIME_FORMAT_12HRS_PM;

	ds1307_set_current_date(&date);
	ds1307_set_current_time(&time);

	ds1307_get_current_date(&date);
	ds1307_get_current_time(&time);

	char *am_pm;
	if (time.format	!= TIME_FORMAT_24HRS)
	{
		am_pm = (time.format) ? "PM" : "AM";
		printf("Current time = %s %s\n", time_to_string(&time), am_pm);
	}else
	{
		printf("Current time = %s\n", time_to_string(&time));
	}

	printf("Current date = %s <%s>\n", date_to_string(&date), get_day_of_week(date.day));

	while(1);
}

void SysTick_Handler(void)
{
	RTC_TIME time;
	RTC_DATE date;

	ds1307_get_current_date(&date);
	ds1307_get_current_time(&time);

	char *am_pm;
	if (time.format	!= TIME_FORMAT_24HRS)
	{
		am_pm = (time.format) ? "PM" : "AM";
		printf("Current time = %s %s\n", time_to_string(&time), am_pm);
	}else
	{
		printf("Current time = %s\n", time_to_string(&time));
	}

	printf("Current date = %s <%s>\n", date_to_string(&date), get_day_of_week(date.day));
}
