#ifndef __LSE_DRIVER_STRENGTH_CONFIG_H__
#define __LSE_DRIVER_STRENGTH_CONFIG_H__

#include <stdio.h>
#include <stdint.h>
#include "n32g45x.h"

typedef enum {
    LSE_TRIM_LOW=0x01,
    LSE_TRIM_MID=0x02,
    LSE_TRIM_HIGHT=0x03,
}LSE_TRIM_NUM;

uint8_t LSE_ConfigDrvTrim(uint8_t trim_val);
uint8_t LSE_GetNormalTrim(void);
uint8_t LSE_GetSleepTrim(void);

#endif/*__LSE_DRIVER_STRENGTH_CONFIG_H__*/
