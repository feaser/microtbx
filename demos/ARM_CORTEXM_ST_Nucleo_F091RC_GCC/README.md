This demo program demonstrates how MicroTBX is integrated and used. It is targeted towards an
ST [Nucleo-F091RC](https://www.st.com/en/evaluation-tools/nucleo-f091rc.html) board and is configured for building with the [GNU ARM Embedded](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) toolchain.

# Prerequisites

It is assumed that the ARM GCC Embedded toolchain in installed on the system. It can be downloaded from [here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm). Additionally, the GNU tools *make* and *rm* should be installed and available on the path. Windows users can install the [GNU MCU Eclipse Windows Build Tools](https://gnu-mcu-eclipse.github.io/windows-build-tools/) for this. On a Debian/Ubuntu based system, the GNU tools can be installed with the following command:

```
sudo apt-get install build-essential
```

# Configuration

The demo program is configured for building with a `makefile`. Before attempting to perform a build, update the location of the toolchain binaries (i.e. `arm-none-eabi-gcc`) in the `makefile`. It is set in variable `TOOL_PATH`:

```
TOOL_PATH=/opt/gcc-arm-none-eabi-5_4-2016q3/bin/
```

If the toolchain binaries are available on the path, then the `TOOL_PATH` variable can be left empty.

# Building

Using the terminal, set the working directory to the directory that has the `makefile` in it. Next, type the following command to build the demo program:

```
make clean all
```

This creates the `microtbx_demo.elf` executable in the `./binary` directory. This file can be programmed onto the ST Nucleo-F091RC board, using the on-board ST-Link debugger interface and with the help of the [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) software program.
