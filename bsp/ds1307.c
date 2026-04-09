/*
 * ds1307.c
 *
 *  Created on: 08.04.2026
 *      Author: Joelikane
 */

#include "stdint.h"
#include "string.h"
#include "ds1307.h"


/********************************************************************
 * 						some helper functions
 */
static void ds1307_i2c_pin_config(void);

static void ds1307_i2c_config(void);

static void ds1307_write(uint8_t value, uint8_t reg_addr);

static uint8_t ds1307_read(uint8_t reg_addr);


/********************************************************************/

I2C_Handle_t ds1307I2CHandle;


/*
 * returns 1: CH = 1 ==> init failed
 * returns 0: CH = 0 ==> init success
 */
uint8_t ds1307_init(void)
{
	//1. init the i2c pins
	ds1307_i2c_pin_config();

	//2. initialize the i2c peripheral
	ds1307_i2c_config();

	//3. enable the I2C peripheral
	I2C_PeriClockControl(DS1307_I2C, ENABLE);

	//4. make clock halt = 0
	//Note: for the first time the chip is powered the internal oscillator is automatically disable (CH bit = 1)
	//Enable the clock by clearing the CH bit
	ds1307_write(0x00, DS1307_ADDR_SEC);

	//5. Read back clock halt bit
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

	return ((clock_state >> 7) & 0x1); //we just need the CH bit to check if the init success or failed

}


void ds1307_set_current_time(RTC_time_t *rtc_time)
{

}


void ds1307_get_current_time(RTC_time_t *rtc_time)
{

}


void ds1307_set_current_date(RTC_date_t *rtc_date)
{

}


void ds1307_get_current_date(RTC_date_t *rtc_date)
{

}

static void ds1307_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda,i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	/*
	 * I2C1_SCL ==> PB6
	 * I2C1_SDA ==> PB7
	 */

	i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinaltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOpType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	GPIO_Init(&i2c_sda);


	i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinaltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOpType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	GPIO_Init(&i2c_scl);

}

static void ds1307_i2c_config(void)
{
	ds1307I2CHandle.pI2Cx = DS1307_I2C;
	ds1307I2CHandle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	ds1307I2CHandle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;

	I2C_Init(&ds1307I2CHandle);
}

static void ds1307_write(uint8_t value, uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;
	tx[1] = value;
	I2C_MasterSendData(&ds1307I2CHandle, tx, 2, DS1307_I2C_ADDRESS, 0);

}

static uint8_t ds1307_read(uint8_t reg_addr)
{
	uint8_t data;
    I2C_MasterSendData(&ds1307I2CHandle, &reg_addr, 1, DS1307_I2C_ADDRESS, 0); // here we don't send any data, just initializing the pointer on the rgister
    I2C_MasterReceiveData(&ds1307I2CHandle, &data, 1, DS1307_I2C_ADDRESS, 0);

    return data;

}
