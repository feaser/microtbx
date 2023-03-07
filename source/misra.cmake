# Create target for MISRA checking MicroTBX if cppcheck can be located.
find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    # Collect MicroTBX include directories.
    get_target_property(microtbx_incs microtbx INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX Cortex-M port include directories.
    get_target_property(microtbx_cortexm_incs microtbx-cortexm INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX RP2040 port include directories.
    get_target_property(microtbx_rp2040_incs microtbx-rp2040 INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX AVR port include directories.
    get_target_property(microtbx_avr_incs microtbx-avr INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX template include directories.
    get_target_property(microtbx_template_incs microtbx-template INTERFACE_INCLUDE_DIRECTORIES)

    # Build list with search paths for include files and transform by adding a "-I".
    set(search_path_incs)
    list(APPEND search_path_incs ${microtbx_incs})
    list(APPEND search_path_incs ${microtbx_cortexm_incs})
    list(APPEND search_path_incs ${microtbx_rp2040_incs})
    list(APPEND search_path_incs ${microtbx_avr_incs})
    list(APPEND search_path_incs ${microtbx_template_incs})
    list(TRANSFORM search_path_incs PREPEND "-I")

    # Collect MicroTBX sources.
    get_target_property(microtbx_srcs microtbx INTERFACE_SOURCES)
    # Collect MicroTBX Cortex-M port sources.
    get_target_property(microtbx_cortexm_srcs microtbx-cortexm INTERFACE_SOURCES)
    # Collect MicroTBX RP2040 port sources.
    get_target_property(microtbx_rp2040_srcs microtbx-rp2040 INTERFACE_SOURCES)
    # Collect MicroTBX AVR port sources.
    get_target_property(microtbx_avr_srcs microtbx-avr INTERFACE_SOURCES)

    # Build list with MicroTBX sources to check.
    set(check_srcs)
    list(APPEND check_srcs ${microtbx_srcs})
    list(APPEND check_srcs ${microtbx_cortexm_srcs})
    list(APPEND check_srcs ${microtbx_rp2040_srcs})
    list(APPEND check_srcs ${microtbx_avr_srcs})

    # Set variable pointing to the addon for configuring the MISRA checks.
    set(misra_addon "${CMAKE_CURRENT_LIST_DIR}/misra.json")

    # Build list with cppcheck commands, one for each given source file
    set(cppcheck_commands)
    foreach(sourcefile ${check_srcs})
        # only include C-source files
        if( sourcefile MATCHES \\.c$ )
            # create command line for running cppcheck on one source file and add it
            # to the list of commands
            list(APPEND cppcheck_commands
                COMMAND ${CMAKE_CXX_CPPCHECK}
                --platform=arm32-wchar_t2.xml
                --addon=${misra_addon}
                --relative-paths
                --error-exitcode=1
                --language=c
                --std=c11
                --inline-suppr 
                --suppress=missingIncludeSystem
                --suppress=unmatchedSuppression
                --enable=warning,style,information,missingInclude
                -DDEBUG
                ${search_path_incs}
                ${sourcefile})
        endif()
    endforeach(sourcefile)

    # add a custom target consisting of all the commands generated above
    add_custom_target(microtbx_MISRA ${cppcheck_commands} VERBATIM)
endif()
