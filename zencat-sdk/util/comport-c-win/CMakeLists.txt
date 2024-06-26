cmake_minimum_required(VERSION 3.29.0)
project(comport-c-win VERSION 0.1.0 LANGUAGES C CXX)

set (PROJECT_SOURCES
    main.c
    rs232.c
)

set (PROJECT_INCLUDES
    C:/msys64/ucrt64/include
    .
)

set (PROJECT_DEFINES
    #CASE1_RECEIVE
    #CASE2_POST
    #CASE3_SLICE
    CASE4_SLICE2
)

if("CASE2_POST" IN_LIST PROJECT_DEFINES)
    list(APPEND PROJECT_SOURCES
        esp32atagent.c
    )
endif()
if("CASE3_SLICE" IN_LIST PROJECT_DEFINES)
    list(APPEND PROJECT_SOURCES
        atagent.c
    )
endif()
if("CASE4_SLICE2" IN_LIST PROJECT_DEFINES)
    list(APPEND PROJECT_SOURCES
        at.c
        at_cmd.c
    )
endif()
message("PROJECT_SOURCES=${PROJECT_SOURCES}")

# Check if the header file exists
if(EXISTS "${CMAKE_SOURCE_DIR}/main_dev.h")
    add_definitions(-DDEV_MODE=1)
else()
    add_definitions(-DDEV_MODE=0)
endif()

include(CTest)
enable_testing()

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

add_executable(comport-c-win ${PROJECT_SOURCES})

if("CASE4_SLICE2" IN_LIST PROJECT_DEFINES)
    target_link_libraries(comport-c-win regex)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
