/*
 * lps25hb.h
 *
 *  Created on: 17. 11. 2019
 *      Author: Xpoltak
 */

#include "main.h"
#include "i2c.h"


#define 	HTS221_DEVICE_ADDRESS_0					0xBE
#define 	HTS221_DEVICE_ADDRESS_1					0xBF

#define 	HTS221_WHO_AM_I_VALUE					0xBC
#define 	HTS221_WHO_AM_I_ADDRES					0x0F

#define 	HTS221_ADDRESS_CTRL1					0x20

 /*HUMIDITY data*/
#define		HTS221_HUMIDITY_OUT_L					0x28
#define		HTS221_HUMIDITY_OUT_H					0x29

/* Temperature output registers */
#define		HTS221_TEMP_OUT_L						0x2A
#define		HTS221_TEMP_OUT_H						0x2B
/* calibration registers */
#define		HTS221_H0_rH_x2							0x30
#define		HTS221_H1_rH_x2							0x31
#define 	HTS221_T0_degC_x8						0x32
#define 	HTS221_T1_degC_x8						0x33
#define 	HTS221_T1_T0_MSB						0x35
#define 	HTS221_H0_T0_OUT_1						0x36
#define 	HTS221_H0_T0_OUT_2						0x37
#define 	HTS221_H1_T0_OUT_1						0x3A
#define 	HTS221_H1_T0_OUT_2						0x3B
#define 	HTS221_T0_OUT_1							0x3C
#define 	HTS221_T0_OUT_2							0x3D
#define 	HTS221_T1_OUT_1							0x3E
#define 	HTS221_T1_OUT_2							0x3F


uint8_t hts221_init(void);
uint8_t hts221_read_byte(uint8_t reg_addr);
void hts221_write_byte(uint8_t reg_addr, uint8_t value);
void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length,uint8_t flag);
