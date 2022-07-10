/************************************************************************************//**
* \file         tbxcxx.cpp
* \brief        Globally overloaded operators source file.
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

/*
 * On a microcontroller it is totally fine to dynamically allocate memory on the heap
 * using new (or malloc). It gets potentially troublesome when you also release it at
 * run-time using delelete (or free). Multiple allocation and release operations can
 * cause memory fragmentation. In a worst case scenario this leads to memory allocation
 * failures, because of running out of heap memory.
 *
 * A possible solution is by dynamically allocating memory using memory pools. This
 * lowers the risk of memory fragmentation. With a carefully selected memory pool setup,
 * you can even eliminate this risk completely. Allowing you to dynamically allocate and
 * release memory during run-time. MicroTBX includes a ready-made memory pool module that
 * can be used for this purpose.
 *
 * One way to approach this is by using C++ "placement new" instead of the usual "new".
 * This allows you the first allocate memory from a memory pool and then place the new
 * object exactly at that memory, instead of having the "new" operator do the allocation.
 * The only issue with this is that there is no "placement delete". This means that to
 * delete the object, you need to manually call its destructor and then give the
 * allocated memory back to the memory pool. Definitely an option, it just requires a bit
 * more work.
 *
 * Another approach is to overload the default "new" and "delete" operators to do all the
 * memory allocation and release using memory pools automatically. That's the purpose of
 * this file. By compiling and linking this source file with your project, the global new
 * and delete operators are overloaded, such that they by default always use the memory
 * pools module of MicroTBX.
 */


/****************************************************************************************
* Include files
****************************************************************************************/
#include <cstdlib>
#include "microtbx.h"


/************************************************************************************//**
** \brief     Overloading global new operator.
** \param     size Size of the memory to allocate.
**
****************************************************************************************/
void * operator new(size_t size)
{
  void * result;
  
  /* Attempt to allocate a block from the best fitting memory pool. */
  result = TbxMemPoolAllocate(size);
  /* Was the allocation not successful? */
  if (result == nullptr)
  {
    /* The allocation failed. This can have two reasons:
     *   1. A memory pool for the requested size hasn't yet been created.
     *   2. The memory pool for the requested size has no more free blocks.
     * Both situations can be solved by calling TbxMemPoolCreate(), as this
     * function automatically extends a memory pool, if it was created before.
     * Note that there is no need to check the return value, because we will
     * attempt to allocate again right afterwards. We can catch the error
     * there in case the allocation fails.
     */
    (void)TbxMemPoolCreate(1U, size);

    /* Assuming sufficient heap was available, the memory pool was extended.
     * Attempt to allocate the block again.
     */
    result = TbxMemPoolAllocate(size);
  }
  /* Verify the allocation result. */
  if (result == nullptr)
  {
    /* Since exceptions aren't used, call abort directly to indicate an abnormal end to
     * the program, since new is not allowed to return a nullptr.
     */
    std::abort();
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of operator new ***/


/************************************************************************************//**
** \brief     Overloading global delete operator.
** \param     mem Pointer of memory block to release.
**
****************************************************************************************/
void operator delete(void * mem)
{
  /* Give the block back to the memory pool. */
  if (mem != nullptr)
  {
    TbxMemPoolRelease(mem);
  }
} /*** end of operator delete ***/


/************************************************************************************//**
** \brief     Overloading global delete operator.
** \param     mem Pointer of memory block to release.
** \param     size Size of the block.
**
****************************************************************************************/
void operator delete(void * mem, unsigned int size)
{
  /* Memory pool manager already knows the size of the block. */
  TBX_UNUSED_ARG(size);
  
  /* Give the block back to the memory pool. */
  return ::operator delete(mem);
} /*** end of operator delete ***/


/************************************************************************************//**
** \brief     Overloading global new[] operator.
** \param     size Size of the memory to allocate.
**
****************************************************************************************/
void * operator new[](size_t size)
{
  /* Allocate and return the block of memory. */
  return ::operator new(size);
} /*** end of operator new[] ***/


/************************************************************************************//**
** \brief     Overloading global delete[] operator.
** \param     mem Pointer of memory block to release.
**
****************************************************************************************/
void operator delete[](void * mem)
{
  /* Give the block back to the memory pool. */
  return ::operator delete(mem);
} /*** end of operator delete[] ***/


/************************************************************************************//**
** \brief     Overloading global delete[] operator.
** \param     mem Pointer of memory block to release.
** \param     size Size of the block.
**
****************************************************************************************/
void operator delete[](void * mem, unsigned int size)
{
  /* Memory pool manager already knows the size of the block. */
  TBX_UNUSED_ARG(size);

  /* Give the block back to the memory pool. */
  return ::operator delete(mem);
} /*** end of operator delete[] ***/


/*********************************** end of operators.cpp ******************************/
