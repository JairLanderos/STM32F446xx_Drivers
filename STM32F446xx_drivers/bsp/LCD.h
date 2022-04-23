#ifndef LCD_H_
#define LCD_H_

#include"STM32F446xx.h"

/*************************************************************************************************/
/*								APPLICATION CONFIGURABLE ITEMS									 */
/*************************************************************************************************/

#define LCD_GPIO_PORT 	GPIOC
#define LCD_GPIO_RS 	GPIO_PIN_N0
#define LCD_GPIO_RW 	GPIO_PIN_N1
#define LCD_GPIO_EN 	GPIO_PIN_N2
#define LCD_GPIO_D4		GPIO_PIN_N3
#define LCD_GPIO_D5 	GPIO_PIN_N4
#define LCD_GPIO_D6 	GPIO_PIN_N5
#define LCD_GPIO_D7 	GPIO_PIN_N6

/*
 * Commands to control LCD (refer to the device datasheet)
 */
#define LCD_CMD_4DL_2N_5X8F		0x28
#define LCD_CMD_DON_CURON		0x0E
#define LCD_CMD_INCADD			0x06
#define LCD_CMD_DIS_CLEAR		0x01
#define LCD_CMD_RETURN_HOME		0x02

/*************************************************************************************************/
/*									FUNCTION PROTOTYPES											 */
/*************************************************************************************************/

void lcd_init(void);

void lcd_send_command(uint8_t command);

void lcd_print_char(uint8_t data);

void lcd_display_clear(void);

void lcd_display_return_home(void);

void lcd_print_string(char *message);



#endif /* LCD_H_ */
