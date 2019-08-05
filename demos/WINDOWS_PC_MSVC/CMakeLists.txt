#****************************************************************************************
# \file         CMakeLists.txt
# \brief        CMake descriptor file for MicroTBX demo program for MS Visual C++.
# \internal
#----------------------------------------------------------------------------------------
#                          C O P Y R I G H T
#----------------------------------------------------------------------------------------
#   Copyright (c) 2019 by Feaser     www.feaser.com     All rights reserved
#
#----------------------------------------------------------------------------------------
#                            L I C E N S E
#----------------------------------------------------------------------------------------
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# \endinternal
#****************************************************************************************
# Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.8)


#****************************************************************************************
# Project configuration
#****************************************************************************************
# Specify the project name
project(microtbx_demo)

# Build debug version by default
set(CMAKE_BUILD_TYPE "Debug")


#****************************************************************************************
# Directories
#****************************************************************************************
# Set the output directory
set (PROJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../binary)

# Set the output directory for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_OUTPUT_DIRECTORY} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_OUTPUT_DIRECTORY} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_OUTPUT_DIRECTORY} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# Set MicroTBX related directory locations
set(MICROTBX_BASE ${PROJECT_SOURCE_DIR}/../../source)
set(MICROTBX_INCS
  "${MICROTBX_BASE}"
  "${MICROTBX_BASE}/port/WINDOWS"
  "${MICROTBX_BASE}/third_party/aes256"
)


#****************************************************************************************
# Compiler flags
#****************************************************************************************
# Set compiler specific macros
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")


#***************************************************************************************
# Includes
#****************************************************************************************
# Set include directories
include_directories("${PROJECT_SOURCE_DIR}" "${MICROTBX_INCS}")


#***************************************************************************************
# Files
#****************************************************************************************
# Collect MicroTBX sources and headers
file(GLOB MICROTBX_CORE_SRCS 
  "${MICROTBX_BASE}/*.h"
  "${MICROTBX_BASE}/*.c"
)
file(GLOB MICROTBX_PORT_SRCS
  "${MICROTBX_BASE}/port/WINDOWS/*.h"
  "${MICROTBX_BASE}/port/WINDOWS/*.c"
)
file(GLOB MICROTBX_THIRDPARTY_SRCS
  "${MICROTBX_BASE}/third_party/aes256/*.h"
  "${MICROTBX_BASE}/third_party/aes256/*.c"
)
set(MICROTBX_SRCS
  "${MICROTBX_CORE_SRCS}"
  "${MICROTBX_PORT_SRCS}"
  "${MICROTBX_THIRDPARTY_SRCS}"
)

# Add program sources
set(PROG_SRCS
  main.c
  "${MICROTBX_SRCS}"
)

#***************************************************************************************
# Targets
#****************************************************************************************
# Set main target. Use "make microtbx_demo" to individually build the program.
add_executable(
  microtbx_demo
  ${PROG_SRCS}
)

#*********************************** end of CMakeLists.txt ******************************
