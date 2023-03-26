;****************************************************************************************
;* \file         port/ARM_CORTEXM/IAR/tbx_comp.s
;* \brief        IAR compiler specifics of the port source file.
;* \internal
;*---------------------------------------------------------------------------------------
;*                          C O P Y R I G H T
;*---------------------------------------------------------------------------------------
;*   Copyright (c) 2019 by Feaser     www.feaser.com     All rights reserved
;*
;*---------------------------------------------------------------------------------------
;*                            L I C E N S E
;*---------------------------------------------------------------------------------------
;*
;* SPDX-License-Identifier: MIT
;*
;* Permission is hereby granted, free of charge, to any person obtaining a copy
;* of this software and associated documentation files (the "Software"), to deal
;* in the Software without restriction, including without limitation the rights
;* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;* copies of the Software, and to permit persons to whom the Software is
;* furnished to do so, subject to the following conditions:
;*
;* The above copyright notice and this permission notice shall be included in all
;* copies or substantial portions of the Software.
;*
;* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;* SOFTWARE.
;*
;* \endinternal
;****************************************************************************************


        PUBLIC  TbxPortInterruptsDisable
        PUBLIC  TbxPortInterruptsRestore


        SECTION .text:CODE:NOROOT(2)
        THUMB

        
        PUBWEAK TbxPortInterruptsDisable
TbxPortInterruptsDisable
        ; Store state of the currently enabled/disabled interrupts in register 0. On the 
        ; ARM Cortex this is stored in CPU register PRIMASK. Note that register 0 is used
        ; by GCC for storing the return value of a function.
        MRS   R0, PRIMASK
        ; Disable the interrupts by setting all bits in the CPU register PRIMASK to a
        ; value of 1, which means the actication of the exception is prevented.
        CPSID I
        ; Return from this function by branching back to the location stored in the link
        ; register.
        BX    LR


        PUBWEAK TbxPortInterruptsRestore
        THUMB
TbxPortInterruptsRestore
        ; Restore the state of the interrupts by storing the value of register 0 in the
        ; CPU register PRIMASK. Note that register 0 is used by GCC for passing the first
        ; function parameter.
        MSR   PRIMASK, R0
        ; Return from this function by branching back to the location stored in the link
        ; register.
        BX    LR
        
      
        END
;*********************************** end of tbx_comp.s **********************************
