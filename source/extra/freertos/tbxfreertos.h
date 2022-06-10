/************************************************************************************//**
* \file         tbxfreertos.h
* \brief        MicroTBX support for FreeRTOS.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2022 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* \endinternal
****************************************************************************************/
#ifndef TBX_FREERTOS_H
#define TBX_FREERTOS_H

/*
 * Include this file at the end of FreeRTOSConfig.h to add support of MicroTBX to
 * FreeRTOS. The current implementation remamps the assertion macro to the one from
 * MicroTBX.
 */

/****************************************************************************************
* Include files
***************************************************************************************/
#include "microtbx.h"


/****************************************************************************************
* Macro definitions
***************************************************************************************/
#ifdef configASSERT
#undef configASSERT
#endif

/** \brief Map the MicroTBX assertions macro to FreeRTOS. */
#define configASSERT(x) TBX_ASSERT(x)


#endif /* TBX_FREERTOS_H */
/*********************************** end of tbxfreertos.h ******************************/