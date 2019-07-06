/************************************************************************************//**
* \file         tbxassert.c
* \brief        Run-time assertions source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2019  by Feaser    http://www.feaser.com    All rights reserved
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


#ifndef NDEBUG
/************************************************************************************//**
** \brief     Sets the application specific assertion handler.
**
****************************************************************************************/
void TbxAssertSetHandler(void)
{
  /* TODO Implement TbxAssertSetHandler(). It needs a param with the actual handler. */
} /*** end of TbxAssertSetHandler ***/


/************************************************************************************//**
** \brief     Triggers the run-time assertion. The default implementation is to enter an
**            infinite loop, which halts the program and can be used for debugging 
**            purposes. Inspecting the values of the file and line parameters gives a 
**            clear indication where the run-time assertion occured. Note that an
**            alternative application specific assertion handler can be configured with
**            function TbxAssertSetHandler().
** \param     file The filename of the source file where the assertion occurred in.
** \param     line The line number inside the file where the assertion occorred.
**
****************************************************************************************/
void TbxAssertTrigger(const char * const file, uint32_t line)
{
  TBX_UNUSED_ARG(file);
  TBX_UNUSED_ARG(line);

  /* TODO Implement feature that allows the default asserting handling to be overridden
   * with a application specific one that was set with TbxAssertSetHandler().
   */

  /* The default implementation is to enter an infinite loop, which halts the program
   * and can be used for debugging purposes. Inspecting the values of the file and line
   * parameters gives a clear indication where the run-time assertion occured.
   */
  while (1)
  {
    ;
  }
} /*** end of TbxAssertTrigger ***/
#endif /* NDEBUG */


/*********************************** end of tbxassert.c ********************************/
