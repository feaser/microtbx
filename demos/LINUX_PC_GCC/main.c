/************************************************************************************//**
* \file         demos/LINUX_GCC/main.c
* \brief        Demo program application source file.
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
#include "microtbx.h"                            /* MicroTBX library                   */
#include <stdio.h>                               /* Standard I/O functions.            */
#include <sys/time.h>                            /* Time definitions                   */


/****************************************************************************************
* Function prototypes
****************************************************************************************/
size_t          main(size_t argc, char const * const argv[]);
static void     CustomAssertionHandler(const char * const file, uint32_t line);
static uint32_t CustomSeedInitHandler(void);


/************************************************************************************//**
** \brief     This is the program entry point.
** \param     argc Number of program arguments.
** \param     argv Array with program arguments.
** \return    Program exit code.
**
****************************************************************************************/
size_t main(size_t argc, char const * const argv[])
{
  uint32_t numbers[8];
  size_t   idx;

  TBX_UNUSED_ARG(argc);
  TBX_UNUSED_ARG(argv);

  /* Register the application specific assertion handler. */
  TbxAssertSetHandler(CustomAssertionHandler);
  /* Register the application specific seed initialization handler. */
  TbxRandomSetSeedInitHandler(CustomSeedInitHandler);

  /* Generate some random numbers and print them on the terminal. */
  for (idx = 0; idx < (sizeof(numbers)/sizeof(numbers[0])); idx++)
  {
    /* Get a new random number. */
    numbers[idx] = TbxRandomNumberGet();
    /* Print the value. */ /*lint -e{586} */
    printf("Random number %u: %u.\n", (uint32_t)idx+1U, (uint32_t)numbers[idx]);
  }

  /* Set program exit code. */
  return 0;
} /*** end of main ***/


/************************************************************************************//**
** \brief     Triggers the run-time assertion. The default implementation is to enter an
**            infinite loop, which halts the program and can be used for debugging
**            purposes. Inspecting the values of the file and line parameters gives a
**            clear indication where the run-time assertion occurred. Note that an
**            alternative application specific assertion handler can be configured with
**            function TbxAssertSetHandler().
** \param     file The filename of the source file where the assertion occurred in.
** \param     line The line number inside the file where the assertion occurred.
**
****************************************************************************************/
static void CustomAssertionHandler(const char * const file, uint32_t line)
{
  TBX_UNUSED_ARG(file);
  TBX_UNUSED_ARG(line);

  /* Hang the program by entering an infinite loop. The values for file and line can
   * then be inspected with the debugger to locate the source of the run-time assertion.
   */
  for (;;)
  {
    ;
  }
} /*** end of CustomAssertionHandler ***/


/************************************************************************************//**
** \brief     Handler function that gets called by the random number generator. This
**            module requires a seed, which this function should obtain. The actual value
**            is not really important as long as it is a value that is different every
**            time the software program runs, so after each reset event.
** \details   This example implementation set the seed based on the value of system time.
** \return    The 32-bit value that the random number generator module uses as a seed to
**            initialize itself.
**
****************************************************************************************/
static uint32_t CustomSeedInitHandler(void)
{
  uint32_t result = 0;
  struct timeval tv;

  if (gettimeofday(&tv, NULL) == 0)
  {
    result = (((uint32_t)tv.tv_sec * 1000U) + ((uint32_t)tv.tv_usec / 1000U));
  }
  
  /* Give the result back to the caller. */
  return result;
} /*** end of CustomSeedInitHandler ***/


/*lint -esym(9045, struct itimerval) */
/*lint -esym(9045, struct timezone) */
/*********************************** end of main.c *************************************/
