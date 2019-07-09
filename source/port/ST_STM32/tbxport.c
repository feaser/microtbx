/************************************************************************************//**
* \file         tbxport.c
* \brief        Port specifics source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2019 by Feaser     www.feaser.com     All rights reserved
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

/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                            /* MicroTBX global header             */


/************************************************************************************//**
** \brief     Configures a timer driven interrupt that triggers an interrupt each
**            millisecond. The interrupt service routine should call the TbxSystemTick()
**            function, which further handles the system tick event.
** \return    none.
**
****************************************************************************************/
void TbxPortSystemTickInit(void)
{
  /* TODO Implement TbxPortSystemTickInit(). */

  /* TODO Implement framework for the System module with the TbxSystemTick() function. */

  /* TODO Implement the timer ISR which then calls the TbxSystemTick() function. */

  /* TODO Decide on supporting the registration of a tick callback function. For example
   *      for the application to call the HAL's increment tick function. Perhaps not
   *      needed, if this one has an override/weak definition. It could also be an option
   *      to implement such functionality with the timers module, which is planned.
   */
} /*** end of TbxPortSystemTickInit ***/


/*********************************** end of tbxport.c **********************************/
