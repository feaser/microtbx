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
  /** \brief Pointer to the block. */
  void                * blockPtr;
  /** \brief Pointer to the next node in the list or NULL if it is the list end. */
  struct t_block_node * nextNodePtr;
} tBlockNode;

/** \brief Linked list consisting of data block nodes. */
typedef tBlockNode (* tBlockList);

/** \brief Layout of a single memory pool. */
typedef struct
{
  /** \brief The number of bytes that fit in one block. */
  size_t       blockSize;
  /** \brief Pointer to the linked list with free data block nodes. */
  tBlockList * freeBlockListPtr;
  /** \brief Pointer to the linked list with used data block nodes. */
  tBlockList * usedBlockListPtr;
} tMemPool;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
/* Block management functions. */
static void   * TbxMemPoolBlockCreate(size_t size);
static void   * TbxMemPoolBlockGetDataPtr(void * memPtr);
static size_t   TbxMemPoolBlockGetBlockSize(void const * memPtr);
static void   * TbxMemPoolBlockGetMemPtr(void * dataPtr);
/* Block list management functions. */
static tBlockList * TbxMemPoolBlockListCreate(void);
static void         TbxMemPoolBlockListInsert(tBlockList * listPtr,
                                              tBlockNode * nodePtr);
static tBlockNode * TbxMemPoolBlockListExtract(tBlockList * listPtr);


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
#if 0
  size_t       blockNodeIdx;
  tBlockNode * blockNodePtr;
  void       * blockDataPtr;
