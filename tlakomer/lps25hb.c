 /*
 * lps25hb.c
 *
 *  Created on: 17. 11. 2019
 *      Author: Xpoltak
 */

#include "lps25hb.h"
#include <math.h>

uint8_t addresLPS25HB = LPS25HB_DEVICE_ADDRESS_0;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addresLPS25HB, 0));
}


void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addresLPS25HB, 0);
}


void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length,uint8_t flag)
{
	i2c_master_read(data, length, reg, addresLPS25HB, flag);
}

void  lps25hb_get_temperature(float *temperature)
{
	const float  scale = 100.0;
	uint8_t data[2];
	lps25hb_readArray(data, LPS25HB_TEMP_OUT_L, 2, 1);

	*temperature = ((int16_t)(data[1] << 8) | data[0])/scale;
}
void  lps25hb_get_pressure(float *pressure)
{
	uint8_t data_H, data_L, data_XL;

	data_XL=lps25hb_read_byte(LPS25HB_PRESS_OUT_XL);
	data_L=lps25hb_read_byte(LPS25HB_PRESS_OUT_L);
	data_H=lps25hb_read_byte(LPS25HB_PRESS_OUT_H);

	*pressure=((data_H << 16) | (data_L << 8) | data_XL)/4096;
}
void lps25hb_get_altitude(float *altitude)
{
	const float  P0 = 1013.25;
	float pressure = 0.0;
	float temperature = 0.0;


	lps25hb_get_pressure(&pressure);
	lps25hb_get_temperature(&temperature);

	*altitude = (pow(P0/pressure,1/5.257)*(temperature+273.15))/0.0065;
}



uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		addresLPS25HB = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, 0b01010000);
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL2, 0b00010000);

	return status;
}
