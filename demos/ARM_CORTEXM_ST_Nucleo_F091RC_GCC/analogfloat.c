/************************************************************************************//**
* \file         demos/ARMCM0_STM32F0_Nucleo_F091RC_GCC/analogfloat.c
* \brief        Floating analog input driver source file.
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
#include <stdint.h>                              /* for standard integer types         */
#include "analogfloat.h"                         /* floating analog input driver       */
#include "stm32f0xx.h"                           /* STM32 CPU and HAL header           */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Handle for accessing the ADC peripheral with the HAL drivers. */
static ADC_HandleTypeDef adcHandle;


/************************************************************************************//**
** \brief     Initializes the driver for reading a floating analog input.
** \return    none.
**
****************************************************************************************/
void AnalogFloatInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_ChannelConfTypeDef ADC_ChannelConfStruct;


  /* GPIO port and ADC peripheral clock enable. */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();

  /* Configure PA0 (AN0 on the board) as an analog input. */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure the global features of the ADC such as the clock, resolution, data
   * alignment and number of conversions.
   */
  adcHandle.Instance = ADC1;
  adcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  adcHandle.Init.Resolution = ADC_RESOLUTION_12B;
  adcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  adcHandle.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  adcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  adcHandle.Init.LowPowerAutoWait = DISABLE;
  adcHandle.Init.LowPowerAutoPowerOff = DISABLE;
  adcHandle.Init.ContinuousConvMode = DISABLE;
  adcHandle.Init.DiscontinuousConvMode = DISABLE;
  adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  adcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  adcHandle.Init.DMAContinuousRequests = DISABLE;
  adcHandle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  (void)HAL_ADC_Init(&adcHandle);

  /* Configure the ADC regular channel (PA0 = ADC channel 0) to be converted. */
  ADC_ChannelConfStruct.Channel = ADC_CHANNEL_0;
  ADC_ChannelConfStruct.Rank = ADC_RANK_CHANNEL_NUMBER;
  ADC_ChannelConfStruct.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  (void)HAL_ADC_ConfigChannel(&adcHandle, &ADC_ChannelConfStruct);
} /*** end of AnalogFloatInit ***/


/************************************************************************************//**
** \brief     Performs an analog to digital conversion of the floating analog input pin.
** \return    The result of the analog to digital conversion.
**
****************************************************************************************/
uint16_t AnalogFloatGet(void)
{
  uint16_t result = 0;

  /* Start a new analog to digital conversion on the configured channel, which is this
   * case is pin PA0 which is not connected to anything. Such a floating channel will
   * pick up noise, so the conversion results always vary slightly.
   */
  if (HAL_ADC_Start(&adcHandle) == HAL_OK)
  {
    /* Wait for the analog to digital conversion to complete. */
    if (HAL_ADC_PollForConversion(&adcHandle, 10) == HAL_OK)
    {
      /* Store the conversion results. */
      result = HAL_ADC_GetValue(&adcHandle);
    }
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of AnalogFloatGet ***/


/*********************************** end of analogfloat.c ******************************/
