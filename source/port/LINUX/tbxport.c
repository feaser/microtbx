/************************************************************************************//**
* \file         port/LINUX/tbxport.c
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
#include <pthread.h>                             /* Posix thread utilities             */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Flag to determine if the critical section object was already initialized. */
static uint8_t criticalSectionInitialized = TBX_FALSE;

/** \brief Critical section object. Note that it was wrapped in a structure for MISRA
 *         19.2 compliance about not using unions.
 */
static struct
{
  pthread_mutex_t u;
} mtxCritSect;


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
  tTbxPortCpuSR result;

  /* Initialize the result. Note that this value is don't care for this port, as under
   * Linux, the already available PThread Mutex API is used.
   */
  result = 0;
  
  /* Make sure the critical section object is initialized. */
  if (criticalSectionInitialized == TBX_FALSE)
  {
    /* Initialize the critical section object. */
    (void)pthread_mutex_init(&(mtxCritSect.u), NULL);
    /* Set initialized flag. */
    criticalSectionInitialized = TBX_TRUE;
  }

  /* Enter the critical section, if not yet entered. */
  (void)pthread_mutex_trylock(&(mtxCritSect.u));
  
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
  /* The parameter is not used for this port, because under Linux, the already
   * available PThread Mutex API is used. 
   */
  TBX_UNUSED_ARG(prev_cpu_sr);

  /* Make sure the critical section object was initialized. */
  TBX_ASSERT(criticalSectionInitialized == TBX_TRUE);

  /* Leave the critical section. Note the this generates a lint warning because a thread
   * mutex is being unlocked, without being first locked in this function. This is not
   * a problem because this function will only be called after TbxPortInterruptsDisable()
   * is first caleld, which does the actual thread mutex locking.
   */
  (void)pthread_mutex_unlock(&(mtxCritSect.u)); /*lint !e455 */

} /*** end of TbxPortInterruptsRestore ***/


/*********************************** end of tbxport.c **********************************/
