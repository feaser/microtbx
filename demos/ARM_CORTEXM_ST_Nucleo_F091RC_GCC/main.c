/************************************************************************************//**
* \file         demos/ARMCM0_STM32F0_Nucleo_F091RC_GCC/main.c
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
#include <stdio.h>                               /* C standard input/output            */
#include "microtbx.h"                            /* MicroTBX library                   */
#include "timer.h"                               /* Timer driver                       */
#include "led.h"                                 /* LED driver                         */
#include "analogfloat.h"                         /* Floating analog input driver       */
#include "stm32f0xx.h"                           /* STM32 CPU and HAL header           */


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void     Init(void);
static void     SystemClock_Config(void);
static void     CustomAssertionHandler(const char * const file, uint32_t line);
static uint32_t CustomSeedInitHandler(void);


/************************************************************************************//**
** \brief     This is the entry point for the software application and is called
**            by the reset interrupt vector after the C-startup routines executed.
** \return    Program exit code.
**
****************************************************************************************/
int main(void)
{
  uint32_t lastLedToggleTime = 0;
  uint32_t numbers[8];
  size_t   idx;

  /* Initialize the microcontroller. */
  Init();

  /* Generate some random numbers and print them on the terminal (57600 bps). */
  for (idx = 0; idx < (sizeof(numbers)/sizeof(numbers[0])); idx++)
  {
    /* Get a new random number. */
    numbers[idx] = TbxRandomNumberGet();
    /* Print the value. */
    printf("Random number %u: %u.\n", idx+1, (unsigned int)numbers[idx]);
  }

  /* Start the infinite program loop. */
  while (1)
  {
    /* Toggle the LED every 500 ms. */
    if ((TimerGet() - lastLedToggleTime) >= 500u)
    {
      LedToggle();
      lastLedToggleTime += 500;
    }
  }
  /* Set program exit code. Note that the program should never get here. */
  return 0;
} /*** end of main ***/


/************************************************************************************//**
** \brief     Initializes the microcontroller.
** \return    none.
**
****************************************************************************************/
static void Init(void)
{
  /* Register the application specific assertion handler. */
  TbxAssertSetHandler(CustomAssertionHandler);
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock. */
  SystemClock_Config();
  /* Initialize the timer driver. */
  TimerInit();
  /* Initialize the LED driver. */
  LedInit();
  /* Initialize the floating analog input driver. */
  AnalogFloatInit();
  /* Register the application specific seed initialization handler. It makes use of the
   * analog float module and therefore needs to be called after the analog float
   * module was initialized.
   */
  TbxRandomSetSeedInitHandler(CustomSeedInitHandler);
} /*** end of Init ***/


/************************************************************************************//**
** \brief     System Clock Configuration. This code was created by CubeMX and configures
**            the system clock.
** \return    none.
**
****************************************************************************************/
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Initializes the CPU, AHB and APB busses clocks. */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Clock configuration incorrect or hardware failure. Hang the system to prevent
     * damage.
     */
    TBX_ASSERT(TBX_FALSE);
  }

  /* Initializes the CPU, AHB and APB busses clocks. */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  /* Set the flash latency. */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    /* Flash latency configuration incorrect or hardware failure. Hang the system to
     * prevent damage.
     */
    TBX_ASSERT(TBX_FALSE);
  }
} /*** end of SystemClock_Config ***/


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
** \details   This example implementation set the seed based on the value of a floating
**            analog input. Such a floating analog input will pick up noise, so the
**            analog to digital conversion results always vary slightly. Other options
**            would be to:
**            * Increment a 32-bit value in EEPROM or a non-volatile register, if
**              supported by the microcontroller, each time this function is called.
**              Keep in mind though that these data storage options have a limited amount
**              of write cycles. A better option might be to use external FRAM.
**            * If the system was access to an external file system such as an SD-card,
**              you could increment a 32-bit value in a file each time this function is
**              called.
** \return    The 32-bit value that the random number generator module uses as a seed to
**            initialize itself.
**
****************************************************************************************/
static uint32_t CustomSeedInitHandler(void)
{
  /* Create a 32-bit seed value by combining two reads of the floating analog pin. */
  return (AnalogFloatGet() << 16u) | AnalogFloatGet();
} /*** end of CustomSeedInitHandler ***/


/************************************************************************************//**
** \brief     Initializes the Global MSP. This function is called from HAL_Init()
**            function to perform system level initialization (GPIOs, clock, DMA,
**            interrupt).
** \return    none.
**
****************************************************************************************/
void HAL_MspInit(void)
{
  /* SYSCFG clock enable. */
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* SVC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
} /*** end of HAL_MspInit ***/


/*********************************** end of main.c *************************************/
