/*!
    \file  byd_alg_example.c
    \brief byd algorithm example
*/

/*
    Copyright (C) 2019 BYD

    2019-06-26, V1.0.0, demo for byd
*/
#include "byd_alg_example.h"
#include "delay.h"


uint8_t m_FPS_INT_Flag = 0;


void FPS_InputExtiInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }
    else if (GPIOx == GPIOC)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
    }
    else if (GPIOx == GPIOD)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);
    }
    else if (GPIOx == GPIOE)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOE, ENABLE);
    }
    else if (GPIOx == GPIOF)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOF, ENABLE);
    }
    else
    {
        if (GPIOx == GPIOG)
        {
            RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOG, ENABLE);
        }
    }

    /*Configure the GPIO pin as input floating*/
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStructure.Pin        = Pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
    /*Configure FPS EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(FPS_INPUT_PORT_SOURCE, FPS_INPUT_PIN_SOURCE);

    /*Configure FPS EXTI line*/
    EXTI_InitStructure.EXTI_Line    = FPS_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set FPS input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = FPS_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void EXTI9_5_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(FPS_INPUT_EXTI_LINE))
    {
			  m_FPS_INT_Flag = 1;
        EXTI_ClrITPendBit(FPS_INPUT_EXTI_LINE);
    }
}

uint8_t Enrollcnt = 0;
extern uint8_t fp_mode;

//Enter
void byd_alg_enroll(void)
{
	uint32_t ret;
	BOOL rettemp;
//	uint8_t Enrollcnt;
	FPS_enroll_return_struct Enroll_ret;
	uint8_t ucSetEnrollPara = 0x02;
	FPS_ReadImg_return_struct ReadImgRet;
	
//    while(1)
//    {
	if(m_FPS_INT_Flag == 1)
	{
		m_FPS_INT_Flag = 0;
		ret = Finger_GetIntState(FALSE);
		if(1 == ret)
		{				  		 
			ReadImgRet = Finger_ReadImage((uint8_t*)ptImageData, ((Enrollcnt<<1)|1));
						
			if(ReadImgRet.Result == TRUE)
			{		
				if(Enrollcnt == 0)
				{
					Enroll_ret = Finger_Enroll(TRUE,0xff,ucSetEnrollPara);				
				}
				else
				{
					Enroll_ret = Finger_Enroll(FALSE,0xff,ucSetEnrollPara);					
				}
				if(Enroll_ret.FingerResult == TRUE)
				{
					Enrollcnt++;
					if(Enrollcnt == BYD_FPS_MAX_ENROLL_NUM)
					{
						Enrollcnt = 0;
						fp_mode = 0x02;
						printf("fp enroll success! \r\n");
						printf("fp enter match mode! \r\n");
						//break;
					}
					else
					{
						printf("Present count = %d , please continue press... \r\n",Enrollcnt);
					}
				}							
			}	 
		}				 
	}
			
			delay_ms(100);
//	}		
}




//Comparison
void byd_alg_match(void)
{
	uint32_t ret;
	BOOL rettemp;
	BOOL BResult;
	FPS_match_return_struct MatchRet;	
    FPS_ReadImg_return_struct ReadImgRet;
// while(1)
// {
    if(m_FPS_INT_Flag == 1)
    {
        m_FPS_INT_Flag = 0;
        ret = Finger_GetIntState(TRUE);
        if(1 == ret)
        {
            ReadImgRet = Finger_ReadImage((uint8_t*)ptImageData, 0);
            if(ReadImgRet.Result == TRUE)
            {
                MatchRet = Finger_Match(0xffff,0x01,0x00);
                if(MatchRet.Result == TRUE)
                {
                    BResult = Finger_TempletUpdate(1); //Template update
                    if(BResult == TRUE)
                    {
                                
                    }
                    printf("FP match success\r\n");
                    //break;
                }
                else
                {
                    printf("FP_match fail\r\n");
                }
            }
        }
    }
//}
}

//Specify ID comparison
void byd_alg_fixId_match(void)
{
	 uint32_t ret;
	 BOOL rettemp;
	 BOOL BResult;
	 FPS_match_return_struct MatchRet;	
   FPS_ReadImg_return_struct ReadImgRet;
   while(1)
     {
            if(m_FPS_INT_Flag == 1)
            {
                 m_FPS_INT_Flag = 0;
                 ret = Finger_GetIntState(TRUE);
                 if(1 == ret)
                 {
                     ReadImgRet = Finger_ReadImage((uint8_t*)ptImageData, 0);
                     if(ReadImgRet.Result == TRUE)
                     {
                         MatchRet = Finger_Match(0x00,0x03,0x00);//Specify 3 templates starting from id0 for comparison
                         if(MatchRet.Result == TRUE)
                         {
                                BResult = Finger_TempletUpdate(1); //Template update
                                if(BResult == TRUE)
                                {
                                    
                                }
                                break;
                         }
                     }
                 }
        }
     }
}

//Read the existing fingerprint and delete the specified ID
void byd_alg_delete_id(void)
{
	 uint8_t ucTmpValue,i;
	 FPS_DeleteFixId_return_struct DeleteRet;
	 FPS_IdRead_return_struct IDReadRst;
	
	 IDReadRst = Finger_IdRead();
	 if(IDReadRst.ExistIdNum > 0)
	 {
		 for(i = 0; i < 8; i++)
		 {
			 ucTmpValue = (IDReadRst.ExistId[0]>>i)&0x01;
			 if(ucTmpValue == 1)
			 {
				 DeleteRet = Finger_DeleteFixId(i);
				 if(DeleteRet.Result == TRUE)
				 {
						break;		 
				 }			 
			 }
	   }		 
	 }	
}

//Empty the fingerprint library
BOOL byd_alg_delete_all(void)
{
	  BOOL BResult;
		BResult = Finger_DeleteAll();
	  return BResult;
}

//Query the template status of the specified ID
void byd_alg_read_template_state(void)
{
	 FPS_TemplateSates TempState;
	 TempState = Finger_QueryTemplateState(0x01);
}

//First compare to find the corresponding ID, and then delete
void byd_alg_match_delete(void)
{		
	 uint32_t ret;
	 FPS_delete_return_struct MatchDelet;
	
   while(1)
	 {
			if(m_FPS_INT_Flag == 1)
			{
				 m_FPS_INT_Flag = 0;		 
				 ret = Finger_GetIntState(TRUE);
				 if(1 == ret)
				 {	
					 MatchDelet = Finger_Delete();
					 if(MatchDelet.Result == TRUE)
					 {
							break;						
					 } 
				 }
		 }
	 }	
}

