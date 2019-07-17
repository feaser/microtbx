Static code analysis was performed to verify compliance with MISRA-C 2012. The following table lists the compliance exceptions:

| Directive | Type     | Rationale |
| :-------: | :------: | :-------- |
| 4.9       | advisory | Function-like macros can provide clarity and allow easy to understand compile time configurations. |
| 11.4      | advisory | Conversions between a pointer and integer type are needed when accessing hardware registers or other direct memory map accesses. |
| 11.5      | advisory | Conversions from pointer to void to pointer to other type. This is needed after allocating memory from the heap and then initialising a pointer to point to this allocated memory. Used for example when allocating memory to build a linked list. |

