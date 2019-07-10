MicroTBX is an open source Microcontroller ToolBoX consisting of software components commonly needed in embedded software applications. Think of it as a Swiss army knife for your firmware.

# Components

The following table presents an overview of the currently supported software components inside MicroTBX:

| Name                                  | Description |
| :------------------------------------ | :---------- |
| [Assertions](assertions.md)           | For checking situations that should never happen. |
| [Critical Sections](critsect.md)      | For mutual exclusive access to shared resources. |

# Integration

Adding MicroTBX to your software project is a simple four step process:

1. Copy the all the files from the `source\` and the correct source\port\XXX port directory to 
   your project.
2. Configure your project such that the added `.c` files are compiled and linked during
   a build.
3. Add the directory that contains the `.h` files to your compiler's include path.
4. Customize the MicroTBX configuraton in `tbx_conf.h`, if desired.

# Usage

1. Add the following line to each source-file, where you intend to make use of MicroTBX:
    ```
    #include "microtbx.h"
    ```
2. Call the function `TbxInit()` once during the initialization of your software program. This is
   typically somewhere before the infinite program loop is entered.
