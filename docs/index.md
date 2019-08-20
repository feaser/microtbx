MicroTBX is an open source Microcontroller ToolBoX consisting of software components commonly needed in embedded software applications. Think of it as a Swiss army knife for your firmware.

MicroTBX is written in the C programming language (C99). It currently supports all microcontrollers based on an ARM Cortex-M core, for example: ST STM32, Infineon XMC, and NXP S32K. There is little effort involved with porting MicroTBX to different microcontroller families.

While developing MicroTBX, special care was given to making the code threadsafe. Access to shared resources are serialized with the help of [critical sections](critsect.md). This make it safe to use MicroTBX in a multithreaded environment, for example when your embedded software program is built upon a pre-emptive RTOS.

If you do not feel like reinventing the wheel, consider using MicroTBX. Ideally, MicroTBX is integrated into your embedded software program at the start of each new project. The following image illustrates where MicroTBX fits into your firmware's architecture:

![alt text](images/software_architecture.png "Software architecture with MicroTBX")

# Components

The following table presents an overview of the currently supported software components inside MicroTBX:

| Name                                  | Description |
| :------------------------------------ | :---------- |
| [Assertions](assertions.md)           | For checking situations that should never happen. |
| [Critical Sections](critsect.md)      | For mutual exclusive access to shared resources. |
| [Heap](heap.md)                       | For static memory pre-allocation on the heap. |
| [Memory Pools](mempools.md)           | For pool based dynamic memory allocation on the heap. |
| [Random Numbers](random.md)           | For generating random numbers. |
| [Checksums](checksum.md)              | For calculating data checksums. |
| [Cryptography](crypto.md)             | For data encryption and decryption. |

# MISRA

MicroTBX was developed with [MISRA-C compliance](misra.md) in mind. This is a software development standard to facilitate best practices for programming safety-critical software in road vehicles and other embedded systems.

# Integration

Adding MicroTBX to your software project is a simple four step process:

1. Copy all the files from the `source`, the `source\third_party`, and the
   correct `source\port\XXX` port directory to your project.
2. Configure your project such that the added `.c` files are compiled and linked during
   a build.
3. Add the directories that contain the `.h` files to your compiler's include path.
4. Customize the MicroTBX configuraton in `tbx_conf.h`, if desired.

# Usage

1. Add the following line to each source-file, where you intend to make use of MicroTBX:
    ```
    #include "microtbx.h"
    ```

# Licensing

MicroTBX is licensed under the [MIT license](license.md). This permissive license enables you to include and distribute MicroTBX with your firmware, even if it is proprietary. The only requirements are that you include a copy of the MIT license and do not change the copyright notice, when you distribute MicroTBX.
