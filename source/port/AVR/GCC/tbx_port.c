/************************************************************************************//**
* \file         port/AVR/GCC/tbx_port.c
* \brief        Port specifics source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2023 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
*
* SPDX-License-Identifier: MIT
*
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
#include <avr/interrupt.h>                       /* AVR IRQ management                 */
#include "microtbx.h"                            /* MicroTBX global header             */


/************************************************************************************//**
** \brief     Stores the current state of the CPU status register and then disables the
**            generation of global interrupts. The status register contains information
**            about the interrupts being disable/enabled before they get disabled. This
**            is needed to later on restore the state.
** \return    The current value of the CPU status register.
**
****************************************************************************************/
tTbxPortCpuSR TbxPortInterruptsDisable(void)
{
  tTbxPortCpuSR  result;

  /* Store the current status register. */
  result = SREG;
  /* Request the compiler to complete the previous load/store before continuing. */
  __asm__ volatile ("" ::: "memory");
  /* Clear the I-bit to make sure the global interrupts are disabled. */
  __asm__ __volatile__ ("cli" ::: "memory");

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxPortInterruptsDisable ***/


/************************************************************************************//**
** \brief     Restores the interrupts enabled/disabled state to the state it was when
**            function TbxPortInterruptsDisable() was previously called. It does this
**            by writing the value of the CPU status register that was returned by
**            TbxPortInterruptsDisable().
** \param     prevCpuSr The previous value of the CPU status register from right before
**            the interrupts where disabled. This value is returned by function
**            TbxPortInterruptsDisable().
**
****************************************************************************************/
void TbxPortInterruptsRestore(tTbxPortCpuSR prevCpuSr)
{
  /* Write the status register to restore its previous state. */
  SREG = prevCpuSr;
  /* Request the compiler to complete the previous load/store before continuing. */
  __asm__ volatile ("" ::: "memory");
} /*** end of TbxPortInterruptsRestore ***/


/*********************************** end of tbx_port.c *********************************/
