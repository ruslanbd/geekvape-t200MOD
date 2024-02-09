#ifndef __IAP_H__
#define __IAP_H__

#include "n32g4fr.h"
                     
                             
#define FLASH_APP_BASE_ADDR 0x08004000 //BOOTLOAD reserves 16K space, 
                                       //APP program starts from 0x08004000
#define FLASH_START_ADDR FLASH_APP_BASE_ADDR


typedef void (*iapfun)(void); // Define a function type parameter.
void iap_load_app(u32 appxaddr); //Jump to APP program execution
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen); //Start at the specified address, write to bin


#endif
