#endif

  /* Verify parameters. */
  TBX_ASSERT(numBlocks > 0U);
  TBX_ASSERT(blockSize > 0U);

  /* Only continue if the parameters are valid. */
  if ( (numBlocks > 0U) && (blockSize > 0U) )
  {
#if 0
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
#endif
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
#if 0
  tBlockNode const * blockNodePtr;
#endif

  /* Verify parameter. */
  TBX_ASSERT(size > 0U);

  /* Only continue if the parameter is valid. */
  if (size > 0U)
  {
#if 0
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
#endif
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


/****************************************************************************************
*   B L O C K   M A N A G E M E N T   F U N C T I O N S
****************************************************************************************/

/************************************************************************************//**
** \brief     Creates and initializes a new block including its memory allocation on the
**            heap. A block consists of the actual memory to hold the block data and is
**            preceded by an element of size_t, where the size of the block is written
**            to:
**            memPtr  -> -----------
**                      | blockSize |
**            dataPtr ->|------------------------------------------------
**                      | data byte 0 | data byte 1 | data byte 2 | etc. |
**                       ------------------------------------------------
** \param     size The data size of the block in bytes.
** \return    Pointer to the memory of the created block if successful, NULL otherwise.
**
****************************************************************************************/
static void * TbxMemPoolBlockCreate(size_t size)
{
  void   * result = NULL;
  void   * blockMemPtr;
  size_t * blockSizeArray;

  /* Verify parameter. */
  TBX_ASSERT(size > 0U);

  /* Only continue if the parameter is valid. */
  if (size > 0U)
  {
    /* Allocate memory for the block data and an extra size_t value at the start. */
    blockMemPtr = TbxHeapAllocate(sizeof(size_t) + size);
    /* Only continue if the memory allocation was successful. */
    if (blockMemPtr != NULL)
    {
      /* Set the result value. */
      result = blockMemPtr;
      /* Create a pointer to an array of size_t elements. */
      blockSizeArray = blockMemPtr;
      /* Write to the first element, which should hold the block size. */
      blockSizeArray[0U] = size;
    }
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockCreate ***/


/************************************************************************************//**
** \brief     Converts the block memory pointer, which points to the start of the block's
**            allocated memory, to the pointer where the actual block data starts.
** \param     memPtr Pointer to the start of the block's allocated memory.
** \return    Pointer to where the block's data starts if successful, NULL otherwise.
**
****************************************************************************************/
static void * TbxMemPoolBlockGetDataPtr(void * memPtr)
{
  void   * result = NULL;
  size_t * blockSizeArray;
  void   * dataPtr;

  /* Verify parameter. */
  TBX_ASSERT(memPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (memPtr != NULL)
  {
    /* Create a pointer to an array of size_t elements. */
    blockSizeArray = memPtr;
    /* The block data starts after the block size value. */
    dataPtr = &blockSizeArray[1U];
    /* Set the result value. */
    result = dataPtr;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockGetDataPtr ***/


/************************************************************************************//**
** \brief     Extract the size of the block data, given the pointer to the block's
**            allocated memory.
** \param     memPtr Pointer to the start of the block's allocated memory.
** \return    Size in bytes of the block's data if successful, 0 otherwise.
**
****************************************************************************************/
static size_t TbxMemPoolBlockGetBlockSize(void const * memPtr)
{
  size_t         result = 0;
  size_t const * blockSizeArray;
  size_t         blockSize;

  /* Verify parameter. */
  TBX_ASSERT(memPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (memPtr != NULL)
  {
    /* Create a pointer to an array of size_t elements. */
    blockSizeArray = memPtr;
    /* The block size value is located at the start of the block, */
    blockSize = blockSizeArray[0U];
    /* Set the result value. */
    result = blockSize;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockGetBlockSize ***/


/************************************************************************************//**
** \brief     Convert the pointer that points to where the block's data starts, to a
**            pointer that points to where the block's allocated memory starts.
** \param     dataPtr Pointer to the start of the block's data.
** \return    Pointer to the memory of the block if successful, NULL otherwise.
**
****************************************************************************************/
static void * TbxMemPoolBlockGetMemPtr(void * dataPtr)
{
  void   * result = NULL;
  void   * blockMemPtr;
  size_t * blockSizeArray;

  /* Verify parameter. */
  TBX_ASSERT(dataPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (dataPtr != NULL)
  {
    /* Create a pointer to an array of size_t elements. */
    blockSizeArray = dataPtr;
    /* Get pointer to the start of the block, which is one size_t element before the
     * pointer to the block's data.
     */
    blockMemPtr = &blockSizeArray[-1];
    /* Set the result value. */
    result = blockMemPtr;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockGetMemPtr ***/


/****************************************************************************************
*   B L O C K   L I S T   M A N A G E M E N T   F U N C T I O N S
****************************************************************************************/

/************************************************************************************//**
** \brief     Creates a new and empty linked list that can be used for building a list
**            with block nodes. Memory needed for the linked list object itself is
**            allocated on the heap.
** \return    Pointer to the newly created blocks list if successful, NULL otherwise.
**
****************************************************************************************/
static tBlockList * TbxMemPoolBlockListCreate(void)
{
  tBlockList * result;

  /* Attempt to create the linked list object on the heap. */
  result = TbxHeapAllocate(sizeof(tBlockList));
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
} /*** end of TbxMemPoolBlockListCreate ***/


/************************************************************************************//**
** \brief     Inserts the specified block node into the linked list with block nodes.
** \param     listPtr Pointer to the linked list with blocks.
** \param     nodePtr Pointer to the block node to insert.
**
****************************************************************************************/
static void TbxMemPoolBlockListInsert(tBlockList * listPtr, tBlockNode * nodePtr)
{
  /* Verify parameters. */
  TBX_ASSERT(listPtr != NULL);
  TBX_ASSERT(nodePtr != NULL);

  /* Only continue if the parameters are valid. */
  if ( (listPtr != NULL) && (nodePtr != NULL) )
  {
    /* Obtain mutual exclusive access to the linked list with block nodes. */
    TbxCriticalSectionEnter();
    /* Is the list currently empty? */
    if (*listPtr == NULL)
    {
      /* There is no next node. */
      nodePtr->nextNodePtr = NULL;
    }
    /* Linked list it not empty. */
    {
      /* Move the current begin of the list one down. */
      nodePtr->nextNodePtr = (*listPtr)->nextNodePtr;
    }
    /* Insert the new node at the start of the list. */
    *listPtr = nodePtr;
    /* Release mutual exclusive access to the linked list with block nodes. */
    TbxCriticalSectionExit();
  }
} /*** end of TbxMemPoolBlockListInsert ***/


/************************************************************************************//**
** \brief     Extracts a block node from the linked list with block nodes.
** \param     listPtr Pointer to the linked list with blocks.
** \return    Pointer to the block node that was extracted or NULL if the linked list
**            contained no more nodes.
**
****************************************************************************************/
static tBlockNode * TbxMemPoolBlockListExtract(tBlockList * listPtr)
{
  tBlockNode * result = NULL;

  /* Verify parameter. */
  TBX_ASSERT(listPtr != NULL);

  /* Only continue if the parameter is valid. */
  if (listPtr != NULL)
  {
    /* Obtain mutual exclusive access to the linked list with block nodes. */
    TbxCriticalSectionEnter();
    /* Only extract a node if the list currently not empty. */
    if (*listPtr != NULL)
    {
      /* Get the first node. */
      result = *listPtr;
      /* Is there currently only one node in the list? */
      if ((*listPtr)->nextNodePtr == NULL)
      {
        /* Set the list to be empty. */
        *listPtr = NULL;
      }
      /* There are currently at least two nodes in the list. */
      else
      {
        /* Make the second node the first one. */
        *listPtr = result->nextNodePtr;
      }
    }
    /* Release mutual exclusive access to the linked list with block nodes. */
    TbxCriticalSectionExit();
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMemPoolBlockListExtract ***/


/*********************************** end of tbxmempool.c *******************************/
