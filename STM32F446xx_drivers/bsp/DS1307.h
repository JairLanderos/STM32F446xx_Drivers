#ifndef DS1307_H_
#define DS1307_H_

#include"STM32F446xx.h"

/*************************************************************************************************/
/*								APPLICATION CONFIGURABLE ITEMS									 */
/*************************************************************************************************/

#define DS1307_I2C				I2C1
#define DS1307_I2C_GPIO_PORT	GPIOB
#define DS1307_I2C_SDA			GPIO_PIN_N7
#define DS1307_I2C_SCL			GPIO_PIN_N6
#define DS1307_I2C_SPEED		I2C_SPEED_SM
#define DS1307_I2C_PUPD			GPIO_PU


/*************************************************************************************************/
/*									REGISTER ADDRESSES											 */
/*************************************************************************************************/

#define DS1307_ADDR_SEC			0x00
#define DS1307_ADDR_MIN			0x01
#define DS1307_ADDR_HRS			0x02
#define DS1307_ADDR_DAY			0x03
#define DS1307_ADDR_DATE		0x04
#define DS1307_ADDR_MONTH		0x05
#define DS1307_ADDR_YEAR		0x06

#define TIME_FORMAT_12HRS_AM 	0
#define TIME_FORMAT_12HRS_PM 	1
#define TIME_FORMAT_24HRS		2

#define DS1307_I2C_ADDRESS		0x68

#define SUNDAY 		1
#define MONDAY 		2
#define TUESDAY 	3
#define WEDNESDAY 	4
#define THURSDAY 	5
#define FRIDAY 		6
#define SATURDAY 	7


/*************************************************************************************************/
/*										DATA STRUCTURES											 */
/*************************************************************************************************/

typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
}RTC_DATE;

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t format;
}RTC_TIME;


/*************************************************************************************************/
/*									FUNCTION PROTOTYPES											 */
/*************************************************************************************************/

	//Initialization
uint8_t ds1307_init(void);

	//Set and get the current time
void ds1307_set_current_time(RTC_TIME *p);
void ds1307_get_current_time(RTC_TIME *p);

//Set and get the current date
void ds1307_set_current_date(RTC_DATE *p);
void ds1307_get_current_date(RTC_DATE *p);


#endif /* DS1307_H_ */
