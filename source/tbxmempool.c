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
/** \brief Layout of a block node, which forms the building block of a linked list
 *         consisting free data blocks.
 */
typedef struct t_block_node
{
  /** \brief Pointer to the block data. */
  void                * dataPtr;
  /** \brief Pointer to the next node in the list or NULL if it is the list end. */
  struct t_block_node * nextNodePtr;
} tBlockNode;

/** \brief Linked list consisting of free data block nodes. */
typedef tBlockNode (* tFreeBlocksList);

/** \brief Layout of a single memory pool. */
typedef struct
{
  /** \brief The number of bytes that fit in one block. */
  size_t            blockSize;
  /** \brief Pointer to the linked list with free data block nodes. */
  tFreeBlocksList * freeBlocksListPtr;
} tMemPool;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static tFreeBlocksList * TbxMemPoolFreeBlocksListCreate(void);
static void TbxMemPoolFreeBlocksListInsert(tFreeBlocksList * freeBlocksListPtr,
                                           tBlockNode * blockNodePtr);
static tBlockNode * TbxMemPoolFreeBlocksListExtract(tFreeBlocksList * freeBlocksListPtr);


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/* TODO ##Vg Improve such that multiple memory pools are supported and dynamically
 *      allocated, instead of just this one that is statically preallocated.
 */
/** \brief Temporary statically allocated memory pool. Eventually, this software module
 *         should support multiple dynamically allocated memory pools.
 */
