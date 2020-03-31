This section provides a quick reference with the functions and macros that MicroTBX offers.

# Generic

| Macro                             | Description |
| :-------------------------------- | :---------- |
| `TBX_VERSION_MAIN`                | Main version number of MicroTBX. |
| `TBX_VERSION_MINOR`               | Minor version number of MicroTBX. |
| `TBX_VERSION_PATCH`               | Patch number of MicroTBX. |
| `TBX_TRUE`                        | Boolean true value. |
| `TBX_FALSE`                       | Boolean false value. |
| `TBX_OK`                          | Generic okay value. |
| `TBX_ERROR`                       | Generic error value. |
| `TBX_UNUSED_ARG()`                | Function-like macro to flag a function parameter as unused. |

# Assertions

More information regarding this software component is found [here](assertions.md).

| Macro                             | Description |
| :-------------------------------- | :---------- |
| `TBX_ASSERT()`                    | Function-like macro to perform an assertion check. |

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxAssertSetHandler()`           | Register a custom assertion error handler. |

# Critical Sections

More information regarding this software component is found [here](critsect.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxCriticalSectionEnter()`       | Enters a critical section. |
| `TbxCriticalSectionExit()`        | Leaves a previously entered a critical section. |

# Heap

More information regarding this software component is found [here](heap.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxHeapAllocate()`               | Statically preallocates memory on the heap. |
| `TbxHeapGetFree()`                | Reports how many bytes are still available on the heap. |

# Memory Pools

More information regarding this software component is found [here](mempools.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxMemPoolCreate()`              | Creates or extends a memory pool of fixed sized blocks. |
| `TbxMemPoolAllocate()`            | Dynamically allocates memory from a memory pool. |
| `TbxMemPoolRelease()`             | Frees memory that was previously allocated from a memory pool. |

# Linked Lists

More information regarding this software component is found [here](lists.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxListCreate()`                 | Create a new an empty linked list. |
| `TbxListDelete()`                 | Delete a previously created linked list. |
| `TbxListClear()`                  | Remove all items from the linked list. |
| `TbxListGetSize()`                | Obtain the number of items stored in the linked list. |
| `TbxListInsertItem()`             | Add a new item at the end of the linked list. |
| `TbxListInsertItemFront()`        | Add a new item at the start of the linked list. |
| `TbxListRemoveItem()`             | Remove an item from the linked list. |
| `TbxListGetFirstItem()`           | Read the item at the start of the linked list. |
| `TbxListGetLastItem()`            | Read the item at the end of the linked list. |
| `TbxListGetNextItem()`            | Read the subsequent item from the linked list. |
| `TbxListGetPreviousItem()`        | Read the preceding item from the linked list. |
| `TbxListSwapItems()`              | Exchange two items in the linked list. |
| `TbxListSortItems()`              | Sort the items in the linked list. |

# Random Numbers

More information regarding this software component is found [here](random.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxRandomNumberGet()`            | Generate a 32-bit random number. |
| `TbxRandomSetSeedInitHandler()`   | Register a custom seed initialization handler. |

# Checksums

More information regarding this software component is found [here](checksum.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxChecksumCrc16Calculate()`     | Calculate the 16-bit CRC checksum over a block of data. |
| `TbxChecksumCrc32Calculate()`     | Calculate the 32-bit CRC checksum over a block of data. |

# Cryptography

More information regarding this software component is found [here](crypto.md).

| Function                          | Description |
| :-------------------------------- | :---------- |
| `TbxCryptoAes256Encrypt()`        | Performs AES encryption on a block of data using a 256-bit key. |
| `TbxCryptoAes256Decrypt()`        | Performs AES decryption on a block of data using a 256-bit key. |
