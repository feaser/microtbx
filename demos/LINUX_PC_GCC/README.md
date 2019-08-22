This demo program demonstrates how MicroTBX is integrated and used. It is targeted towards a GNU/Linux based PC and is configured for building with the GNU GCC toolchain.

# Prerequisites

The demo program relies on [CMake](https://cmake.org/) to automatically detect the build tools on your system and to generate the build environment for it. On a Debian/Ubuntu based system, the following command installs the build dependencies:

```
sudo apt-get install build-essential cmake
```

# Building

Using the terminal, set the working directory to the `./build` directory. Next, type the following two commands to detect and generate the Unix Makefiles based build environment and then build the demo program:

```
cmake ..
make
```

This creates the `microtbx_demo` executable in the `./binary` directory.
