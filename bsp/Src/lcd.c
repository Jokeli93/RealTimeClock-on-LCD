/*
 * lcd.c
 *
 *  Created on: 10.04.2026
 *      Author: Joelikane
 */


#include "stdint.h"
#include "string.h"
#include "lcd.h"

static void write_4_bits(uint8_t value);
static void lcd_enable(void);
static void mdelay(uint32_t cnt);
static void udelay(uint32_t cnt);

void lcd_send_command(uint8_t cmd)
{
	//RS = 0 for LCD command
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	//RnW = 0 when writing to LCD
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//first send the higher nibble of the data (left 4 bits)
	write_4_bits(cmd >> 4);

	//send the lower nibble of the data(right 4 bits)
	write_4_bits(cmd & 0x0F);

	//mdelay(2);

}

/*
 *This function sends a character to the LCD
 *Here we used 4 bit parallel data transmission.
 *First higher nibble of the data will be sent on to the data lines D4,D5,D6,D7
 *Then lower nibble of the data will be set on to the data lines D4,D5,D6,D7
 */
void lcd_print_char(uint8_t data)
{
	//RS = 1 for user data
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);

	//RnW = 0 when writing to LCD
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//first send the higher nibble of the data (left 4 bits)
	write_4_bits(data >> 4);

	//send the lower nibble of the data(right 4 bits)
	write_4_bits(data & 0x0F);

	//mdelay(2);

}

void lcd_print_string(char *message)
{
	while(*message != '\0')
	{
		lcd_print_char((uint8_t)*message);
		message++;
	}
}

void lcd_init(void)
{

	//1. Configure the gpio pins which are used for lcd connections

	GPIO_Handle_t lcd_signal;

	lcd_signal.pGPIOx = LCD_GPIO_PORT;
	lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PinConfig.GPIO_PinOpType = GPIO_OP_TYPE_PP;
	lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;
	GPIO_Init(&lcd_signal);

	//keep all the pins in to logic 0 state initially
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

	//2. LCD initialization (check the reference manual of the HD44780U LCD-H to understand the procedure)

	//wait for more than 40ms after Vcc rises to 2.7 V
	mdelay(40);

	//RS = 0 for LCD command
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	//RnW = 0 when writing to LCD
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//write 0 0 1 1 respectively to DB7 DB6 DB5 DB4 of the LCD
	write_4_bits(0x3);

	//wait for more than 4.1ms
	mdelay(5);

	//write 0 0 1 1 respectively to DB7 DB6 DB5 DB4 of the LCD
	write_4_bits(0x3);

	//wait for more  than 100us
	udelay(150);

	//write 0 0 1 1 respectively to DB7 DB6 DB5 DB4 of the LCD
	write_4_bits(0x3);

	//write 0 0 1 0 respectively to DB7 DB6 DB5 DB4 of the LCD
	write_4_bits(0x2);

	//function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	//display ON and cursor ON
	lcd_send_command(LCD_CMD_DON_CURON);

	//clear display
	lcd_display_clear();

	//entry mode set
	lcd_send_command(LCD_CMD_INCADD);

}

void lcd_display_clear(void)
{
	//Display clear
	lcd_send_command(LCD_CMD_DIS_CLEAR);

	/*
	 * check page number 24 of datasheet.
	 * display clear command execution wait time is around 2ms
	 */

	//2 millisecond delay
	mdelay(2);
}

//Cursor returns to home position
void lcd_display_return_home(void)
{

	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of datasheet.
	 * return home command execution wait time is around 2ms
	 */

	//2 millisecond delay
	mdelay(2);
}

/**
  *   Set Lcd to a specified location given by row and column information
  *   Row Number (1 to 2)
  *   Column Number (1 to 16) Assuming a 2 X 16 characters display
  */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
  column--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      lcd_send_command((column |= 0x80));
      break;
    case 2:
      /* Set cursor to 2nd row address and add index*/
        lcd_send_command((column |= 0xC0));
      break;
    default:
      break;
  }
}


/* writes 4 bits of data/command on to D4,D5,D6,D7 lines */
static void write_4_bits(uint8_t value)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D4, ((value >> 0) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D5, ((value >> 1) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D6, ((value >> 2) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D7, ((value >> 3) & 0x1) );

	//make LCD enable (make a high to low transition on the EN pin)
	lcd_enable();

}

 static void lcd_enable(void)
{
	 GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	 udelay(20);
	 GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	 udelay(100); //instruction execution time > 37 micro seconds
}

 static void mdelay(uint32_t cnt)
 {
 	for(uint32_t i = 0; i < (cnt * 16000); i++);
 }

 static void udelay(uint32_t cnt)
 {
 	for(uint32_t i = 0; i < (cnt * 16); i++);
 }
