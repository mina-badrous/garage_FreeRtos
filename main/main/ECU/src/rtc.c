/*
 * rtc.c
 *
 * Created: 21/05/2018 17:56:54
 *  Author: BADROUS
 */ 
#include "rtc.h"
//------------------------------------------------------------------





//-------------------------------------------------------------------
rtc_error_t u8Rtc_Init(void)
{
	u8 error=0;
	error= u8I2c_Init(&i2c_param);
	//set ch address 0 bit 7 to 0
	
	return (error);
}
//------------------------------------------------------------
rtc_error_t u8Rtc_Get_Time(rtc_time_t* time)
{
	u8 temp[RTC_TIME_REG];
	u8I2c_Start();
	u8I2c_Send_Address(RTC_ADDRESS,I2C_WRITE);
	u8I2c_Send_Data(RTC_FIRST_REG_ADDRESS);
	u8I2c_Restart();
	u8I2c_Send_Address(RTC_ADDRESS,I2C_READ);
	u8I2c_Read_Data(temp,RTC_TIME_REG);
	vI2c_Stop();
	time->seconds.raw_data= temp[0];
	time->minutes.raw_data=temp[1];
	time->hours.raw_data  =temp[2];
	time->day			  =temp[3];
	time->date.raw_data   =temp[4];
	time->month.raw_data  =temp[5];
	time->year.raw_data   =temp[6];
	return 0;
}
//-----------------------------------------------------------------
rtc_error_t u8Rtc_Set_Time(rtc_time_t* time)
{
	u8 temp[RTC_TIME_REG];
	temp[0]=time->seconds.raw_data;
	temp[1]=time->minutes.raw_data;
	temp[2]=time->hours.raw_data;
	temp[3]=time->day;
	temp[4]=time->date.raw_data;
	temp[5]=time->month.raw_data;
	temp[6]=time->year.raw_data;
	u8I2c_Start();
	u8I2c_Send_Address(RTC_ADDRESS,I2C_WRITE);
	u8I2c_Send_Data(RTC_FIRST_REG_ADDRESS);
	u8I2c_Send_Stream(temp,RTC_TIME_REG);
	vI2c_Stop();
	return 0;
}
//-------------------------------------------------------------------
rtc_error_t u8Rtc_Set_format(rtc_format_t format)
{
	//save the hour register
	rtc_hours hour,new_hour;
	u8 first_dig,second_dig;
	u8Rtc_get_reg(&(hour.raw_data),RTC_HOURS_REG);
	//set format
	//if format 12
	if(format ==RTC_12_FORMAT)
	{
		//check current format
		if(hour.h12.SELECT_12_24==RTC_12_FORMAT)
		{
			return 1;
		}
		else
		{
			//convert time to 12 AM-PM FORMAT
			first_dig=hour.h24.first_digit;
			second_dig=hour.h24.second_digit;
			first_dig=first_dig+10*second_dig;
			if(first_dig ==12)
			{
				new_hour.h12.first_digit=2;
				new_hour.h12.second_digit=1;
				new_hour.h12.SELECT_AM_PM=RTC_PM;
				new_hour.h12.SELECT_12_24=RTC_12_FORMAT;
			}
			else if(first_dig >12)
			{
				//subtract 12
				first_dig=first_dig-12;
				second_dig=first_dig/10;
				first_dig=first_dig%10;
				new_hour.h12.first_digit=first_dig;
				new_hour.h12.second_digit=second_dig;
				new_hour.h12.SELECT_AM_PM=RTC_PM;
				new_hour.h12.SELECT_12_24=RTC_12_FORMAT;
											
			}
			else
			{
				//if 0 >>12am
				if(first_dig==0)
				{
					//convert to 12 am
					new_hour.h12.first_digit=2;
					new_hour.h12.second_digit=1;
					new_hour.h12.SELECT_AM_PM=RTC_AM;
					new_hour.h12.SELECT_12_24=RTC_12_FORMAT;
				}
				else
				{
					//if 1>>>11
					second_dig=first_dig/10;
					first_dig=first_dig%10;
					new_hour.h12.first_digit=first_dig;
					new_hour.h12.second_digit=second_dig;
					new_hour.h12.SELECT_AM_PM=RTC_AM;
					new_hour.h12.SELECT_12_24=RTC_12_FORMAT;
				}
											
			}
										
		}
	}
	else
	{
		//if format 24
		if(hour.h24.SELECT_12_24==RTC_24_FORMAT)
		{
			return 1;
		}
		else
		{
			//check am/pm
			if(hour.h12.SELECT_AM_PM ==RTC_AM)
			{
				//case am
				//check if  12
				if((hour.h12.first_digit==2) && (hour.h12.second_digit ==1))
				{
					//convert it to 0
					new_hour.h24.first_digit=0;
					new_hour.h24.second_digit=0;
					new_hour.h24.SELECT_12_24=RTC_24_FORMAT;
				}
				else
				{
					//any number from 1-11
					//the same hour
					new_hour.h24.first_digit=hour.h12.first_digit;
					new_hour.h24.second_digit=hour.h12.second_digit;
					new_hour.h24.SELECT_12_24=RTC_24_FORMAT;
				}
			}
			else
			{
				//case pm
				//check if  12
				if((hour.h12.first_digit==2) && (hour.h12.second_digit ==1))
				{
					//convert it to 12
					new_hour.h24.first_digit=2;
					new_hour.h24.second_digit=1;
					new_hour.h24.SELECT_12_24=RTC_24_FORMAT;
				}
				else
				{
					//any number 1-11
					//increment 12
					first_dig=hour.h12.first_digit;
					second_dig=hour.h12.second_digit;
					first_dig=first_dig+10*second_dig;
					first_dig=first_dig+12;
					second_dig=first_dig/10;
					first_dig=first_dig%10;
					new_hour.h24.first_digit=first_dig;
					new_hour.h24.second_digit=second_dig;
					new_hour.h24.SELECT_12_24=RTC_24_FORMAT;
				}
			}
										
		}
	}
	u8Rtc_Set_reg(new_hour.raw_data,RTC_HOURS_REG);
	return 0;
}
//-------------------------------------------------------------------
rtc_error_t u8Rtc_Set_reg(u8 data,u8 reg_number)
{
		u8I2c_Start();
		u8I2c_Send_Address(RTC_ADDRESS,I2C_WRITE);
		u8I2c_Send_Data(reg_number);
		u8I2c_Send_Data(data);
		vI2c_Stop();
		return 0;
}
//-------------------------------------------------------------------
rtc_error_t u8Rtc_get_reg(u8* data,u8 reg_number)
{
	u8I2c_Start();
	u8I2c_Send_Address(RTC_ADDRESS,I2C_WRITE);
	u8I2c_Send_Data(reg_number);
	u8I2c_Restart();
	u8I2c_Send_Address(RTC_ADDRESS,I2C_READ);
	u8I2c_Read_Data(data,1);
	vI2c_Stop();
	return 0;
}