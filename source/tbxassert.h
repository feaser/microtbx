/************************************************************************************//**
* \file         tbxassert.h
* \brief        Run-time assertions header file.
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
#ifndef TBXASSERT_H
#define TBXASSERT_H

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************************
* Macro definitions
****************************************************************************************/
#if (TBX_ASSERTIONS_ENABLE > 0u)
/** \brief Macro for run-time assertions. */
#define TBX_ASSERT(cond) { if(!(cond)) { TbxAssertTrigger(__FILE__, __LINE__); } }
#else
/** \brief Dummy macro for when assertions are disabled. */
#define TBX_ASSERT(cond) { ; }
#endif /* (TBX_ASSERTIONS_ENABLE > 0u) */


#if (TBX_ASSERTIONS_ENABLE > 0u)
/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Function type for a run-time assertion handler function. */
typedef void (* tTbxAssertHandler)(const char * const file, uint32_t line);


/****************************************************************************************
* Function prototypes
****************************************************************************************/
void TbxAssertSetHandler(tTbxAssertHandler assertHandler);
void TbxAssertTrigger(const char * const file, uint32_t line);
#endif /* (TBX_ASSERTIONS_ENABLE > 0u) */


#ifdef __cplusplus
}
#endif

#endif /* TBXASSERT_H */
/*********************************** end of tbxassert.h ********************************/
