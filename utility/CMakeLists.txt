###########################
### declare the appropriate sources and install rules based on driver selected
###########################
if(NOT SPIDEV_EXISTS)
    message(WARNING "Detecting /dev/spidev0.0 failed - continuing anyway. Please make sure SPI is enabled.")
endif()
install(FILES
        ${RF24_DRIVER}/includes.h
        ${RF24_DRIVER}/gpio.h
        ${RF24_DRIVER}/spi.h
        ${RF24_DRIVER}/compatibility.h
        ${RF24_DRIVER}/RF24_arch_config.h
        ${RF24_DRIVER}/interrupt.h
    DESTINATION include/RF24/utility/${RF24_DRIVER}
)
set(RF24_DRIVER_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/includes.h
    ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/gpio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/spi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/compatibility.cpp
    ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/RF24_arch_config.h
    ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/interrupt.cpp
    PARENT_SCOPE
)

# copy the includes file to the project source directory's utility folder
execute_process(COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/${RF24_DRIVER}/includes.h ${CMAKE_CURRENT_LIST_DIR}/includes.h)
