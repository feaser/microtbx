# MicroTBX
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

MicroTBX is an open source Microcontroller ToolBoX consisting of software components commonly needed in embedded software applications. Think of it as a Swiss army knife for your firmware.

MicroTBX is written in the C programming language (C99) with MISRA compliance in mind. It currently supports all microcontrollers based on an ARM Cortex-M core, for example: ST STM32, Infineon XMC, and NXP S32K. There is little effort involved with porting MicroTBX to different microcontroller families.

While developing MicroTBX, special care was given to making the code threadsafe. Access to shared resources are serialized with the help of critical sections. This make it safe to use MicroTBX in a multithreaded environment, for example when your embedded software program is built upon a pre-emptive RTOS.

If you do not feel like reinventing the wheel, consider using MicroTBX. Ideally, MicroTBX is integrated into your embedded software program at the start of each new project. The following image illustrates where MicroTBX fits into your firmware's architecture:

![alt text](docs/images/software_architecture.png "Software architecture with MicroTBX")

# Getting Started

To get started with MicroTBX, it is recommended to download the latest stable release from the [releases](https://github.com/feaser/microtbx/releases/) page. Next, follow the integration steps outlined on the first page of the [user manual](https://feaser.github.io/microtbx).

# User manual

The online user manual is located at the following location:

* [https://feaser.github.io/microtbx](https://feaser.github.io/microtbx)

# Development

Development of MicroTBX takes place on GitHub. Feel free to contribute by submitting issues and pull requests.

* [https://github.com/feaser/microtbx/](https://github.com/feaser/microtbx/)

# Contact

Development and maintenance of MicroTBX is sponsered by Feaser. Feaser also offers integration/customization services, and professional technical support on a commercial basis:

* [https://www.feaser.com](https://www.feaser.com)
