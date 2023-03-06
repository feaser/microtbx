# Create interface library for hardware independent MicroTBX sources.
add_library(microtbx INTERFACE)

target_sources(microtbx INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxaes256.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxassert.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxchecksum.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxcritsect.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxcrypto.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxheap.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxlist.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmempool.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxplatform.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxrandom.c"
)

target_include_directories(microtbx INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source"
)

# Create interface library for Cortex-M port specific sources.
add_library(microtbx-cortexm INTERFACE)

target_sources(microtbx-cortexm INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM/tbxport.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM/GCC/tbxcomp.s"
)

target_include_directories(microtbx-cortexm INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM"
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM/GCC"
)

# Create interface library for Raspberry PI Pico RP2040 port specific sources.
add_library(microtbx-rp2040 INTERFACE)

target_sources(microtbx-rp2040 INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/RP2040/tbxport.c"
)

target_include_directories(microtbx-rp2040 INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/RP2040"
)

# Create interface library for Linux port specific sources.
add_library(microtbx-linux INTERFACE)

target_sources(microtbx-linux INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/LINUX/tbxport.c"
)

target_include_directories(microtbx-linux INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/LINUX"
)

# Create interface library for FreeRTOS extra sources.
add_library(microtbx-extra-freertos INTERFACE)

target_sources(microtbx-extra-freertos INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/freertos/tbxfreertos.c"
)

target_include_directories(microtbx-extra-freertos INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/freertos"
)

# Create interface library for C++ extra sources.
add_library(microtbx-extra-cpp INTERFACE)

target_sources(microtbx-extra-cpp INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus/tbxcxx.cpp"
)

target_include_directories(microtbx-extra-cpp INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus"
)

# Create interface library for the unit test specific sources.
add_library(microtbx-tests INTERFACE)

target_sources(microtbx-tests INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/tests/unittests.c"
)

target_include_directories(microtbx-tests INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/tests"
)

# Create interface library for the template. Only used for MISRA check.
add_library(microtbx-template INTERFACE)

target_include_directories(microtbx-template INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/template"
)

