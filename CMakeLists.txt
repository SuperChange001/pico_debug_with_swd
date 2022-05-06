cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (TARGET tinyusb_device)

    ################################################################################
    # creates i2c_bus_scan executable
    add_executable(test_led app/test_led.c)
    target_include_directories(test_led PUBLIC .)
    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(test_led pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(test_led 1)
    pico_enable_stdio_uart(test_led 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(test_led)
    ################################################################################

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()