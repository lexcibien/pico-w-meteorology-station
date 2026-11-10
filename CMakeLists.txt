# Pico W Meteorology Station

cmake_minimum_required(VERSION 3.35)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(MET_STATION DESCRIPTION "Projeto de uma estação meteorológica" LANGUAGES C CXX ASM)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

if(PICO_SDK_VERSION_STRING VERSION_LESS "2.3.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_library(met_compile_options INTERFACE)

# Compiler flags for the firmware target
target_compile_options(met_compile_options INTERFACE
  -Wall
  -fstack-usage
  -ffunction-sections
  -fdata-sections
  $<$<COMPILE_LANGUAGE:CXX>:
    -fno-exceptions
    -fno-unwind-tables
    -fno-rtti
    -fno-use-cxa-atexit
  >
)

target_compile_definitions(met_compile_options INTERFACE
  PICO_CXX_ENABLE_EXCEPTIONS=0
)

# Project libraries define their own sources, public headers and dependencies.
add_subdirectory(lib)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp)
add_executable(MET_STATION ${SRC_FILES})
target_include_directories(MET_STATION PRIVATE ${SRC_DIR})

target_compile_definitions(MET_STATION PRIVATE
  USBD_MANUFACTURER="Japatech Projects"
  USBD_PRODUCT="Meteorology Station"
  TINYUSB_NEED_POLLING_TASK
)

target_link_libraries(MET_STATION PRIVATE
  met_compile_options
  DHT_Sensor_Library
  RTC
  pico_stdlib
  pico_cyw43_arch_lwip_poll
  hardware_adc
)

pico_enable_stdio_usb(MET_STATION 1)
pico_enable_stdio_uart(MET_STATION 0)
pico_add_extra_outputs(MET_STATION)

add_custom_command(TARGET MET_STATION POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/compile_commands.json
  ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
)

add_custom_command(TARGET MET_STATION POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "=== Memory Usage Report ==="
  COMMAND ${Python3_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/scripts/memory_size.py
  --file $<TARGET_FILE:MET_STATION>
  --board ${PICO_BOARD}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM
)

set(FIRMWARE_NAME ${PROJECT_NAME})
add_custom_target(flash
  COMMAND picotool load -f -x
  ${CMAKE_BINARY_DIR}/${FIRMWARE_NAME}.bin -o 0x10000000
  DEPENDS ${FIRMWARE_NAME}
)
