/************************************************************************************//**
* \file         port/LINUX/tbxport.c
* \brief        Port specifics source file.
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


/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                            /* MicroTBX global header             */
#include <pthread.h>                             /* Posix thread utilities             */
#include <stdbool.h>                             /* Boolean definitions                */
#include <stdatomic.h>                           /* Atomic operations                  */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Value that indicates the the global interrupts are enabled in the simulated
 *         CPU status register.
 */
#define TBX_PORT_CPU_SR_IRQ_EN    (1U)


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Flag that simulates the global interrupt enabled/disabled state. Assume
 *         enabled by default.
 */
static atomic_bool interruptsDisabledFlag = ATOMIC_VAR_INIT(false);

/** \brief Critial section mutex. */
static pthread_mutex_t critSectMutex = PTHREAD_MUTEX_INITIALIZER;


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
  tTbxPortCpuSR result = 0U;
  bool disabledFlagPrev;

  /* Disable the simulated global interrupts by setting its flag to true, and store its
   * previous flag value.
   */ 
  disabledFlagPrev = atomic_flag_test_and_set(&interruptsDisabledFlag);

  /* Were the simulated global interrupts actually enabled, before we just disabled 
   * them? 
   */
  if (disabledFlagPrev == false)
  {
    /* Simulate disabling the global interrupts by locking out other threads. */
    (void)pthread_mutex_trylock(&critSectMutex);
    /* Update the result accordingly. */
    result = TBX_PORT_CPU_SR_IRQ_EN;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxPortInterruptsDisable ***/


/************************************************************************************//**
** \brief     Restores the interrupts enabled/disabled state to the state it was when
**            function TbxPortInterruptsDisable() was previously called. It does this
**            by writing the value of the CPU status register that was returned by
**            TbxPortInterruptsDisable().
** \param     prev_cpu_sr The previous value of the CPU status register from right before
**            the interrupts where disabled. This value is returned by function
**            TbxPortInterruptsDisable().
**
****************************************************************************************/
void TbxPortInterruptsRestore(tTbxPortCpuSR prev_cpu_sr)
{
  /* Should the simulated global interrupts be restored to the enabled state? */
  if (prev_cpu_sr == TBX_PORT_CPU_SR_IRQ_EN)
  {
    /* Simulate enabling the global interrupts by no longer locking out other threads. 
     * Note the this generates a lint warning because a thread mutex is being unlocked, 
     * without being first locked in this function. This is not a problem because this
     * function will only be called after TbxPortInterruptsDisable() is first caleld, 
     * which does the actual thread mutex locking.
     */
    (void)pthread_mutex_unlock(&critSectMutex); /*lint !e455 */  
    /* Enable the simulated global interrupts by setting its flag to false. */
    atomic_flag_clear(&interruptsDisabledFlag);
  }
} /*** end of TbxPortInterruptsRestore ***/


/*********************************** end of tbxport.c **********************************/