static tMemPool tbxMemPool;


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
  uint8_t      result = TBX_ERROR;
  size_t       blockNodeIdx;
  tBlockNode * blockNodePtr;
  void       * blockDataPtr;

  /* Verify parameters. */
  TBX_ASSERT(numBlocks > 0U);
  TBX_ASSERT(blockSize > 0U);

  /* Only continue if the parameters are valid. */
  if ( (numBlocks > 0U) && (blockSize > 0U) )
  {
    /* Set the result value to okay. */
    result = TBX_OK;
    /* Store the block size of this memory pool. */
    tbxMemPool.blockSize = blockSize;
    /* Create a linked list object for holding the free memory blocks. */
    tbxMemPool.freeBlocksListPtr = TbxMemPoolFreeBlocksListCreate();
    /* Verify that the linked list object could be created. */
    if (tbxMemPool.freeBlocksListPtr == NULL)
    {
      result = TBX_ERROR;
    }
    /* Only continue if all is okay so far. */
    if (result == TBX_OK)
    {
    }
    /* Only continue if all is okay so far. */
    if (result == TBX_OK)
    {
      /* Loop through all the blocks. */
      for (blockNodeIdx = 0; blockNodeIdx < numBlocks; blockNodeIdx++)
      {
        /* Allocate memory for the block node. */
        blockNodePtr = TbxHeapAllocate(sizeof(tBlockNode));
        /* Verify that the node could be allocated. */
        if (blockNodePtr == NULL)
        {
          /* Flag error and stop looping. */
          result = TBX_ERROR;
        }
        else
        {
          /* Allocate memory for the actual block data */
          blockDataPtr = TbxHeapAllocate(blockSize);
          /* Verify that the data could be allocated. */
          if (blockDataPtr == NULL)
          {
            /* Flag error and stop looping. */
            result = TBX_ERROR;
          }
          else
          {
            /* Initialize the node. */
            blockNodePtr->dataPtr = blockDataPtr;
            blockNodePtr->nextNodePtr = NULL;
            /* Add the block node to the linked list with free block nodes. */
            TbxMemPoolFreeBlocksListInsert(tbxMemPool.freeBlocksListPtr, blockNodePtr);
          }
        }
      }
    }
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
  void             * result = NULL;
  tBlockNode const * blockNodePtr;

  /* Verify parameter. */
  TBX_ASSERT(size > 0U);

  /* Only continue if the parameter is valid. */
  if (size > 0U)
  {
    /* Make sure that the size fits inside a block of the memory pool. Note that there
     * is no need to use a critical section when accessing the blockSize and
     * freeBlocksListPtr of the memory pool. These fields are only written once when
     * the memory pool object is created and will not be changed afterwards.
     */
    if (size <= tbxMemPool.blockSize)
    {
      /* Attempt to extract a block node from the linked list with free block nodes. */
      blockNodePtr = TbxMemPoolFreeBlocksListExtract(tbxMemPool.freeBlocksListPtr);
      /* Only continue if a free block node could be extracted. */
      if (blockNodePtr != NULL)
      {
        /* Read and store the pointer that points to the block's data. */
        result = blockNodePtr->dataPtr;
      }
    }
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

  /* TODO ##Vg. Current approach does not work, because there is no way to get the
   *      tBlockNode pointer for the node who's dataPtr element equals the specified
   *      memPtr. Then it is not possible to return it to the free list. At least not
   *      in a MISRA compliant way.
   *      Perhaps the approach where you do not have a free block list, but a block list
   *      works better. Assuming that block node has a field for 'available'. Then the
   *      free ones are at the head and the used ones at the tail to easily get a node.
   *      === CONTINUE HERE ===
   */

  /* Only continue if the parameter is valid. */
  if (memPtr != NULL)
  {
    /* TODO ##Vg Implement TbxMemPoolRelease(). */
  }
} /*** end of TbxMemPoolRelease ***/


/************************************************************************************//**
** \brief     Creates a new and empty linked list that can be used for building a list
**            with free block nodes. Memory needed for the linked list object itself is
**            allocated on the heap.
** \return    Pointer to the newly created free blocks list if successful, NULL
**            otherwise.
**
****************************************************************************************/
static tFreeBlocksList * TbxMemPoolFreeBlocksListCreate(void)
{
  tFreeBlocksList * result;

  /* Attempt to create the linked list object on the heap. */
  result = TbxHeapAllocate(sizeof(tFreeBlocksList));
  /* Only continue with the linked list initialization when the allocation was
   * successful.
   */
  if (result != NULL)
  {
    /* Initialize the linked list to be empty. */
    *result = NULL;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolFreeBlocksListCreate ***/


/************************************************************************************//**
** \brief     Inserts the specified block node into the linked list with free block
**            nodes.
** \param     freeBlocksListPtr Pointer to the linked list with free blocks.
** \param     blockNodePtr Pointer to the block node to insert.
**
****************************************************************************************/
static void TbxMemPoolFreeBlocksListInsert(tFreeBlocksList * freeBlocksListPtr,
                                           tBlockNode * blockNodePtr)
{
  /* Verify parameters. */
  TBX_ASSERT(freeBlocksListPtr != NULL);
  TBX_ASSERT(blockNodePtr != NULL);

  /* Only continue if the parameters are valid. */
  if ( (freeBlocksListPtr != NULL) && (blockNodePtr != NULL) )
  {
    /* Obtain mutual exclusive access to the linked list with free block nodes. */
    TbxCriticalSectionEnter();
    /* Is the list currently empty? */
    if (*freeBlocksListPtr == NULL)
    {
      /* There is no next node. */
      blockNodePtr->nextNodePtr = NULL;
    }
    /* Linked list it not empty. */
    {
      /* Move the current begin of the list one down. */
      blockNodePtr->nextNodePtr = (*freeBlocksListPtr)->nextNodePtr;
    }
    /* Insert the next node at the start of the list. */
    *freeBlocksListPtr = blockNodePtr;
    /* Release mutual exclusive access to the linked list with free block nodes. */
    TbxCriticalSectionExit();
  }
} /*** end of TbxMemPoolFreeBlocksListInsert ***/


/************************************************************************************//**
** \brief     Extracts a free block node from the linked list with free block nodes.
** \param     freeBlocksListPtr Pointer to the linked list with free blocks.
** \return    Pointer to the block node that was extracted or NULL if the linked list
**            contained no more nodes with a free block.
**
****************************************************************************************/
static tBlockNode * TbxMemPoolFreeBlocksListExtract(tFreeBlocksList * freeBlocksListPtr)
{
  tBlockNode * result = NULL;

  /* Verify parameter. */
  TBX_ASSERT(freeBlocksListPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (freeBlocksListPtr != NULL)
  {
    /* Obtain mutual exclusive access to the linked list with free block nodes. */
    TbxCriticalSectionEnter();
    /* Only extract a node if the list currently not empty. */
    if (*freeBlocksListPtr != NULL)
    {
      /* Get the first node. */
      result = *freeBlocksListPtr;
      /* Is there currently only one node in the list? */
      if ((*freeBlocksListPtr)->nextNodePtr == NULL)
      {
        /* Set the list to be empty. */
        *freeBlocksListPtr = NULL;
      }
      /* There are currently at least two nodes in the list. */
      else
      {
        /* Make the second node the first one. */
        (*freeBlocksListPtr) = result->nextNodePtr;
      }
    }
    /* Release mutual exclusive access to the linked list with free block nodes. */
    TbxCriticalSectionExit();
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolFreeBlocksListExtract ***/


/*********************************** end of tbxmempool.c *******************************/
