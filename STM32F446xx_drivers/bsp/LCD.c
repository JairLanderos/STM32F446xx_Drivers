#include<LCD.h>

static void write_4_bits(uint8_t value);

static void lcd_enable(void);

static void mdelay(uint32_t count);

static void udelay(uint32_t count);

void lcd_send_command(uint8_t command)
{
	/*
	 * 	RS = 0 for LCD command
	 * 	R/nW = 0 to write
	 */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	/* 	Send the higher nibble	*/
	write_4_bits(command >> 4);
	/* 	Send the lower nibble	*/
	write_4_bits(command & 0x0F);
}

void lcd_print_char(uint8_t data)
{
	/*
	 * 	RS = 1 for LCD user data
	 * 	R/nW = 0 to write
	 */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	/* 	Send the higher nibble	*/
	write_4_bits(data >> 4);
	/* 	Send the lower nibble	*/
	write_4_bits(data & 0x0F);
}

void lcd_print_string(char *message)
{
	do
	{
		lcd_print_char((uint8_t)*message++);
	}while (*message != '\0');
}

void lcd_init(void)
{
	//1. Configure the GPIO pins which are used for LCD connections
	GPIO_Handle lcd_signal;

	lcd_signal.pGPIO = LCD_GPIO_PORT;
	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_RS;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_RW;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_EN;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_D4;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_D5;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_D6;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PIN_CONFIG.GPIO_PinNumber = LCD_GPIO_D7;
	GPIO_Init(&lcd_signal);

	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

	//2. Do the LCD initialization
	mdelay(40);

	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	write_4_bits(0x3);

	mdelay(5);

	write_4_bits(0x3);

	udelay(150);

	write_4_bits(0x3);

	write_4_bits(0x2);

	//Function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	//Display and cursor ON
	lcd_send_command(LCD_CMD_DON_CURON);

	//Display clear
	lcd_display_clear();

	//Entry mode set
	lcd_send_command(LCD_CMD_INCADD);

}

static void write_4_bits(uint8_t value)
{
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D4, ( (value >> 0) & 0x1 ) );
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D5, ( (value >> 1) & 0x1 ) );
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D6, ( (value >> 2) & 0x1 ) );
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D7, ( (value >> 3) & 0x1 ) );

	lcd_enable();
}

static void lcd_enable(void)
{
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	udelay(10);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	udelay(100);
}

void lcd_display_clear(void)
{
	lcd_send_command(LCD_CMD_DIS_CLEAR);

	mdelay(2);
}

void lcd_display_return_home(void)
{
	lcd_send_command(LCD_CMD_RETURN_HOME);

	mdelay(2);
}

void lcd_set_cursor(uint8_t row, uint8_t column)
{
	column--;
	switch(row)
	{
	case 1:
		lcd_send_command((column |= 0x80));
		break;
	case 2:
		lcd_send_command((column |= 0xC0));
		break;
	default:
		break;
	}
}

static void mdelay(uint32_t count)
{
	for (uint32_t i = 0; i < (count * 1000); i++);
}

static void udelay(uint32_t count)
{
	for (uint32_t i = 0; i < (count * 1); i++);
}
