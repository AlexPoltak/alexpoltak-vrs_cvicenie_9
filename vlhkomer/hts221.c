/*
 * lps25hb.c
 *
 *  Created on: 17. 11. 2019
 *      Author: Xpoltak
 */

#include "hts221.h"

uint8_t addresHTS221 = HTS221_DEVICE_ADDRESS_0;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addresHTS221, 0));
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addresHTS221, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length,uint8_t flag)
{
	i2c_master_read(data, length, reg, addresHTS221, flag);
}

void  hts221_get_humidity(float *humidity)
{
	/*Read H0_rH, H1_rH */
	float H0_rH  = hts221_read_byte(HTS221_H0_rH_x2)/2;
	float H1_rH  = hts221_read_byte(HTS221_H1_rH_x2)/2;

	/*Read H0_T0_OUT, H1_T0_OUT*/
	uint8_t data[4];
	int16_t H0_T0_OUT, H1_T0_OUT, H_OUT;
	hts221_readArray(data, HTS221_H0_T0_OUT_1, 2,1);
	hts221_readArray(data + 2, HTS221_H1_T0_OUT_1, 2,1);
	H0_T0_OUT = (((uint16_t)data[1])<<8) | (uint16_t)data[0];
	H1_T0_OUT = (((uint16_t)data[3])<<8) | (uint16_t)data[2];

	/*Read H_OUT */
	uint8_t hum_out_l = hts221_read_byte(HTS221_HUMIDITY_OUT_L);
	uint8_t hum_out_h = hts221_read_byte(HTS221_HUMIDITY_OUT_H);
	H_OUT= (int16_t)(hum_out_h << 8) | hum_out_l;

	/*Compute the RH value by linear interpolation*/
	*humidity= (H1_rH-H0_rH)*(H_OUT-H0_T0_OUT)/(H1_T0_OUT-H0_T0_OUT)+H0_rH;
}

uint8_t hts221_init(void)
{

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_ADDRES)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		addresHTS221 = HTS221_DEVICE_ADDRESS_1;
		val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);
		if(val == HTS221_WHO_AM_I_ADDRES)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}


	uint8_t ctrl1 = hts221_read_byte(HTS221_ADDRESS_CTRL1);
	ctrl1 &= ~0xFC;
	ctrl1 |= 0x70;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}
