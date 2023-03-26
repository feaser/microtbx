# Create interface library for hardware independent MicroTBX sources.
add_library(microtbx INTERFACE)

target_sources(microtbx INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_aes256.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_assert.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_checksum.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_critsect.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_crypto.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_heap.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_list.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_mempool.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_platform.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbx_random.c"
)

target_include_directories(microtbx INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source"
)

# Create interface library for Cortex-M port specific sources.
add_library(microtbx-cortexm INTERFACE)

target_sources(microtbx-cortexm INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM/tbx_port.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM/GCC/tbx_comp.s"
)

target_include_directories(microtbx-cortexm INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM"
    "${CMAKE_CURRENT_LIST_DIR}/source/port/ARM_CORTEXM/GCC"
)

# Create interface library for Raspberry PI Pico RP2040 port specific sources.
add_library(microtbx-rp2040 INTERFACE)

target_sources(microtbx-rp2040 INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/RP2040/tbx_port.c"
)

target_include_directories(microtbx-rp2040 INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/RP2040"
)

# Create interface library for AVR port specific sources.
add_library(microtbx-avr INTERFACE)

target_sources(microtbx-avr INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/AVR/GCC/tbx_port.c"
)

target_include_directories(microtbx-avr INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/AVR"
    "${CMAKE_CURRENT_LIST_DIR}/source/port/AVR/GCC"
)

# Create interface library for Linux port specific sources.
add_library(microtbx-linux INTERFACE)

target_sources(microtbx-linux INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/port/LINUX/tbx_port.c"
)

target_include_directories(microtbx-linux INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/port/LINUX"
)

# Create interface library for FreeRTOS extra sources.
add_library(microtbx-extra-freertos INTERFACE)

target_sources(microtbx-extra-freertos INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/freertos/tbx_freertos.c"
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

