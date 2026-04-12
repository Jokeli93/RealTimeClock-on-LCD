/*
 * 017rtc_lcd.c
 *
 *  Created on: 10.04.2026
 *      Author: Joelikane
 */


#include<stdio.h>
#include "ds1307.h"
#include "lcd.h"

//extern void initialise_monitor_handles(void);

#define SYSTICK_TIM_CLK		16000000UL

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

    //some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter

}


char* get_day_of_week(uint8_t i)
{
	char* days[] = {"Mon","Tue","Wed","Thu","Fri","Sat", "Sun"};

	return days[i-1];
}

void number_to_string(uint8_t num , char* buf)
{

	if(num < 10){
		buf[0] = '0';
		buf[1] = num+48;
	}else if(num >= 10 && num < 99)
	{
		buf[0] = (num/10) + 48;
		buf[1]= (num % 10) + 48;
	}
}

//time format ==> hh:mm:ss
char* time_to_string(RTC_time_t *rtc_time)
{
	static char buf[9];

	buf[2]= ':';
	buf[5]= ':';

	number_to_string(rtc_time->hours, buf);
	number_to_string(rtc_time->minutes, &buf[3]);
	number_to_string(rtc_time->seconds, &buf[6]);

	buf[8] = '\0';

	return buf;

}

//date format ==> dd/mm/yy
char* date_to_string(RTC_date_t *rtc_date)
{
	static char buf[9];

	buf[2]= '/';
	buf[5]= '/';

	number_to_string(rtc_date->date, buf);
	number_to_string(rtc_date->month, &buf[3]);
	number_to_string(rtc_date->year, &buf[6]);

	buf[8]= '\0';

	return buf;

}

void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 16000); i++);
}


RTC_date_t current_date;
RTC_time_t current_time;

int main(void)
{
	//initialise_monitor_handles();

	//printf("RTC test\r\n");

	lcd_init();

	lcd_print_string("RTC Test...");

	//mdelay(100);

	lcd_display_clear();

	lcd_display_return_home();

	if(ds1307_init())
	{
		printf("RTC init failed\n");
		while(1);
	}

	//initialize the SysTick timer
	init_systick_timer(1);

	current_date.day = SUNDAY;
	current_date.date = 12;
	current_date.month = 4;
	current_date.year = 26;

	current_time.hours = 8;
	current_time.minutes = 00;
	current_time.seconds = 00;
	current_time.time_format = TIME_FORMAT_12HRS_AM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	ds1307_get_current_date(&current_date);
	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";
		//printf("Current time = %s %s\n", time_to_string(&current_time),am_pm); // 06:05:00 PM
		lcd_set_cursor(1, 1);
		lcd_print_string(time_to_string(&current_time));
		lcd_print_char(' ');
		lcd_print_string(am_pm);
	}
	else
	{
		//printf("Current time = %s\n", time_to_string(&current_time)); // 06:05:00
		lcd_set_cursor(1, 1);
		lcd_print_string(time_to_string(&current_time));
	}

	// 10/04/26 <Fri>
	//printf("Current date = %s <%s>\n", date_to_string(&current_date), get_day_of_week(current_date.day));
	lcd_set_cursor(2, 1);
	lcd_print_string(date_to_string(&current_date));
	lcd_print_string(" <");
	lcd_print_string(get_day_of_week(current_date.day));
	lcd_print_char('>');

	while(1);

	return 0;
}


void SysTick_Handler(void)
{
	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";
		//printf("Current time = %s %s\n", time_to_string(&current_time),am_pm); // 06:05:00 PM
		lcd_set_cursor(1, 1);
		lcd_print_string(time_to_string(&current_time));
		lcd_print_char(' ');
		lcd_print_string(am_pm);

	}
	else
	{
		//printf("Current time = %s\n", time_to_string(&current_time)); // 06:05:00
		lcd_set_cursor(1, 1);
		lcd_print_string(time_to_string(&current_time));
	}

	ds1307_get_current_date(&current_date);

	// 10/04/26 <Fri>
	//printf("Current date = %s <%s>\n",date_to_string(&current_date), get_day_of_week(current_date.day));
	lcd_set_cursor(2, 1);
	lcd_print_string(date_to_string(&current_date));
	lcd_print_string(" <");
	lcd_print_string(get_day_of_week(current_date.day));
	lcd_print_char('>');

}
