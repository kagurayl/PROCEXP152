cmake_minimum_required(VERSION 3.25)
project(PROCEXP152 LANGUAGES C)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "PROCEXP152 requires MSVC targeting Windows x64.")
endif()
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dev/cmake")
find_package(WDK REQUIRED)
wdk_add_driver(PROCEXP152 WINVER 0x0601 NTDDI_VERSION 0x06010000
    src/procexp_entry.c src/procexp_recovered.c src/procexp_data.c src/procexp_names.c
    src/procexp_compat.h src/procexp_globals.h src/procexp_functions.h)
set_target_properties(PROCEXP152 PROPERTIES C_STANDARD 17 C_STANDARD_REQUIRED YES
    MSVC_RUNTIME_LIBRARY MultiThreaded)
target_include_directories(PROCEXP152 PRIVATE src)
target_link_libraries(PROCEXP152 WDK::WDMSEC)
target_compile_options(PROCEXP152 PRIVATE /GS /Ob0 /Z7)
# Retain FindWDK's GsDriverEntry wrapper for security-cookie initialization.
# It replaces the original CRT startup wrapper; DriverEntry remains the driver logic.
target_link_options(PROCEXP152 PRIVATE "/MAP:$<TARGET_FILE_DIR:PROCEXP152>/PROCEXP152.map")
