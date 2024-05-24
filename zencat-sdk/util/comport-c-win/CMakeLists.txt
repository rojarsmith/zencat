cmake_minimum_required(VERSION 3.29.0)
project(comport-c-win VERSION 0.1.0 LANGUAGES C CXX)

set (PROJECT_SOURCES
    main.c
    rs232.c
)

set (PROJECT_INCLUDES
    .
)

include(CTest)
enable_testing()

include_directories (${PROJECT_INCLUDES})

add_executable(comport-c-win ${PROJECT_SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
