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
/** \brief Linked list consisting of the linked lists created by the user. */
typedef tTbxList (* tTbxListList);


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Linked list that holds the linked lists created by the user. */
static tTbxListList tbxListList = NULL;


/************************************************************************************//**
** \brief     Creates a new and empty linked list and returns its pointer. Make sure to
**            store the pointer because it serves as a handle to the linked list, which
**            is needed when calling the other functions in this module.
** \return    The pointer to the created linked list.
**
****************************************************************************************/
tTbxList * TbxListCreate(void)
{
  tTbxList * result = NULL;

  /* TODO Implement TbxListCreate(). */

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
  TBX_UNUSED_ARG(list);

  /* Verify parameters. */
  TBX_ASSERT(list != NULL);

  /* Only continue if the parameter is valid. */
  if (list != NULL)
  {
    /* TODO Implement TbxListDelete(). */
  }

} /*** end of TbxListDelete ***/


/*********************************** end of tbxlist.c **********************************/
