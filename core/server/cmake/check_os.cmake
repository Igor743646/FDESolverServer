message(STATUS "-----------------------------------------------------")
message(STATUS "OS: ${CMAKE_SYSTEM_NAME} [${CMAKE_SYSTEM_VERSION}]")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "-----------------------------------------------------")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CC Compiler: ${CMAKE_C_COMPILER} [${CMAKE_C_COMPILER_VERSION}]")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER} [${CMAKE_CXX_COMPILER_VERSION}]")
message(STATUS "-----------------------------------------------------")
message(STATUS "Binary dir: ${CMAKE_BINARY_DIR}")
message(STATUS "Source dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "-----------------------------------------------------")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS_DEBUG_INIT "/Wall /EHa")
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "/Wall /O2 /arch:AVX512")

    # conan generated package configs
    set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O2")

    # conan generated package configs
    set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators/" "${CMAKE_PREFIX_PATH}")
else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "-----------------------------------------------------")
include("${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake")
message(STATUS "-----------------------------------------------------")
