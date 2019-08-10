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


/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Layout of a data block entry, which forms the building block of a linked list
 *         consisting of data blocks.
 */
typedef struct t_block_entry
{
  /** \brief Pointer to the block data. */
  void                 * dataPtr;
  /** \brief Flag to keep track if the block data is already allocated. */
  uint8_t                allocated;
  /** \brief Pointer to the next block in the list or NULL if it is the list end. */
  struct t_block_entry * nextBlockPtr;
} tBlockEntry;

/** \brief Type of a linked list consisting of data blocks. */
typedef tBlockEntry (* tBlockList);


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static uint8_t   TbxMemPoolBlockListInit(tBlockList * blockListPtr, size_t numBlocks,
                                         size_t blockSize);
static void    * TbxMemPoolBlockListAllocate(tBlockList * blockListPtr);
static uint8_t   TbxMemPoolBlockListRelease(tBlockList * blockListPtr,
                                            void const * dataPtr);


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


/************************************************************************************//**
** \brief     Initializes the linked list with data blocks. This includes the memory
**            allocation for the list entries and the actual data for the data blocks.
**            All entries are marked as available such that its data block can be
**            allocated.
** \param     blockListPtr Pointer to the linked list with data blocks that needs to be
**            initialized.
** \param     numBlocks The number of blocks to statically preallocate on the heap for
**            this memory pool.
** \param     blockSize The size of each block in bytes.
** \return    TBX_OK if successful, TBX_ERROR otherwise for example when there is no
**            more space available on the heap.
**
****************************************************************************************/
static uint8_t TbxMemPoolBlockListInit(tBlockList * blockListPtr, size_t numBlocks,
                                       size_t blockSize)
{
  uint8_t       result = TBX_ERROR;
  void        * blockDataPtr;
  tBlockEntry * blockEntryPtr;
  tBlockEntry * lastBlockEntryPtr = NULL;
  size_t        blockIdx;

  /* Verify parameters. */
  TBX_ASSERT(blockListPtr != NULL);
  TBX_ASSERT(numBlocks > 0U);
  TBX_ASSERT(blockSize > 0U);

  /* Only continue if the parameters are valid. */
  if ( (blockListPtr != NULL) && (numBlocks > 0U) && (blockSize > 0U) )
  {
    /* Set the result value to okay. */
    result = TBX_OK;

    /* Loop through all the blocks to construct each data block entry and to add it to
     * the list of data blocks.
     */
    for (blockIdx = 0U; blockIdx < numBlocks; blockIdx++)
    {
      /* Allocate memory on the heap for the data block entry. */
      blockEntryPtr = TbxHeapAllocate(sizeof(tBlockEntry));
      /* Allocate memory on the heap for the block's data. */
      blockDataPtr = TbxHeapAllocate(blockSize);
      /* Evaluate success of the memory allocation. */
      if ( (blockEntryPtr == NULL) || (blockDataPtr == NULL) )
      {
        /* Allocation issue detected. Update result value and stop looping. */
        result = TBX_ERROR;
        break;
      }
      /* Initialize the block entry. */
      blockEntryPtr->dataPtr = blockDataPtr;
      blockEntryPtr->allocated = TBX_FALSE;
      blockEntryPtr->nextBlockPtr = NULL;
      /* Is this the first node for the list? */
      if (blockIdx == 0U)
      {
        /* Add the node to the head of the list. */
        *blockListPtr = blockEntryPtr;
      }
      /* It is not the first node in the list. */
      {
        /* Sanity check. */
        TBX_ASSERT(lastBlockEntryPtr != NULL);

        /* Only write to lastBlockEntry if the sanity check passed. */
        if (lastBlockEntryPtr != NULL)
        {
          /* Add the node at the end of the linked list. */
          lastBlockEntryPtr->nextBlockPtr = blockEntryPtr;
        }
      }
      /* Store the pointer to the last node in the list, which is needed to add the
       * next new node.
       */
      lastBlockEntryPtr = blockEntryPtr;
    }
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockListInit ***/


/************************************************************************************//**
** \brief     Allocates an entry from the linked list with data blocks that is still
**            available and returns the pointer to its data.
** \param     blockListPtr Pointer to the linked list with data blocks.
** \return    Pointer to the data of the block that was allocated if successful, or NULL
**            when no entry was found in the list that is still available for allocation.
**
****************************************************************************************/
static void * TbxMemPoolBlockListAllocate(tBlockList * blockListPtr)
{
  void * result = NULL;

  /* Verify parameter. */
  TBX_ASSERT(blockListPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (blockListPtr != NULL)
  {
    /* TODO ##Vg Implement TbxMemPoolBlockListAllocate(). */

    /* TODO ##Vg Attempt to make this deterministic. For example by having the free ones
     *      be in the front of the list and the allocated ones in the back. For this
     *      you just need to know where the end of the list is, so a new element for
     *      the list would be needed that holds a pointer to the last entry. This just
     *      means that the tBlockList type needs to be different. It should have
     *      elements: size, * head, * tail.
     */
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockListAllocate ***/


/************************************************************************************//**
** \brief     Releases the entry from the linked list with data blocks who's data pointer
**            matches the one specified by the parameter. This essentially makes the
**            data block entry available again for allocation.
** \param     blockListPtr Pointer to the linked list with data blocks.
** \param     dataPtr Pointer of the data block that should be released.
** \return    TBX_OK if successful, TBX_ERROR otherwise for example when the specified
**            data pointer does not match the data pointers in any of the list entries.
**
****************************************************************************************/
static uint8_t TbxMemPoolBlockListRelease(tBlockList * blockListPtr,
                                          void const * dataPtr)
{
  uint8_t result = TBX_ERROR;

  /* Verify parameters. */
  TBX_ASSERT(blockListPtr != NULL);
  TBX_ASSERT(dataPtr != NULL);

  /* Only continue if the parameters are valid. */
  if ( (blockListPtr != NULL) && (dataPtr != NULL) )
  {
    /* TODO ##Vg Implement TbxMemPoolBlockListRelease(). */

    /* TODO ##Vg The challenge here is that the dataPtr != entryPtr. This means a
     *      loop through the list is always needed. The only solution would be to
     *      store the size of the blocks with the list and make the entry a union with
     *      first the data, then the other elements. This way the dataPtr == entryPtr.
     *      With this modification is should be possible to make this call deterministic.
     */
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockListRelease ***/


/*********************************** end of tbxmempool.c *******************************/
