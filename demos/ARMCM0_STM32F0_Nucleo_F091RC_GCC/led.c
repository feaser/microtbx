/************************************************************************************//**
* \file         demos/ARMCM0_STM32F0_Nucleo_F091RC_GCC/led.c
* \brief        LED driver source file.
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
#include "microtbx.h"                            /* MicroTBX library                   */
#include "led.h"                                 /* LED driver                         */
#include "stm32f0xx.h"                           /* STM32 CPU and HAL header           */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Current state of the LED. TBX_TRUE for ON, TBX_FALSE for off. */
static uint8_t ledState;


/************************************************************************************//**
** \brief     Initializes the LED driver.
** \return    none.
**
****************************************************************************************/
void LedInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Initialize locals. */
  ledState = TBX_FALSE;

  /* GPIO ports clock enable. */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure the LED GPIO pin. */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* turn the LED off */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
} /*** end of LedInit ***/


/************************************************************************************//**
** \brief     Controls the state of the LED.
** \param     on Boolean true to turn the LED on, boolean false for off.
** \return    none.
**
****************************************************************************************/
void LedSet(uint8_t on)
{
  if (on == TBX_TRUE)
  {
    /* turn the LED on */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    ledState = TBX_TRUE;
  }
  else
  {
    /* turn the LED off */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    ledState = TBX_FALSE;
  }
} /*** end of LedSet ***/


/************************************************************************************//**
** \brief     Obtains the current state of the LED.
** \return    Boolean true if the LED is on, boolean false if off.
**
****************************************************************************************/
uint8_t LedGet(void)
{
  return ledState;
} /*** end of LedGet ***/


/************************************************************************************//**
** \brief     Toggles the current state of the LED.
** \return    none.
**
****************************************************************************************/
void LedToggle(void)
{
  uint8_t newState = TBX_TRUE;

  /* Toggle the LED state. */
  if (LedGet() == TBX_TRUE)
  {
    newState = TBX_FALSE;
  }
  LedSet(newState);
} /*** end of LedToggle ***/


/*********************************** end of led.c **************************************/
