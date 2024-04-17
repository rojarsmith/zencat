# ZenCat

The ZenCat project is a framework for fast developing STM32 IoT Device.

Support:

- Boot Loader, IAP
- CMake

## experiment/cmake-template

CMake template support STM32CubeIDE

Import --> Existing Projects into Workspace --> `experiment/cmake-template`

## experiment/cmake-template-iap

Project Properties-->C/C++ Build-->Other options

-DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake -DCMAKE_BUILD_TYPE=Debug -DSDK_PATH="C:/zencat/experiment/cmake-template-sdk"