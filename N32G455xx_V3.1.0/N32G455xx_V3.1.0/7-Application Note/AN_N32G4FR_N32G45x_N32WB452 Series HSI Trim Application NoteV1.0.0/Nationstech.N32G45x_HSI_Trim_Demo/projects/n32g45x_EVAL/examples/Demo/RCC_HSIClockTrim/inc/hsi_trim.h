#ifndef __HSI_TRIM_H__
#define __HSI_TRIM_H__

#include "n32g45x.h" 
uint8_t HSI_GetCurTrim(void);

uint8_t HSI_ConfigTrim(uint8_t trim_type, uint8_t value);

#endif /* __HSI_TRIM_DRV_H__*/
