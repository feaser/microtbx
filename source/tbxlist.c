/************************************************************************************//**
* \file         tbxlist.c
* \brief        Linked lists source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2020 by Feaser     www.feaser.com     All rights reserved
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
/** \brief Layout of a linked list of lists. */
typedef struct t_tbx_list_list_node
{
  /** \brief Pointer to the actual list stored in this node. */
  tTbxList                    * listPtr;
  /** \brief Pointer to the previous node in the list or NULL if it is the list start. */
  struct t_tbx_list_list_node * prevNodePtr;
  /** \brief Pointer to the next node in the list or NULL if it is the list end. */
  struct t_tbx_list_list_node * nextNodePtr;
} tTbxListListNode;

/** \brief Linked list consisting of the linked lists created by the user. */
typedef tTbxListListNode (* tTbxListList);


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Linked list that holds the linked lists created by the user. */
static tTbxListList tbxListList = NULL;


/************************************************************************************//**
** \brief     Creates a new and empty linked list and returns its pointer. Make sure to
**            store the pointer because it serves as a handle to the linked list, which
**            is needed when calling the other API functions in this module.
** \return    The pointer to the created linked list.
**
****************************************************************************************/
tTbxList * TbxListCreate(void)
{
  tTbxList         * result = NULL;
  uint8_t            errorDetected = TBX_FALSE;
  tTbxListListNode * newListListNodePtr;
  tTbxList         * newListPtr;

  /* Check if this is the first time that this function is called. In this case the
   * pointer to the linked list that should hold the created lists still has a NULL
   * value.
   */
  if (tbxListList == NULL)
  {
    /* This module manages its own linked list of tTbxListListNode. Each tTbxListListNode
     * holds information about a tTbxList that was created with this function. A tTbxList
     * in turn is a linked list of tTbxListNode. Each tTbxListNode holds the actual
     * user create item pointer.
     * All these three items (tTbxListListNode, tTbxList and tTbxListNode) should support
     * dynamic allocation. Therefore three memory pools need to be created. An initial
     * size of 1 is sufficient. The plan is to expand the memory pools whenever more
     * blocks are needed.
     */
    if (TbxMemPoolCreate(1, sizeof(tTbxListListNode)) == TBX_ERROR)
    {
      /* Flag the error. */
      errorDetected = TBX_TRUE;
    }
    if (TbxMemPoolCreate(1, sizeof(tTbxList)) == TBX_ERROR)
    {
      /* Flag the error. */
      errorDetected = TBX_TRUE;
    }
    if (TbxMemPoolCreate(1, sizeof(tTbxListNode)) == TBX_ERROR)
    {
      /* Flag the error. */
      errorDetected = TBX_TRUE;
    }
  }

  /* Only continue if no error was detected so far. */
  if (errorDetected == TBX_FALSE)
  {
    /* Attempt to allocate a block for a node in the internal linked list. */
    newListListNodePtr = TbxMemPoolAllocate(sizeof(tTbxListListNode));
    /* In case the allocation failed, the memory pool could be exhausted. Try to add
     * another block to the memory pool. This works as long as there is enough heap
     * configured.
     */
    if (TbxMemPoolCreate(1, sizeof(tTbxListListNode)) == TBX_OK)
    {
      /* Second attempt of the block allocation. */
      newListListNodePtr = TbxMemPoolAllocate(sizeof(tTbxListListNode));
    }

    /* Attempt to allocate a block for the actual list that the node should hold. */
    newListPtr = TbxMemPoolAllocate(sizeof(tTbxList));
    /* In case the allocation failed, the memory pool could be exhausted. Try to add
     * another block to the memory pool. This works as long as there is enough heap
     * configured.
     */
    if (TbxMemPoolCreate(1, sizeof(tTbxList)) == TBX_OK)
    {
      /* Second attempt of the block allocation. */
      newListPtr = TbxMemPoolAllocate(sizeof(tTbxList));
    }

    /* Only continue if the allocations were successful. */
    if ( (newListListNodePtr != NULL) && (newListPtr != NULL) )
    {
      /* By default the created list is empty. */
      newListPtr->firstNodePtr = NULL;
      newListPtr->nodeCount = 0U;
      /* Initialize the node for the internal linked list. */
      newListListNodePtr->listPtr = newListPtr;
      newListListNodePtr->prevNodePtr = NULL;
      newListListNodePtr->nextNodePtr = NULL;

      /* Obtain mutual exclusive access to tbxListList. */
      TbxCriticalSectionEnter();
      /* Check if the internal linked list is not empty. */
      if (tbxListList != NULL)
      {
        /* The new node that is to be inserted will be added as the start and the current
         * start of the list should be moved down.
         */
        newListListNodePtr->nextNodePtr = tbxListList;
        newListListNodePtr->nextNodePtr->prevNodePtr = newListListNodePtr;
      }
      /* Insert the new node at the start of the list. */
      tbxListList = newListListNodePtr;
      /* Release mutual exclusive access of tbxListList. */
      TbxCriticalSectionEnter();

      /* The list was successfully created and stored in a node that was inserted into
       * the internal linked list. The only thing left to do is update the result
       * to give the pointer to the newly created list back to the caller. This pointer
       * serves as the handle to the list and is needed when calling API function of
       * this module.
       */
      result = newListPtr;
    }
  }

  /* Return the result to the caller. */
  return result;
} /*** end of TbxListCreate ***/


