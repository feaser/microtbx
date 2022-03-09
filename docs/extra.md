# Extra functionality

You can find some optional and extra functionality in the directory `source/extra`. This section provides additional information regarding this functionality.

## MicroTBX for FreeRTOS

The [FreeRTOS](https://www.freertos.org) real-time operating system is widely used in the embedded industry. No wonder, because it's free, open source, high quality, MISRA compliant and maintained by [AWS](https://aws.amazon.com/). 

Whenever you use both FreeRTOS and MicroTBX, a few pieces of functionality are redundant. Both MicroTBX and FreeRTOS offer heap management and assertion functionality. There is nothing wrong with using both. However, if you'd like to reap the benefits of MicroTBX, while also using FreeRTOS, consider using the files in directory:

* `source/extra/freertos/`

### Heap management

FreeRTOS ships with a few different examples for heap management. Some allow just one-time memory allocation, some feature functionality to release allocated memory again, with minimal memory fragmentation. You can find these examples in directory:

* `FreeRTOS/Source/portable/MemMang/`

The file `tbxfreertos.c` offers an alternative heap management implementation, using MicroTBX's memory pools. This allows you to dynamically allocate and release memory on the heap, for your application's FreeRTOS objects.

To use this solution heap management solution, you just need to remove the `heap_x.c` source file from your project and compile and link `tbxfreertos.c` instead.

### Assertions

In the FreeRTOS configuration header file `FreeRTOSConfig.h`, you can add and configure the `configASSERT` macro to enable assertions in the FreeRTOS code base. MicroTBX includes an assertion module that you can use for this. The easiest way to link the MicroTBX assertion `TBX_ASSERT` macro to the FreeRTOS `configASSERT` macro, is by including the `tbxfreertos.h` header file all the way at the end. Just before the last `#endif`:

```c
#define INCLUDE_vTaskCleanUpResources                 1
#define INCLUDE_vTaskSuspend                          1
#define INCLUDE_vTaskDelayUntil                       1
#define INCLUDE_vTaskDelay                            1
#define INCLUDE_uxTaskGetStackHighWaterMark           1

/* Use MicroTBX assertion in FreeRTOS. */
#include "tbxfreertos.h" /* <---- ADD THIS LINE */

#endif /* FREERTOS_CONFIG_H */
```

Just make sure to add the directory, where the `tbxfreertos.h` resides, to your compiler's search path for included header files.

Alternatively, you can directly add and configure the `configASSERT` macro as follows in `FreeRTOSConfig.h`:

```c
#include "microtbx.h"
#define configASSERT( x ) TBX_ASSERT( x )
```



