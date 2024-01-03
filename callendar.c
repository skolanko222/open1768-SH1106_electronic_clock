#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void calc_date_form_epoch(uint64_t epoch, uint8_t date[8], uint8_t time[4])
{
	unsigned char month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	unsigned char ntp_hour, ntp_minute, ntp_second, ntp_week_day, ntp_date, ntp_month, leap_days, leap_year_ind;
	unsigned short temp_days;

	unsigned int ntp_year, days_since_epoch, day_of_year;
	leap_days = 0;
	leap_year_ind = 0;

	ntp_second = epoch % 60;
	epoch /= 60;
	ntp_minute = epoch % 60;
	epoch /= 60;
	ntp_hour = epoch % 24;
	epoch /= 24;

	days_since_epoch = epoch;						

	ntp_year = 1970 + (days_since_epoch / 365);

	int i;
	for (i = 1972; i < ntp_year; i += 4) 
		if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
			leap_days++;

	ntp_year = 1970 + ((days_since_epoch - leap_days) / 365); 
	day_of_year = ((days_since_epoch - leap_days) % 365) + 1;

	if (((ntp_year % 4 == 0) && (ntp_year % 100 != 0)) || (ntp_year % 400 == 0))
	{
		month_days[1] = 29;
		leap_year_ind = 1;
	}
	else
		month_days[1] = 28;

	temp_days = 0;

	for (ntp_month = 0; ntp_month <= 11; ntp_month++)
	{
		if (day_of_year <= temp_days)
			break;
		temp_days = temp_days + month_days[ntp_month];
	}

	temp_days = temp_days - month_days[ntp_month - 1]; 
	ntp_date = day_of_year - temp_days;

	date[0] = ntp_date / 10;
	date[1] = ntp_date % 10;
	date[2] = ntp_month / 10;
	date[3] = ntp_month % 10;
	date[4] = ntp_year / 1000;
	date[5] = (ntp_year % 1000) / 100;
	date[6] = ((ntp_year % 1000) % 100) / 10;
	date[7] = ((ntp_year % 1000) % 100) % 10;

	time[0] = ntp_hour / 10;
	time[1] = ntp_hour % 10;
	time[2] = ntp_minute / 10;
	time[3] = ntp_minute % 10;

}

uint64_t calc_epoch_from_date(uint8_t date[8], uint8_t time[4])
{
	unsigned char month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31,
									30, 31, 30, 31};
	unsigned char ntp_hour, ntp_minute, ntp_second, ntp_week_day, ntp_date, ntp_month, leap_days, leap_year_ind;
	unsigned short temp_days;

	unsigned int ntp_year, days_since_epoch, day_of_year;
	leap_days = 0;
	leap_year_ind = 0;

	ntp_hour = time[0] * 10 + time[1];
	ntp_minute = time[2] * 10 + time[3];
	ntp_second = 0;

	ntp_date = date[0] * 10 + date[1];
	ntp_month = date[2] * 10 + date[3];
	ntp_year = date[4] * 1000 + date[5] * 100 + date[6] * 10 + date[7];

	if (((ntp_year % 4 == 0) && (ntp_year % 100 != 0)) || (ntp_year % 400 == 0))
	{
		month_days[1] = 29; 
		leap_year_ind = 1;	
	}
	else
		month_days[1] = 28; 
	temp_days = 0;

	for (int i = 0; i < ntp_month - 1; i++) 
	{
		temp_days = temp_days + month_days[i];
	}

	temp_days = temp_days + ntp_date; 
	day_of_year = temp_days;

	for (int i = 1972; i < ntp_year; i += 4) 
		if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
			leap_days++;

	days_since_epoch = (ntp_year - 1970) * 365 + leap_days + day_of_year - 1;

	uint64_t epoch = days_since_epoch * 24 * 60 * 60 + ntp_hour * 60 * 60 + ntp_minute * 60 + ntp_second;
	return epoch;
}

int main()
{
	uint8_t date[8] = {3,1,1,2,2,0,2,3};
	uint8_t time[4] = {2,1,3,7};

	uint64_t epoch = calc_epoch_from_date(date, time);
	printf("Epoch: %llu\n", epoch);
	calc_date_form_epoch(epoch, date, time);
	printf("Date: %d%d/%d%d/%d%d%d%d\n", date[0], date[1], date[2], date[3], date[4], date[5], date[6], date[7]);
	printf("Time: %d%d:%d%d\n", time[0], time[1], time[2], time[3]);

	return 0;
}