/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __N32G45x_STL_LIB_H
#define __N32G45x_STL_LIB_H

/* Includes ------------------------------------------------------------------*/
#include "n32g45x_STLparam.h"
/* Include Class B variables */
  /* ==============================================================================*/
  /* MISRA violation of rule 8.5 - alocation of variables done via header file jus at this place */
	#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
		#pragma diag_suppress=Pm123
	#endif /* __IAR_SYSTEMS_ICC__ */
#include "n32g45x_STLclassBvar.h"
	#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
		#pragma diag_default=Pm123
	#endif /* __IAR_SYSTEMS_ICC__ */
  /* ==============================================================================*/

/* Self Test library routines main flow after initialization and at run */
#include "n32g45x_STLstartup.h"
#include "n32g45x_STLmain.h"

/* Cortex-M0 CPU test */
#include "n32g45x_STLcpu.h"

/* Clock frequency test */
#include "n32g45x_STLclock.h"

/* Invariable memory test */
#include "n32g45x_STLcrc32.h"

/* Variable memory test */
#include "n32g45x_STLRam.h"



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __n32g45x_STL_LIB_H */

/******************* (C)  *****END OF FILE****/
