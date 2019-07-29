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


/****************************************************************************************
* Local constant declarations
****************************************************************************************/
/* The key that will be used to encrypt and decrypt data. */
static const uint8_t cryptoKey[32] =
{
  0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x21, 0x41,
  0x25, 0x44, 0x2A, 0x47, 0x2D, 0x4B, 0x61, 0x50,
  0x64, 0x53, 0x67, 0x56, 0x6B, 0x59, 0x70, 0x33,
  0x73, 0x36, 0x76, 0x39, 0x79, 0x24, 0x42, 0x3F
};

/* Original data. */
static const uint8_t originalData[64] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
};


/************************************************************************************//**
** \brief     This is the entry point for the bootloader application and is called
**            by the reset interrupt vector after the C-startup routines executed.
** \return    Program exit code.
**
****************************************************************************************/
int main(void)
{
  uint32_t lastLedToggleTime = 0;
  uint32_t numbers[8];
  uint8_t  idx;
  uint8_t  buffer[64];

  /* Initialize the microcontroller. */
  Init();

  /* Copy the original data to the buffer. */
  for (idx = 0U; idx < 64U; idx++)
  {
    buffer[idx] = originalData[idx];
  }
  /* Encrypt the data in the buffer. */
  TbxCryptoAes256Encrypt(buffer, 64, cryptoKey);
  /* Decrypt the data in the buffer. */
  TbxCryptoAes256Decrypt(buffer, 64, cryptoKey);
  /* Verify that the decrypted data is the same as the original data. */
  for (idx = 0U; idx < 64U; idx++)
  {
    if (buffer[idx] != originalData[idx])
    {
      TBX_ASSERT(TBX_ERROR);
    }
  }

  /* Generate some random numbers and print them on the terminal. */
  for (idx = 0; idx < (sizeof(numbers)/sizeof(numbers[0])); idx++)
  {
    /* Get a new random number. */
    numbers[idx] = TbxRandomNumberGet();
    /* Print the value. */
    printf("Random number %d: %u.\n", idx+1, (unsigned int)numbers[idx]);
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
  while (1)
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
