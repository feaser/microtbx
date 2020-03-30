/************************************************************************************//**
* \file         tbxlist.h
* \brief        Linked lists header file.
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
#ifndef TBXLIST_H
#define TBXLIST_H

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Layout of a linked list node, which forms the building block of a linked list
 *         internally. Note that its elements should be considered private and only be
 *         accessed internally by this linked list module.
 */
typedef struct t_tbx_list_node
{
  /** \brief Pointer to the actual item stored in this node. */
  void                   * itemPtr;
  /** \brief Pointer to the previous node in the list or NULL if it is the list start. */
  struct t_tbx_list_node * prevNodePtr;
  /** \brief Pointer to the next node in the list or NULL if it is the list end. */
  struct t_tbx_list_node * nextNodePtr;
} tTbxListNode;

/** \brief Layout of a linked list. Its pointer serves as the handle to the linked list
 *         which is obtained after creation of the list and which is needed in the other
 *         functions of this module. Note that its elements should be considered private
 *         and only be accessed internally by this linked list module.
 */
typedef struct
{
  /** \brief Total number of nodes that are currently present in the linked list. */
  size_t         nodeCount;
  /** \brief Pointer to the first node of the linked list, also known as the head. */
  tTbxListNode * firstNodePtr;
  /** \brief Pointer to the last node of the linked list, also known as the tail. */
  tTbxListNode * lastNodePtr;
} tTbxList;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
tTbxList * TbxListCreate(void);
void       TbxListDelete(tTbxList * list);
void       TbxListClear(tTbxList * list);
size_t     TbxListGetSize(tTbxList const * list);
uint8_t    TbxListInsertItem(tTbxList * list, void * item);
uint8_t    TbxListInsertItemFront(tTbxList * list, void * item);
void       TbxListRemoveItem(tTbxList * list, void const * item);
void     * TbxListGetFirstItem(tTbxList const * list);
void     * TbxListGetLastItem(tTbxList const * list);
void     * TbxListGetNextItem(tTbxList const * list, void const * item);
void     * TbxListGetPreviousItem(tTbxList const * list, void const * item);
void       TbxListSwapItems(tTbxList const * list, void * item1, void * item2);

#ifdef __cplusplus
}
#endif

#endif /* TBXLIST_H */
/*********************************** end of tbxlist.h **********************************/
