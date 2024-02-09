#include "m_uart.h"
#include "iap.h"
#include "string.h"

iapfun jump2app;  
uint8_t uart_receiveBIN_ok;

uint8_t pages_number = 0;
uint32_t ready_write_addr = 0;

extern uint8_t flash_buf[];
extern uint8_t receive_app_done;
extern void IAP_UPDATE_APP(void);




__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/**============================================== =================
        APP jump
        appxaddr: The starting address of the user code.
================================================== ==============*/
void iap_load_app(u32 appxaddr)
{
    if(((*(vu32*)appxaddr)&0x0FFFFFFF) <1024*512) // Check whether the top address of the stack is legal.
    {
        jump2app = (iapfun)*(vu32*)(appxaddr+4);
        MSR_MSP(*(vu32*)appxaddr); // initialize the stack pointer
        jump2app(); // Jump to APP.
    }
}
/**============================================== =================
================================================== ==============*/
int32_t app_flag_write(uint32_t data ,uint32_t start_add)
{
    FLASH_Unlock();
    //
    FLASH_EraseOnePage(start_add); //Erase once before writing, 2K each time
    if (FLASH_COMPL != FLASH_ProgramWord(start_add, data)) //write
    {
        FLASH_Lock();
        //printf("flash write fail! \r\n");
        return 1;
    }
    FLASH_Lock();
    return 0;
}
/**================================================================
================================================================*/
#define FLASH_PAGE_SIZE 		2048			 	

/**
 * @brief
 * @param void
 * @return
 *-`SUCCESS: indicates that the operation was successful
 *-Other values indicate an error
 */
int32_t app_flash_write(uint32_t *data ,uint32_t Flash_address)
{
    uint32_t i;
    uint32_t start_add;
    start_add = Flash_address;
        
    FLASH_Unlock();
    //
    for(i = 0;i<FLASH_PAGE_SIZE/FLASH_PAGE_SIZE;i++)
    {
        FLASH_EraseOnePage(start_add+i*FLASH_PAGE_SIZE); //Erase once before writing, 2K each time
    }
    //
    for(i=0;i<FLASH_PAGE_SIZE/4;i++)
    {
        if (FLASH_COMPL != FLASH_ProgramWord(start_add+i*4, data[i])) //write
        {
            FLASH_Lock();
            //printf("flash write fail! \r\n");
            receive_app_done = 0;
            return 1;
        }
    }
    FLASH_Lock();
    return 0;
}
/**============================================== =================
        //Upgrade APP
================================================== ==============*/
void IAP_UPDATE_APP(void)
{
    ready_write_addr = FLASH_APP_BASE_ADDR + pages_number*2048;
    //
    while(app_flash_write((uint32_t *)flash_buf, ready_write_addr)); //IAP upgrade 2K each time
    //
    memset(flash_buf,0x00,2048);
    pages_number++;

}