/************************************************************************************//**
** \brief     Deletes a previously created linked list. Afterwards, the pointer to the
**            linked list is no longer valid and should not be used anymore.
** \param     list Pointer to a previously created linked list to operate on.
**
****************************************************************************************/
void TbxListDelete(tTbxList * list)
{
  tTbxListListNode * currentListListNodePtr;

  /* Verify parameters. */
  TBX_ASSERT(list != NULL);

  /* Only continue if the parameter is valid. */
  if (list != NULL)
  {
    /* Obtain mutual exclusive access to tbxListList. */
    TbxCriticalSectionEnter();
    /* Get the pointer to the node at the head of the internal linked list. */
    currentListListNodePtr = tbxListList;
    /* Loop through the nodes to find the location of the list that is to be deleted. */
    while (currentListListNodePtr != NULL)
    {
      /* Does this node hold the list that is to be deleted? */
      if (currentListListNodePtr->listPtr == list)
      {
        /* Remove the node from the list. First check if it is the only node in the
         * list.
         */
        if ( (currentListListNodePtr->prevNodePtr == NULL) && \
             (currentListListNodePtr->nextNodePtr == NULL) )
        {
          /* Sanity check. This should also be the start of the list. */
          TBX_ASSERT(currentListListNodePtr == tbxListList);
          /* Set the internal linked list to empty. */
          tbxListList = NULL;
        }
        /* Check if it is at the start of the list. */
        else if (currentListListNodePtr->prevNodePtr == NULL)
        {
          /* Sanity check. This should be the start of the list. */
          TBX_ASSERT(currentListListNodePtr == tbxListList);
          /* Sanity check. There should be a next node. */
          TBX_ASSERT(currentListListNodePtr->nextNodePtr != NULL);
          /* Make the next node the new start of the list. */
          tbxListList = currentListListNodePtr->nextNodePtr;
        }
        /* Check if it is at the end of the list. */
        else if (currentListListNodePtr->nextNodePtr == NULL)
        {
          /* Sanity check. There should be a previous node. */
          TBX_ASSERT(currentListListNodePtr->prevNodePtr != NULL);
          /* Make the previous node the end of the list. */
          currentListListNodePtr->prevNodePtr->nextNodePtr = NULL;
        }
        /* If it is not the only node in the list, not at the start and not at the end,
         * then the list must have at least three nodes and the current node is somewhere
         * in the middle.
         */
        else
        {
          /* Sanity check. There should be a previous node. */
          TBX_ASSERT(currentListListNodePtr->prevNodePtr != NULL);
          /* Sanity check. There should be a next node. */
          TBX_ASSERT(currentListListNodePtr->nextNodePtr != NULL);
          /* Remove ourselves from the list. */
          currentListListNodePtr->prevNodePtr->nextNodePtr = currentListListNodePtr->nextNodePtr;
          currentListListNodePtr->nextNodePtr->prevNodePtr = currentListListNodePtr->prevNodePtr;
        }

        /* Clear the list. */
        TbxListClear(list);
        /* Release memory of the list. */
        TbxMemPoolRelease(list);
        /* Release memory of the list list node. */
        TbxMemPoolRelease(currentListListNodePtr);
        /* List deleted so there is no need to continue the loop. */
        break;
      }
      /* Continue with the next node in the following loop iteration. */
      currentListListNodePtr = currentListListNodePtr->nextNodePtr;
    }
    /* Release mutual exclusive access of tbxListList. */
    TbxCriticalSectionEnter();
  }
} /*** end of TbxListDelete ***/


/************************************************************************************//**
** \brief     Removes all items from the linked list. Keep in mind that it is the
**            caller's responsibility to release the memory of the items that were placed
**            in the list, before calling this function.
** \param     list Pointer to a previously created linked list to operate on.
**
****************************************************************************************/
void TbxListClear(tTbxList * list)
{
  tTbxListNode * currentListNodePtr;
  tTbxListNode * tempListNodePtr;

  /* Verify parameters. */
  TBX_ASSERT(list != NULL);

  /* Only continue if the parameter is valid. */
  if (list != NULL)
  {
    /* Obtain mutual exclusive access to the list. */
    TbxCriticalSectionEnter();
    /* Get the pointer to the node at the head of the internal linked list. */
    currentListNodePtr = list->firstNodePtr;
    /* Loop through the nodes to find the location of the list that is to be deleted. */
    while (currentListNodePtr != NULL)
    {
      /* Make a copy of the current node pointer, which will later be used to release its
       * allocated memory back to the memory pool.
       */
      tempListNodePtr = currentListNodePtr;
      /* Update the node pointer to continue with the next node in the following loop
       * iteration.
       */
      currentListNodePtr = currentListNodePtr->nextNodePtr;
      /* The node is now no longer needed and its memory can be released. */
      TbxMemPoolRelease(tempListNodePtr);
    }
    /* Set the list to empty now that the memory to each of its nodes was released. */
    list->firstNodePtr = NULL;
    list->nodeCount = 0U;
    /* Release mutual exclusive access of the list. */
    TbxCriticalSectionEnter();
  }
} /*** end of TbxListClear ***/


/************************************************************************************//**
** \brief     Obtains the number of items that are currently stored in the list.
** \param     list Pointer to a previously created linked list to operate on.
** \return    Total number of items currently stored in the list.
**
****************************************************************************************/
size_t TbxListGetSize(tTbxList const * list)
{
  size_t result = 0;

  /* Verify parameters. */
  TBX_ASSERT(list != NULL);

  /* Only continue if the parameter is valid. */
  if (list != NULL)
  {
    /* Obtain mutual exclusive access to the list. */
    TbxCriticalSectionEnter();
    /* Store the current number of items in the list in the result variable. */
    result = list->nodeCount;
    /* Release mutual exclusive access of the list. */
    TbxCriticalSectionEnter();
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxListGetSize ***/


/*********************************** end of tbxlist.c **********************************/
