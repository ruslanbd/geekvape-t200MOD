/*!
    \file  byd_fps_spi.h
    \brief delay define
*/

/*
    Copyright (C) 2020 BYD

    2020-01-16, V1.0.0, app for byd
*/

#include <stdint.h>
#include "n32g4fr_spi.h"

#ifndef BYD_FPS_SPI_H
#define	BYD_FPS_SPI_H


#define BYD_FPS_SPI            SPI1
#define FPS_SPI_CLK            RCC_APB2_PERIPH_SPI1
#define FPS_SPI_SCK_PIN        GPIO_PIN_5 /* PA.05 */
#define FPS_SPI_SCK_GPIO_PORT  GPIOA      /* GPIOA */
#define FPS_SPI_SCK_GPIO_CLK   RCC_APB2_PERIPH_GPIOA
#define FPS_SPI_MISO_PIN       GPIO_PIN_6 /* PA.06 */
#define FPS_SPI_MISO_GPIO_PORT GPIOA      /* GPIOA */
#define FPS_SPI_MISO_GPIO_CLK  RCC_APB2_PERIPH_GPIOA
#define FPS_SPI_MOSI_PIN       GPIO_PIN_7 /* PA.07 */
#define FPS_SPI_MOSI_GPIO_PORT GPIOA      /* GPIOA */
#define FPS_SPI_MOSI_GPIO_CLK  RCC_APB2_PERIPH_GPIOA
#define FPS_CS_PIN             GPIO_PIN_4 /* PA.04 */
#define FPS_CS_GPIO_PORT       GPIOA      /* GPIOA */
#define FPS_CS_GPIO_CLK        RCC_APB2_PERIPH_GPIOA


extern SPI_Module *FPS_SPI;

void _delay_us(unsigned int us);
void _delay_ms(unsigned int ms);
void gpio_config(void);
void byd_spi_init(uint32_t chipmode);
void EnableChipSelect(void);
void DisableChipSelect(void);


#endif
