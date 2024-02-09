/*!
    \file  delay.c
    \brief delay app
*/

/*
    Copyright (C) 2019 BYD

    2019-08-26, V1.0.0, app for byd
*/

#include "delay.h"


/*!
    \brief      delay_us
    \param[in]  us
    \param[out] none
    \retval     none
*/
void delay_us(unsigned int us)
{
	unsigned int i,j,count = 10,clk=0;
	
	for(j=0;j<us;j++)
	{
			for(i=0;i<count;i++)
			{
				  
			}
	}
}
/*!
    \brief      delay_ms
    \param[in]  ms
    \param[out] none
    \retval     none
*/
void delay_ms(unsigned int ms)
{
		unsigned int i,j,count = 11650,clk;
		for(j=0;j<ms;j++)
		{
			for(i=0;i<count;i++)
			{
				 
			}
		}
}
