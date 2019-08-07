/************************************************************************************//**
* \file         tbxmempool.c
* \brief        Pool based dynamic heap memory manager source file.
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
#include "microtbx.h"                            /* MicroTBX global header             */


/************************************************************************************//**
** \brief     Creates a new memory pool with the specified number of blocks, where each
**            block has the size as specified by the second function parameter. The
**            required memory is statically preallocated on the heap. This function is
**            typically called one or more times during software program initialization,
**            before the infinite program loop is entered. Once one ore more memory pools
**            were created, the functions TbxMemPoolAllocate() and TbxMemPoolRelease()
**            can be called to perform dynamic memory allocation. A well designed memory
**            pool approach makes dynamic memory allocation possible on RAM constrained
**            microcontrollers, without the need to worry about memory fragmentation.
**            Note that deleting a previously created memory pool is no supported on
**            purpose to prevent memory fragmentation.
** \param     numBlocks The number of blocks to statically preallocate on the heap for
**            this memory pool.
** \param     blockSize The size of each block in bytes.
** \return    TBX_OK if successful, TBX_ERROR otherwise for example when there is no
**            more space available on the heap to statically preallocated the blocks.
**
****************************************************************************************/
uint8_t TbxMemPoolCreate(size_t numBlocks, size_t blockSize)
{
  uint8_t result = TBX_ERROR;

  /* Verify parameters. */
  TBX_ASSERT(numBlocks > 0U);
  TBX_ASSERT(blockSize > 0U);

  /* Only continue if the parameters are valid. */
  if ( (numBlocks > 0U) && (blockSize > 0U) )
  {
    /* TODO ##Vg Implement TbxMemPoolCreate(). */
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolCreate ***/


/************************************************************************************//**
** \brief     Attempts to allocate the desired number of bytes in a previously created
**            memory pool. A block will be allocated in a memory pool that best fits the
**            requested byte size. For example, if the following memory pools were
**            created during program initialization:
**              TbxMemPoolCreate(10, 8);
**              TbxMemPoolCreate(10, 16);
**              TbxMemPoolCreate(10, 32);
**            And the following function call is made:
**              uint8_t * myMem = TbxMemPoolAllocate(9);
**            Then the memory will be allocated from the memory pool with block size 16,
**            so the second memory pool that was created. If there are no more blocks
**            available in that memory pool, then the memory pool with one block size
**            larger is used, so the one with block size 32, etc.
** \param     size The number of bytes to allocate in a memory pool.
** \return    Pointer to the start of the newly allocated memory if successful, NULL
**            otherwise.
**
****************************************************************************************/
void * TbxMemPoolAllocate(size_t size)
{
  void * result = NULL;

  /* Verify parameter. */
  TBX_ASSERT(size > 0U);

  /* Only continue if the parameter is valid. */
  if (size > 0U)
  {
    /* TODO ##Vg Implement TbxMemPoolAllocate(). */
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolAllocate ***/


/************************************************************************************//**
** \brief     Releases the previously allocated block of memory. Once the memory is
**            released, it can be allocated again afterwards with function
**            TbxMemPoolAllocate(). Note that this function automatically finds the
**            memory pool that the memory block belongs to.
** \param     memPtr Pointer to the start of the memory block. Basically, the pointer
**            that was returned by function TbxMemPoolAllocate(), when the memory was
**            initially allocated.
**
****************************************************************************************/
void TbxMemPoolRelease(void const * memPtr)
{
  /* Verify parameter. */
  TBX_ASSERT(memPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (memPtr != NULL)
  {
    /* TODO ##Vg Implement TbxMemPoolRelease(). */
  }
} /*** end of TbxMemPoolRelease ***/


/*********************************** end of tbxmempool.c *******************************/
