/************************************************************************************//**
* \file         unittests.c
* \brief        Unit tests source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2022 by Feaser     www.feaser.com     All rights reserved
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
#include "unity.h"                               /* Unity unit test framework          */
#include "unittests.h"                           /* Unit tests header                  */
#include <sys/time.h>                            /* Time definitions                   */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Keeps track of how often an assertion got triggered. */
uint32_t assertionCnt = 0;


/************************************************************************************//**
** \brief     Handles the run-time assertions. 
** \param     file The filename of the source file where the assertion occurred in.
** \param     line The line number inside the file where the assertion occurred.
**
****************************************************************************************/
void handleTbxAssertion(const char * const file, uint32_t line)
{
  /* Increment the assertion counter. */
  assertionCnt++;
} /*** end of handleTbxAssertion ***/


/************************************************************************************//**
** \brief     Handler function that gets called by the random number generator. This
**            module requires a seed, which this function should obtain. 
** \details   This example implementation set the seed based on the value of system time.
** \return    The 32-bit value that the random number generator module uses as a seed to
**            initialize itself.
**
****************************************************************************************/
uint32_t seedInitHandler(void)
{
  uint32_t result = 0;
  struct timeval tv;

  if (gettimeofday(&tv, NULL) == 0)
  {
    result = (((uint32_t)tv.tv_sec * 1000U) + ((uint32_t)tv.tv_usec / 1000U));
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of seedInitHandler ***/


/************************************************************************************//**
** \brief     Tests that an assertion triggers if you specify NULL when configuring a
**            custom assertion handler.
**
****************************************************************************************/
void test_TbxAssertSetHandler_ShouldTriggerAssertionIfParamNull(void)
{
  /* Attempt to configure an invalid custom assertion handler. */
  TbxAssertSetHandler(NULL);
  /* Make sure an assertion was triggered. Note that this also verifies that the 
   * initially configured assertion handler still works.
   */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxAssertSetHandler_ShouldTriggerAssertionIfParamNull ***/


/************************************************************************************//**
** \brief     Tests that the triggering of assertions work.
**
****************************************************************************************/
void test_TbxAssertTrigger_ShouldTriggerAssertion(void)
{
  /* Perform an assertion that shouldn't trigger. */
  TBX_ASSERT(TBX_TRUE);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
  /* Perform an assertion that shouldn trigger. */
  TBX_ASSERT(TBX_FALSE);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxAssertTrigger_ShouldTriggerAssertion ***/


/************************************************************************************//**
** \brief     Tests that an assertion triggerss if you attempt to exit a critical section
**            before first entering one.
**
****************************************************************************************/
void test_TbxCriticalSectionExit_ShouldTriggerAssertionIfNotInCritSect(void)
{
  /* Exit a critical section, which hasn't actually been entered yet. */
  TbxCriticalSectionExit();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxCriticalSectionExit_ShouldTriggerAssertionIfNotInCritSect ***/


/************************************************************************************//**
** \brief     Tests that an assertion triggerss if you attempt to exit a critical section
**            before first entering one.
**
****************************************************************************************/
void test_TbxCriticalSectionEnter_ShouldNotAssertUponCritSectExit(void)
{
  /* Enter and exit a critical section. */
  TbxCriticalSectionEnter();
  TbxCriticalSectionExit();
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxCriticalSectionEnter_ShouldNotAssertUponCritSectExit ***/


/************************************************************************************//**
** \brief     Tests that free heap size reporting works.
** \attention Should run before any other tests that might allocated from the heap.
**
****************************************************************************************/
void test_TbxHeapGetFree_ShouldReturnActualFreeSize(void)
{
  size_t initialFreeHeap;
  size_t currentFreeHeap;
  void * mem;
  const size_t allocSize = 2;

  /* Get the initial free heap size. */
  initialFreeHeap = TbxHeapGetFree();
  /* Heap should not be zero. */
  TEST_ASSERT_GREATER_THAN(0, initialFreeHeap);
  /* First time that anything gets allocated means that the full heap should be free. */
  TEST_ASSERT_EQUAL(TBX_CONF_HEAP_SIZE, initialFreeHeap);
  /* Allocate some memory from the heap. */
  mem = TbxHeapAllocate(allocSize);
  /* Get the current free heap size. */
  currentFreeHeap = TbxHeapGetFree();
  /* Free size after allocation should be less then before. */
  TEST_ASSERT_LESS_THAN(initialFreeHeap, currentFreeHeap);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxHeapGetFree_ShouldReturnActualFreeSize ***/


/************************************************************************************//**
** \brief     Tests that basic memory allocation from the heap works.
**
****************************************************************************************/
void test_TbxHeapAllocate_ShouldReturnNotNull(void)
{
  void * mem;
  const size_t allocSize = 2;

  /* Allocate some memory from the heap. */
  mem = TbxHeapAllocate(allocSize);
  /* Make sure the allocation worked. */
  TEST_ASSERT_NOT_NULL(mem);  
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxHeapAllocate_ShouldReturnNotNull ***/


/************************************************************************************//**
** \brief     Tests that a zero size memory allocation does not work.
**
****************************************************************************************/
void test_TbxHeapAllocate_ShouldReturnNullIfZeroSizeAllocated(void)
{
  size_t initialFreeHeap;
  size_t currentFreeHeap;
  void * mem;
  const size_t allocSize = 0;

  /* Get the initial free heap size. */
  initialFreeHeap = TbxHeapGetFree();
  /* Allocate zero memory from the heap. */
  mem = TbxHeapAllocate(allocSize);
  /* Make sure the allocation failed. */
  TEST_ASSERT_NULL(mem);  
  /* Get current free heap size. */
  currentFreeHeap = TbxHeapGetFree();
  /* Should still be the same as before, because nothing should have been allocated. */
  TEST_ASSERT_EQUAL(initialFreeHeap, currentFreeHeap);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxHeapAllocate_ShouldReturnNullIfZeroSizeAllocated ***/


/************************************************************************************//**
** \brief     Tests that too much memory allocation does not work.
**
****************************************************************************************/
void test_TbxHeapAllocate_ShouldReturnNullIfTooMuchAllocated(void)
{
  size_t initialFreeHeap;
  size_t currentFreeHeap;
  size_t allocSize;
  void * mem;

  /* Get the initial free heap size. */
  initialFreeHeap = TbxHeapGetFree();
  /* Allocate more memory from the heap than what is currently available. */
  allocSize = initialFreeHeap + 1;
  mem = TbxHeapAllocate(allocSize);
  /* Make sure the allocation failed. */
  TEST_ASSERT_NULL(mem);  
  /* Get current free heap size. */
  currentFreeHeap = TbxHeapGetFree();
  /* Should still be the same as before, because nothing should have been allocated. */
  TEST_ASSERT_EQUAL(initialFreeHeap, currentFreeHeap);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxHeapAllocate_ShouldReturnNullIfTooMuchAllocated ***/


/************************************************************************************//**
** \brief     Tests that the allocated size automatically aligns to the pointer size
**            of the architecture. So for 32-bit memory addresses, the allocated size
**            should always be aligned to 4 bytes.
**
****************************************************************************************/
void test_TbxHeapAllocate_ShouldAlignToAddressSize(void)
{
  size_t initialFreeHeap;
  size_t currentFreeHeap;
  size_t delta;
  size_t addressSize;
  const size_t allocSize = 1;
  void * mem;

  /* Get the initial free heap size. */
  initialFreeHeap = TbxHeapGetFree();
  /* Allocate a single byte. */
  mem = TbxHeapAllocate(allocSize);
  /* Make sure the allocation worked. */
  TEST_ASSERT_NOT_NULL(mem);  
  /* Get current free heap size. */
  currentFreeHeap = TbxHeapGetFree();
  /* Calculate the delta. */
  delta = initialFreeHeap - currentFreeHeap;
  /* Determine architectures address size by looking at the width of a pointer. */
  addressSize = sizeof(void *);
  /* Make sure the allocated size was aligned to the address size automatically. */
  /* Should still be the same as before, because nothing should have been allocated. */
  TEST_ASSERT_EQUAL(addressSize, delta);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxHeapAllocate_ShouldAlignToAddressSize ***/


/************************************************************************************//**
** \brief     Tests that an assertion is triggered if you try to set an invalid seed
**            initialization handler.
**
****************************************************************************************/
void test_TbxRandomSetSeedInitHandler_ShouldTriggerAssertionIfParamNull(void)
{
  /* Attempt to configure an invalid custom seed initialization handler. */
  TbxRandomSetSeedInitHandler(NULL);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxRandomSetSeedInitHandler_ShouldTriggerAssertionIfParamNull ***/


/************************************************************************************//**
** \brief     Tests that the setting of a seed initialization handler works.
**
****************************************************************************************/
void test_TbxRandomSetSeedInitHandler_ShouldWork(void)
{
  /* Attempt to configure an valid custom seed initialization handler. */
  TbxRandomSetSeedInitHandler(seedInitHandler);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxRandomSetSeedInitHandler_ShouldWork ***/


/************************************************************************************//**
** \brief     Tests that the randon numbers are actually created.
**
****************************************************************************************/
void test_TbxRandomNumberGet_ShouldReturnRandomNumbers(void)
{
  uint32_t randomNumber1;
  uint32_t randomNumber2;

  /* Attempt to obtain two random numbers. */
  randomNumber1 = TbxRandomNumberGet();
  randomNumber2 = TbxRandomNumberGet();
  /* Make sure the numbers are not equal. */
  TEST_ASSERT_NOT_EQUAL_UINT32(randomNumber1, randomNumber2);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxRandomNumberGet_ShouldReturnRandomNumbers ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger and assertion and returns zero.
**
****************************************************************************************/
void test_TbxChecksumCrc16Calculate_ShouldAssertOnInvalidParams(void)
{
  uint16_t checksum; 
  const uint8_t sourceData[] = 
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,    
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F    
  };
  const size_t sourceLen = sizeof(sourceData)/sizeof(sourceData[0]);

  /* Pass a NULL pointer for the data byte array. */
  checksum = TbxChecksumCrc16Calculate(NULL, sourceLen);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Verify that it did not continue with the actual checksum calculation. */
  TEST_ASSERT_EQUAL_UINT16(0, checksum);
  /* Reset the assertion counter. */
  assertionCnt = 0;
  /* Pass an invalid size. */
  checksum = TbxChecksumCrc16Calculate(sourceData, 0);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Verify that it did not continue with the actual checksum calculation. */
  TEST_ASSERT_EQUAL_UINT16(0, checksum);
} /*** end of test_TbxChecksumCrc16Calculate_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the calculated checksum is correct.
**
****************************************************************************************/
void test_TbxChecksumCrc16Calculate_ShouldReturnValidCrc16(void)
{
  uint16_t checksum; 
  const uint8_t sourceData[] = 
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,    
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F    
  };
  const size_t sourceLen = sizeof(sourceData)/sizeof(sourceData[0]);
  /* Obtained using an online CRC calculator using the CRC16_CCITT_FALSE algorithm:
   * http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
   */
  const uint16_t correctChecksum = 0x23B3;

  /* Calculate the checksum. */
  checksum = TbxChecksumCrc16Calculate(sourceData, sourceLen);
  /* Verify the correctness of the checksum calculation. */
  TEST_ASSERT_EQUAL_UINT16(correctChecksum, checksum);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxChecksumCrc16Calculate_ShouldReturnValidCrc16 ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger and assertion and returns zero.
**
****************************************************************************************/
void test_TbxChecksumCrc32Calculate_ShouldAssertOnInvalidParams(void)
{
  uint32_t checksum; 
  const uint8_t sourceData[] = 
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,    
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F    
  };
  const size_t sourceLen = sizeof(sourceData)/sizeof(sourceData[0]);

  /* Pass a NULL pointer for the data byte array. */
  checksum = TbxChecksumCrc32Calculate(NULL, sourceLen);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Verify that it did not continue with the actual checksum calculation. */
  TEST_ASSERT_EQUAL_UINT32(0, checksum);
  /* Reset the assertion counter. */
  assertionCnt = 0;
  /* Pass an invalid size. */
  checksum = TbxChecksumCrc32Calculate(sourceData, 0);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Verify that it did not continue with the actual checksum calculation. */
  TEST_ASSERT_EQUAL_UINT32(0, checksum);
} /*** end of test_TbxChecksumCrc32Calculate_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the calculated checksum is correct.
**
****************************************************************************************/
void test_TbxChecksumCrc32Calculate_ShouldReturnValidCrc32(void)
{
  uint32_t checksum; 
  const uint8_t sourceData[] = 
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,    
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F    
  };
  const size_t sourceLen = sizeof(sourceData)/sizeof(sourceData[0]);
  /* Obtained using an online CRC calculator using the CRC16_BUYPASS algorithm:
   * http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
   */
  const uint32_t correctChecksum = 0x8F819950UL;

  /* Calculate the checksum. */
  checksum = TbxChecksumCrc32Calculate(sourceData, sourceLen);
  /* Verify the correctness of the checksum calculation. */
  TEST_ASSERT_EQUAL_UINT32(correctChecksum, checksum);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxChecksumCrc32Calculate_ShouldReturnValidCrc32 ***/


/************************************************************************************//**
** \brief     Handles the running of the unit tests.
** \return    Test results.
**
****************************************************************************************/
int runTests(void)
{
  /* Inform the framework that unit testing is about to start. */
  UNITY_BEGIN();
  /* Tests for the assertion module. */
  RUN_TEST(test_TbxAssertSetHandler_ShouldTriggerAssertionIfParamNull);
  RUN_TEST(test_TbxAssertTrigger_ShouldTriggerAssertion);
  /* Tests for the critical section module. */
  RUN_TEST(test_TbxCriticalSectionExit_ShouldTriggerAssertionIfNotInCritSect);
  RUN_TEST(test_TbxCriticalSectionEnter_ShouldNotAssertUponCritSectExit);
  /* Tests for the heap module. */
  RUN_TEST(test_TbxHeapGetFree_ShouldReturnActualFreeSize);
  RUN_TEST(test_TbxHeapAllocate_ShouldReturnNotNull);
  RUN_TEST(test_TbxHeapAllocate_ShouldReturnNullIfZeroSizeAllocated);
  RUN_TEST(test_TbxHeapAllocate_ShouldReturnNullIfTooMuchAllocated);
  RUN_TEST(test_TbxHeapAllocate_ShouldAlignToAddressSize);
  /* Tests for the random number module. */
  RUN_TEST(test_TbxRandomSetSeedInitHandler_ShouldTriggerAssertionIfParamNull);
  RUN_TEST(test_TbxRandomSetSeedInitHandler_ShouldWork);
  RUN_TEST(test_TbxRandomNumberGet_ShouldReturnRandomNumbers);
  /* Tests for the checksum module. */
  RUN_TEST(test_TbxChecksumCrc16Calculate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxChecksumCrc16Calculate_ShouldReturnValidCrc16);
  RUN_TEST(test_TbxChecksumCrc32Calculate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxChecksumCrc32Calculate_ShouldReturnValidCrc32);
  /* Inform the framework that unit testing is done and return the result. */
  return UNITY_END();
} /*** end of runUnittests ***/


/************************************************************************************//**
** \brief     Initialization before running the unit tests.
**
****************************************************************************************/
void initializeTests(void)
{
/* Register the application specific assertion handler. */
  TbxAssertSetHandler(handleTbxAssertion);
} /*** end of initializeTests ***/


/************************************************************************************//**
** \brief     Code to run before each test.
**
****************************************************************************************/
void setUp(void)
{
  /* Reset the assertion counter. */
  assertionCnt = 0;
} /*** end of setUp ***/


/************************************************************************************//**
** \brief     Code to run after each test.
**
****************************************************************************************/
void tearDown(void)
{
} /*** end of tearDown ***/


/*********************************** end of unittests.c ********************************/
