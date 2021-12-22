/*
 * lps25hb.h
 *
 *  Created on: 17. 11. 2019
 *      Author: Xpoltak
 */

#include "main.h"
#include "i2c.h"


#define 	LPS25HB_DEVICE_ADDRESS_0				0xB8
#define 	LPS25HB_DEVICE_ADDRESS_1				0xBA

#define 	LPS25HB_WHO_AM_I_VALUE					0xBD
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F


 /*pressure output registers*/
#define		LPS25HB_PRESS_OUT_XL					0x28
#define		LPS25HB_PRESS_OUT_L						0x29
#define		LPS25HB_PRESS_OUT_H						0x2A

/* Temperature output registers */
#define		LPS25HB_TEMP_OUT_L						0x2B
#define		LPS25HB_TEMP_OUT_H						0x2C


#define 	LPS25HB_ADDRESS_CTRL1					0x20


#define 	LPS25HB_ADDRESS_TEMP_L					0x20

uint8_t lps25hb_init(void);
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);
void  lps25hb_get_pressure(float *pressure);
