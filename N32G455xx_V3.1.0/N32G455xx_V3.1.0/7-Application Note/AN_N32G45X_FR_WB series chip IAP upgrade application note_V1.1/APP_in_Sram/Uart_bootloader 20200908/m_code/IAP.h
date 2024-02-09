#ifndef __IAP_H__
#define __IAP_H__

#include "n32g4fr.h"
                     
#define Sram_buf_len 1024*32 // Maximum receiving is 32K, length is 0x8000
#define Sram_buf_addr (0x20000000 + Sram_buf_len) // app receive buffer start address


typedef void (*iapfun)(void); // Define a function type parameter
void iap_load_app(u32 appxaddr); //Jump to APP program execution
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen); //Start at the specified address, write to bin


#endif
















