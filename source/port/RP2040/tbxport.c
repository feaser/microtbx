/************************************************************************************//**
* \file         port/RP2040/tbxport.c
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
#include <hardware/sync.h>                       /* Hardware synchronzation library    */
#include "microtbx.h"                            /* MicroTBX global header             */

/* Only use this port on the Raspberry PI Pico (RP2040) microcontroller, if you actually
 * use multiple cores in your firmware. If you only use one core, the ARM_CORTEXM port is
 * the better choice from a run-time performance perspective.
 */

/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Spin lock handle. */
static volatile spin_lock_t * spinLock = NULL;

/** \brief Flags to keep track if a specific core has the spin lock. */
static volatile uint8_t coreHasLock[NUM_CORES] = { 0 };


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
  tTbxPortCpuSR  result = 0U;
  static uint8_t spinLockInitialized = TBX_FALSE;

  /* Make sure the spin lock handle is initialized. */
  if (spinLockInitialized == TBX_FALSE)
  {
    /* Initialization only needs to be done once. */
    spinLockInitialized = TBX_TRUE;
    /* Initialize the flags. */
    for (uint8_t coreIdx = 0U; coreIdx < NUM_CORES; coreIdx++)
    {
      coreHasLock[coreIdx] = TBX_FALSE;
    }
    /* Initialize the spin lock handle. Start by getting a free lock number. */
    int32_t claimResult = spin_lock_claim_unused(false);
    /* Make sure a spin lock could be claimed. */
    TBX_ASSERT(claimResult != -1);
    /* Only continue with a valid spin lock number. */
    if (claimResult != -1)
    {
      /* Convert the result to the lock number. */
      uint32_t lockNum = (uint32_t)claimResult;
      /* Get handle to the spin lock instance. */
      spinLock = spin_lock_instance(lockNum);
      /* Verify instance handle. */
      TBX_ASSERT(spinLock != NULL);
    }
  }

  /* Only continue with a valid instance handle. */
  if (spinLock != NULL)
  {
    /* This function is called upon every critical section entry, so it can be called
     * when the calling core already has the spin lock. In that case there is no need to
     * obtain it again. In fact, the code shouldn't, because it will deadlock. Therefore
     * only continue if we do not already have the spin lock.
     */
    if (coreHasLock[get_core_num()] == TBX_FALSE)
    {
      /* Wait (spin) the calling core to obtain the spin lock. Basically a multicore
       * hardware mutex. Additionally, disable the interrupts on the calling core, while
       * storing the current state of its CPU status register.
       */
      result = spin_lock_blocking(spinLock);
      /* Set flag that this core now has the lock. Needs to be done after the call to
       * spin_lock_blocking(), because it is a shared resource for each core.
       */
      coreHasLock[get_core_num()] = TBX_TRUE;
    }
  }
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
  /* Only continue with a valid instance handle. */
  if (spinLock != NULL)
  {
    /* Reset the flag now that this core is about to release the lock. Needs to be done
     * before the call to spin_unlock(), because it is a shared resource for each core.
     */
    coreHasLock[get_core_num()] = TBX_FALSE;
    /* Release the spin lock and restore the interrupts on the core that disabled the
     * interrupts previously.
     */
    spin_unlock(spinLock, prevCpuSr);
  }
} /*** end of TbxPortInterruptsRestore ***/


/*********************************** end of tbxport.c **********************************/
