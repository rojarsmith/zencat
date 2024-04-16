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
set (PROJECT_NAME             "cmake-template-ltdc")
set (PROJECT_TYPE             "exe")
set (LINKER_SCRIPT            "../STM32H747XIHX_FLASH.ld")
set (MCPU                     "-mcpu=Cortex-M7")
set (MFPU                 "-mfpu=fpv5-d16")
set (MFLOAT_ABI               "")
set (RUNTIME_LIBRARY          "--specs=nano.specs")
set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")


set (PROJECT_SOURCES
	# LIST SOURCE FILES HERE
	../../../application/src/main.c
    ../../../application/src/stm32h7xx_hal_msp.c
	../../../application/src/stm32h7xx_it.c
	../../../application/src/syscalls.c
	../../../drivers/BSP/Components/adv7533/adv7533.c
	../../../drivers/BSP/Components/adv7533/adv7533_reg.c
	../../../drivers/BSP/Components/is42s32800j/is42s32800j.c
    ../../../drivers/BSP/Components/nt35510/nt35510.c
    ../../../drivers/BSP/Components/nt35510/nt35510_reg.c
	../../../drivers/BSP/Components/otm8009a/otm8009a.c
    ../../../drivers/BSP/Components/otm8009a/otm8009a_reg.c
    ../../../drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.c
	../../../drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.c
	../../../drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
	../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dsi.c
	../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
	../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c	
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    ../../../drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
    ../../../target/common/system_stm32h7xx.c
	startup_stm32h747xihx.s
	)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE

    )

set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
	../../../application/inc
    ../../../drivers/BSP/Components/adv7533
    ../../../drivers/BSP/Components/Common
	../../../drivers/BSP/Components/is42s32800j
    ../../../drivers/BSP/Components/nt35510
    ../../../drivers/BSP/Components/otm8009a
	../../../drivers/BSP/STM32H747I-DISCO
    ../../../drivers/CMSIS/Device/ST/STM32H7xx/Include
    ../../../drivers/CMSIS/Include
    ../../../drivers/STM32H7xx_HAL_Driver/Inc
    ../../../drivers/STM32H7xx_HAL_Driver/Inc/Legacy
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

add_definitions(
    -DCORE_CM7
    -DSTM32H747xx
)

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set (CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
