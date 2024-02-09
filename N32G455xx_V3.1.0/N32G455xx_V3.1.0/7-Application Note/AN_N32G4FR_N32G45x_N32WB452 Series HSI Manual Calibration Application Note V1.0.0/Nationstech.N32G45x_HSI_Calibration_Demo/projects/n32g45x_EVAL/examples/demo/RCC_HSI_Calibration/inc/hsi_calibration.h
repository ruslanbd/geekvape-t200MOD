#ifndef __HSI_CALIBRATION_H__
#define __HSI_CALIBRATION_H__

#include "n32g45x.h" 

void HSI_GetOptTrim(uint8_t *opt_value,uint8_t *trim_value);
uint8_t HSI_SetOptTrim(uint8_t opt_type, uint8_t opt_val, uint8_t trim_type, uint8_t trim_val);


#endif /* __HSI_CALIBRATION_H__*/
