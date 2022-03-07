# MISRA compliance

Static code analysis was performed to verify compliance with MISRA-C 2012. The following table lists the compliance exceptions:

| Directive |   Type   | Rationale                                                    |
| :-------: | :------: | :----------------------------------------------------------- |
|    4.9    | advisory | Function-like macros can provide clarity and allow easy to understand<br>compile time configurations. |
|   11.5    | advisory | Conversions from pointer to void to pointer to other type. This is needed<br>after allocating memory from the heap and then initializing a pointer to<br>point to this allocated memory. Used for example when allocating memory<br>to build a linked list. |

