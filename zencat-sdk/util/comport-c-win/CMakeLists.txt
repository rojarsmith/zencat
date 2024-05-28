cmake_minimum_required(VERSION 3.29.0)
project(comport-c-win VERSION 0.1.0 LANGUAGES C CXX)

set (PROJECT_SOURCES
    main.c
    rs232.c
    esp32atagent.c
)

set (PROJECT_INCLUDES
    .
)

set (PROJECT_DEFINES
    #CASE1_RECEIVE
    CASE2_POST
)

include(CTest)
enable_testing()

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

add_executable(comport-c-win ${PROJECT_SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
