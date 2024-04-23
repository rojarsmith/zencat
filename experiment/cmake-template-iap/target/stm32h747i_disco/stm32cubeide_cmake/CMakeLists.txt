#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1 -j
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1 -j
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)

###################### CONSTANTS ######################################
set (PROJECT_TYPE_EXECUTABLE          "exe")
set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set (MCPU_CORTEX_M0				      "-mcpu=cortex-m0")
set (MCPU_CORTEX_M0PLUS				  "-mcpu=cortex-m0plus")
set (MCPU_CORTEX_M3				      "-mcpu=cortex-m3")
set (MCPU_CORTEX_M4				      "-mcpu=cortex-m4")
set (MCPU_CORTEX_M7				      "-mcpu=cortex-m7")
set (MCPU_CORTEX_M33				  "-mcpu=cortex-m33")
set (MCPU_CORTEX_M55				  "-mcpu=cortex-m55")
set (MCPU_CORTEX_M85				  "-mcpu=cortex-m85")
set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set (RUNTIME_LIBRARY_STD_C            "")
set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")
#######################################################################

###################### VARIABLES ######################################
set (PROJECT_NAME             "cmake-template-iap")
set (PROJECT_TYPE             "exe")
set (LINKER_SCRIPT            "../STM32H747XIHX_FLASH.ld")
set (MCPU                     "-mcpu=Cortex-M7")
set (MFPU                 "-mfpu=fpv5-d16")
set (MFLOAT_ABI               "")
set (RUNTIME_LIBRARY          "--specs=nano.specs")
set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")

set (SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../cmake-template-sdk")
set (BASE_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../")

set (PROJECT_SOURCES
	# LIST SOURCE FILES HERE
	startup_stm32h747xihx.s
	${BASE_PROJECT_DIR}/application/src/main.c
	${BASE_PROJECT_DIR}/application/src/stm32h7xx_hal_msp.c
	${BASE_PROJECT_DIR}/application/src/stm32h7xx_it.c
	${BASE_PROJECT_DIR}/application/src/syscalls.c
	${SDK_PATH}/stm32/drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.c
    ${SDK_PATH}/stm32/drivers/CMSIS/Device/ST/STM32H7xx/Source/Templates/system_stm32h7xx_dualcore_boot_cm4_cm7.c
	${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
	)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
    CORE_CM7
    STM32H747xx
    )

set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
    ${BASE_PROJECT_DIR}/application/inc
    ${SDK_PATH}/stm32/drivers/BSP/Components/is42s32800j
    ${SDK_PATH}/stm32/drivers/BSP/STM32H747I-DISCO
    ${SDK_PATH}/stm32/drivers/CMSIS/Include
    ${SDK_PATH}/stm32/drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Inc
    ${SDK_PATH}/stm32/drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    )

############ MODIFY ACCORDING TO REQUIREMENTS) ########################

#######################################################################

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM)

if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
elseif (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_STATIC_LIBRARY})
  add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set (CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=${PROJECT_NAME}.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
