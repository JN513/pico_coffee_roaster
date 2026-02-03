cmake_minimum_required(VERSION 3.13)

include("$ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake")
include("$ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake")

project(pico_roster)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)


pico_sdk_init()

add_executable(pico_roster
    src/main.c
    src/temperature.c
    src/motor.c
    )

pico_generate_pio_header(pico_roster
    ${CMAKE_CURRENT_LIST_DIR}/src/triac.pio
)

# pull in common dependencies
target_link_libraries(pico_roster
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_pio
    hardware_clocks
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# 👉 AQUI É O PONTO PRINCIPAL
target_include_directories(pico_roster PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/include
    ${CMAKE_CURRENT_LIST_DIR}/lib
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include
)

pico_enable_stdio_usb(pico_roster 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_roster)


