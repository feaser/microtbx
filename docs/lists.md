This software component consists of an easy-to-use set of functions for managing and
sorting items in a linked list. It builds upon the [memory pool](mempools.md)
functionality, which makes is possible to dynamically create, delete and automatically
grow linked lists in a convenient way. Think of a linked list as an array on steroids.
You do not yet have to know its size upon creation and you can insert, remove and
sort its items with ease.

A linked list is a sequence of data items, where each data item contains a connection
to the adjacent items. In this software component the definition of a data item is
a pointer to the data. The only thing you as the programmer need to take care of,
is that the data pointer actually points to valid data before inserting it into
the linked list.

Linked lists are commonly used data structures in software programs and they are
perfect for scenarios where you need to store a sequence of data, but you do not
yet know the maximum number of items to store ahead of time. An example would be a
first-in-first-out (FIFO) buffer.

# Usage

Linked list can be created and deleted at any time in the software program with
functions `TbxListCreate()` and `TbxListCreate().`. The `TbxListCreate()` returns
a pointer to the new list. This list pointer serves as a handle to the list. You
can pass this handle as a parameter to all the other functions in the software
component to identify the list that should be operated on.

Once the list is created, you can start adding items to the list with functions
`TbxListInsertItem()` and `TbxListInsertItemFrom()`, depending on where in the list
you would like to add the item. For reading items and for iterating over items,
the following functions are available: `TbxListGetFirstItem()`,
`TbxListGetLastItem()`, `TbxListGetNextItem()` and `TbxListGetPreviousItem()`.

At any given time, you can obtain the number of items that are stored in the list
with function `TbxListGetSize()`. Call function `TbxListRemoveItem()` to remove
a single item from the list, or call `TbxListClear()` to remove all items at once.

For editing the order of the items in the list, functions `TbxListSwapItems()` and
`TbxListSortItems()` are available. When calling `TbxListSortItems()` you can
specify your own function that will be called during the sort operation. In this
callback function you can implement your own application specific logic for
comparing two data items, therefore giving you full control and flexibility
over how the software works.

# Examples

TODO

# Configuration

The linked list software component itself does not have to be configured. However, when
creating memory pools with function `TbxMemPoolCreate()`, the data blocks are statically
preallocated on the heap with the help of the [heap](heap.md) module. In case the memory
pool creation fails, it is likely that the heap size needs to be increased using the
macro `TBX_CONF_HEAP_SIZE`:

```c
/** \brief Configure the size of the heap in bytes. */
#define TBX_CONF_HEAP_SIZE                       (2048U)
```
