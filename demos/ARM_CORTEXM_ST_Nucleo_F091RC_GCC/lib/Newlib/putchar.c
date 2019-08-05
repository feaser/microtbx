/************************************************************************************//**
* \file         demos/ARMCM0_STM32F0_Nucleo_F091RC_GCC/lib/Newlib/putchar.c
* \brief        Transmit character source file.
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
#include <stdint.h>                              /* for standard integer types         */
#include "putchar.h"                             /* putchar module                     */
#include "stm32f0xx.h"                           /* STM32 CPU and HAL header           */
#include "stm32f0xx_ll_usart.h"                  /* STM32 LL USART header              */


/************************************************************************************//**
** \brief     Initializes the communication interface for sending the character.
** \return    none.
**
****************************************************************************************/
void PutCharInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  LL_USART_InitTypeDef USART_InitStruct;

  /* GPIO ports clock enable. */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* UART Peripheral clock enable. */
  __HAL_RCC_USART2_CLK_ENABLE();

  /* UART TX and RX GPIO pin configuration. */
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure UART peripheral */
  USART_InitStruct.BaudRate = 57600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  /* Initialize the UART peripheral */
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_Enable(USART2);
} /*** end of PutCharInit ***/


/************************************************************************************//**
** \brief     Transmits the character on the communication interface.
** \param     ch The value of the character to transmit.
** \return    The value of the transmitted character if successful, -1 otherwise.
**
****************************************************************************************/
int PutCharTransmit(int ch)
{
  int result = ch;
  uint8_t c;

  /* Convert to 8-bit character. */
  c = ch & 0x00FF;

  /* Write byte to transmit holding register */
  LL_USART_TransmitData8(USART2, c);
  /* Wait for tx holding register to be empty */
  while (LL_USART_IsActiveFlag_TXE(USART2) == 0)
  {
    ;
  }

  /* Automatically send carriage return with each newline. */
  if (ch == '\n')
  {
    (void)PutCharTransmit('\r');
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of PutCharTransmit ***/


/*********************************** end of putchar.c **********************************/
