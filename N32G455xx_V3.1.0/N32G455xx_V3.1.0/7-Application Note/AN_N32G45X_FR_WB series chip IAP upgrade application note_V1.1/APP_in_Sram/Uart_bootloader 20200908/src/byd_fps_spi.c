/*!
    \file  byd_fps_spi.c
    \brief 
*/

/*
    Copyright (C) 2019 BYD

    2019-08-26, V1.0.0, app for byd
*/

#include "n32g4fr.h"
#include "byd_fps_api.h"
#include "byd_fps_spi.h"


SPI_Module *FPS_SPI = BYD_FPS_SPI;

/************************************************* ******************************
* Function name: void _delay_ms(unsigned int ms)
* Function description: delay ms
* Input parameters: none
* Return value: None
* Other instructions: accurately implement the delay function according to the actual dsp system clock! ! ! !
* Modified date:
************************************************** *****************************/
void _delay_us(unsigned int us)
{
	unsigned int i,j;
	
	i =  us;
	while(i--)
	{
		for(j = 0; j < 43; j++)
		{
			;
		}
	}
}


void _delay_ms(unsigned int ms)
{
	unsigned int i;
	for(i=0;i<ms;i++)
		_delay_us(1000);
}


/************************************************* ******************************
* Function name: void gpio_config(void)
* Function description: gpio configuration
* Input parameters: none
* Return value: None
* other instructions:
* Modified date:
************************************************** *****************************/
void gpio_config(void)
{	
    GPIO_InitType GPIO_InitStructure;

    /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO
         and sFLASH_SPI_SCK_GPIO Periph clock enable */
    RCC_EnableAPB2PeriphClk(FPS_CS_GPIO_CLK | FPS_SPI_MOSI_GPIO_CLK | FPS_SPI_MISO_GPIO_CLK | FPS_SPI_SCK_GPIO_CLK, ENABLE);

    /*!< sFLASH_SPI Periph clock enable */
    RCC_EnableAPB2PeriphClk(FPS_SPI_CLK, ENABLE);

    /*!< Configure sFLASH_SPI pins: SCK */
    GPIO_InitStructure.Pin        = FPS_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(FPS_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MOSI */
    GPIO_InitStructure.Pin = FPS_SPI_MOSI_PIN;
    GPIO_InitPeripheral(FPS_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MISO */
    GPIO_InitStructure.Pin       = FPS_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(FPS_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
    GPIO_InitStructure.Pin       = FPS_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(FPS_CS_GPIO_PORT, &GPIO_InitStructure);		
}


/************************************************* ******************************
* Function name: void byd_spi_init(uint32_t chipmode)
* Function description: spi configuration
* Input parameters: none
* Return value: None
* Other instructions: The clock source of spi1 and spi2 of gd303 is APB1, APB1=system clock/2
* Modified date:
************************************************** *****************************/
void byd_spi_init(uint32_t chipmode)
{  
    SPI_InitType SPI_InitStructure;

    /*!< Deselect the FLASH: Chip Select high */
    DisableChipSelect();

    /*!< SPI configuration */
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;//SPI_CLKPOL_HIGH;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;//SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;

    //SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_8;
		
#if BYD_SH_METHOD_SEL != 2
        
        if(chipmode == CHIP_TESTMODE)//read offset value is less than or equal to 1M
        {
                SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_128;
        }
        else //Read image is less than or equal to 8M
        {
                SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_8;
        }

#else
        
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_8;//The read image is less than or equal to 8M
        
#endif

    SPI_InitStructure.FirstBit = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly  = 7;
    SPI_Init(FPS_SPI, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Enable(FPS_SPI, ENABLE);
}


/************************************************* ******************************
* Function name: void EnableChipSelect(void)
* Function description: cs configuration is low
* Input parameters: none
* Return value: None
* other instructions:
* Modified date:
************************************************** *****************************/
void EnableChipSelect(void)
{
	  GPIO_ResetBits(FPS_CS_GPIO_PORT, FPS_CS_PIN);
		_delay_us(2);
}
/************************************************* ******************************
* Function name: void DisableChipSelect(void)
* Function description: cs configuration is high
* Input parameters: none
* Return value: None
* other instructions:
* Modified date:
************************************************** *****************************/
void DisableChipSelect(void)
{	
	 GPIO_SetBits(FPS_CS_GPIO_PORT, FPS_CS_PIN);
		_delay_us(2);
}

