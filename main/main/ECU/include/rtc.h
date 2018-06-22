/*
 * rtc.h
 *
 * Created: 21/05/2018 17:56:42
 *  Author: BADROUS
 */ 
/*
 The RTC registers are located inaddress locations 00h to 07h. 
 The RAM registers are located in address locations 08h to 3Fh. 
 During a multibyte access, when the address pointer reaches 3Fh, the end of RAM space,
  it wraps around to location 00h, the beginning of the clock space

Bit 7 of Register 0 is the clock halt (CH) bit. When this bit is set to 1, the
oscillator is disabled. When cleared to 0, the oscillator is enabled

Bit 6 of the hours register is defined as the12-hour or 24-hour mode-select bit. 
When high, the 12-hour mode is selected. 

In the 12-hour mode, bit 5 is the AM/PM bit with logic high being PM. 
In the 24-hour mode, bit 5 is the second 10-hour bit (20 to 23 hours).
 
The hours value must be re-entered whenever the 12/24-hour mode bit is changed

When reading or writing the time and date registers, secondary (user) buffers are used to prevent errors 


00H CH 10Seconds Seconds Seconds 00–59
01H 0  10Minutes Minutes Minutes 00–59 12 10 Hour
02H 0 24 PM/AM 10 Hour Hours Hours 1–12 +AM/PM 00–23
03H 0 0 0 0 0 DAY Day 01–07
04H 0 0 10    Date Date Date 01–31
05H 0 0 0 10 Month Month Month 01–12
06H 10 Year Year Year 00–99
07H OUT 0 0 SQWE 0 0 RS1 RS0 Control —
08H-3FH RAM 56 x 8 00H–FFH
support write stream

*/

#ifndef RTC_H_
#define RTC_H_
//--------------------------------------------------------------
#include "i2c_Cfg.h"
//-----------------------------------------------------------------------
#define RTC_ADDRESS 0xD0
#define RTC_TIME_REG 7U
#define RTC_FIRST_REG_ADDRESS 0U
#define RTC_AM 0U
#define RTC_PM 1U
typedef enum
{
	RTC_SECONDS_REG=0,
	RTC_MINUTES_REG	,
	RTC_HOURS_REG,
	RTC_DAY_REG,
	RTC_DATE_REG,
	RTC_MONTH_REG,
	RTC_YEAR_REG,
	RTC_CONTROL_REG
} rtc_reg_t;




//-------------------------------------------------------------------
typedef struct  
{
	u8 first_digit  :4;
	u8 second_digit :3;
	u8 CH_BIT       :1;
} rtc_seconds_t;
//--------------------
typedef union
{
	u8			  raw_data  ;
	rtc_seconds_t bcd_data;
} rtc_seconds;
//----------------------------------------------------------------
typedef struct
{
	u8 first_digit  :4;
	u8 second_digit :3;
} rtc_minutes_t;
//------------------
typedef union
{
	u8			  raw_data  ;
	rtc_minutes_t bcd_data;
} rtc_minutes;
//----------------------------------------------------------------
typedef struct
{
	u8 first_digit  :4;
	u8 second_digit :1;
	u8 SELECT_AM_PM :1;
	u8 SELECT_12_24 :1;
} rtc_12_hours;
//--------------------
typedef struct
{
	u8 first_digit  :4;
	u8 second_digit :2;
	u8 SELECT_12_24 :1;
} rtc_24_hours;
//--------------------
typedef union
{
	rtc_24_hours h24;
	rtc_12_hours h12;
	u8			 raw_data;
} rtc_hours;
//----------------------------------------------------------------
typedef u8 rtc_day;
//----------------------------------------------------------------
typedef struct
{
	u8 first_digit  :4;
	u8 second_digit :2;
} rtc_date_t;
//----------------------
typedef union
{
	u8			  raw_data  ;
	rtc_date_t    bcd_data;
} rtc_date;
//----------------------------------------------------------------
typedef struct
{
	u8 first_digit  :4;
	u8 second_digit :1;
} rtc_month_t;
//-----------------------
typedef union
{
	u8			  raw_data  ;
	rtc_month_t    bcd_data;
} rtc_month;
//----------------------------------------------------------------
typedef struct
{
	u8 first_digit  :4;
	u8 second_digit :4;
} rtc_year_t;
//---------------------
typedef union
{
	u8			   raw_data  ;
	rtc_year_t    bcd_data;
} rtc_year;
//----------------------------------------------------------------
typedef struct
{
	u8 RTC_RS    :2;
	u8 reserved0 :2;
	u8 RTC_SQWE  :1;
	u8 reserved1 :2;
	u8 RTC_OUT  :1;
} rtc_control_reg_t;
//------------------
typedef union
{
	u8			   raw_data  ;
	rtc_control_reg_t    bcd_data;
} rtc_control_reg;

//-------------------------------------------------------------------

typedef enum
{
	RTC_NO_ERROR
} rtc_error_t;
//-----------------------------------------------------------
typedef struct
{
	rtc_seconds seconds;
	rtc_minutes minutes;
	rtc_hours   hours;
    rtc_day     day;
	rtc_date    date;
	rtc_month   month;
	rtc_year    year;
} rtc_time_t;
//------------------------------------
typedef enum
{
	RTC_24_FORMAT=0,
	RTC_12_FORMAT
} rtc_format_t;

//------------------------------------------------------------
rtc_error_t u8Rtc_Init(void);
rtc_error_t u8Rtc_Get_Time(rtc_time_t* time);
rtc_error_t u8Rtc_Set_Time(rtc_time_t* time);
rtc_error_t u8Rtc_Set_reg(u8 data,u8 reg_number);
rtc_error_t u8Rtc_Set_format(rtc_format_t format);
rtc_error_t u8Rtc_get_reg(u8* data,u8 reg_number);
#endif /* RTC_H_ */