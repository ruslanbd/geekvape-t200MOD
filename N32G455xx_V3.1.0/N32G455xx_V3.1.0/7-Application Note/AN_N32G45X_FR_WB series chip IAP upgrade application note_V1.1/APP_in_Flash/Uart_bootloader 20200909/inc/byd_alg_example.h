/*!
    \file  byd_alg_example.h
    \brief the header file of byd algorithm example
*/

/*
    Copyright (C) 2019 BYD

    2019-04-12, V1.0.0, demo for byd
*/

#ifndef BYD_ALG_EXAMPLE_H
#define BYD_ALG_EXAMPLE_H

#include "byd_algorithm_api.h"


#define FPS_INPUT_PORT        GPIOA
#define FPS_INPUT_PIN         GPIO_PIN_8
#define FPS_INPUT_EXTI_LINE   EXTI_LINE8
#define FPS_INPUT_PORT_SOURCE GPIOA_PORT_SOURCE
#define FPS_INPUT_PIN_SOURCE  GPIO_PIN_SOURCE8
#define FPS_INPUT_IRQn        EXTI9_5_IRQn

void FPS_InputExtiInit(GPIO_Module* GPIOx, uint16_t Pin);

void byd_alg_enroll(void);
void byd_alg_match(void);
void byd_alg_fixId_match(void);
void byd_alg_delete_id(void);
BOOL byd_alg_delete_all(void);
void byd_alg_read_template_state(void);
void byd_alg_match_delete(void);

#endif
