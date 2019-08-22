This demo program demonstrates how MicroTBX is integrated and used. It is targeted towards a Microsoft Windows based PC and is configured for building with the Microsoft Visual C++ software development environment.

# Prerequisites

The demo program relies on [CMake](https://cmake.org/) to automatically detect the build tools on your system and to generate the build environment for it. The following instructions assume that CMake is installed on your system and is available on the path.

# Building

Using the Command Prompt in Windows, set the working directory to the `.\build` directory and run the command:

```
cmake ..
```

A solution file for Microsoft Visual C++ is then automatically generated, called `microtbx_demo.sln`. Open the solution in Microsoft Visual C++ and build the program from there, by selecting *Build->Rebuild Solution* from the menu. This creates the `microtbx_demo` executable in the `.\binary` directory.
