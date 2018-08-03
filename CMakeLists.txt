cmake_minimum_required(VERSION 3.10)
project(clion_test)

set(CMAKE_CXX_STANDARD 17)

macro(option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT)
    if(NOT DEFINED ${OPTION_NAME})
        set(${OPTION_NAME} ${OPTION_DEFAULT})
    endif(NOT DEFINED ${OPTION_NAME})
    option(${OPTION_NAME} "${OPTION_STRING}" ${${OPTION_NAME}})
endmacro(option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT)

option_with_default(FOR_ANDROID "Build for android" OFF)

if(FOR_ANDROID)
    set(CMAKE_SYSTEM_NAME Android)
    set(CMAKE_ANDROID_STL_TYPE c++_static)

    if(NOT DEFINED ANDROID_NATIVE_API_LEVEL)
        set(CMAKE_SYSTEM_VERSION 23)
    else()
        set(CMAKE_SYSTEM_VERSION ${ANDROID_NATIVE_API_LEVEL})
    endif()

    if("$ENV{ANDROID_NDK}" STREQUAL "")
        message(FATAL_ERROR "Environment variable ANDROID_NDK not defined")
    endif()

    set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK})
    set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)

    if(NOT DEFINED CMAKE_ANDROID_ARCH_ABI)
        set(CMAKE_ANDROID_ARCH_ABI armeabi-v7a CACHE STRING "Android ABI (architecture)" FORCE)
    endif()
    set_property(CACHE CMAKE_ANDROID_ARCH_ABI PROPERTY STRINGS arm64-v8a armeabi-v7a armeabi mips mips64 x86 x86_64)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DSPDLOG_DEBUG_ON)
endif()
add_definitions(-DSPDLOG_TRACE_ON)

include(catch2.cmake)
include(json.cmake)
include(mem.cmake)
include(BoostAndroid.cmake)

option_with_default(SDK_BUILD_SHARED_LIB "Build shared SDK libs" OFF)
if(SDK_BUILD_SHARED_LIB)
    set(SDK_LIBRARY_TYPE SHARED)
else(SDK_BUILD_SHARED_LIB)
    set(SDK_LIBRARY_TYPE STATIC)
endif(SDK_BUILD_SHARED_LIB)

option_with_default(USE_MYSQL "using mysql database" ON)
if(USE_MYSQL)
    set(MYSQL_INC_DIR /usr/local/Cellar/mysql/8.0.12/include)
    set(MYSQL_LIB_DIR /usr/local/Cellar/mysql/8.0.12/lib)
    link_directories(${MYSQL_LIB_DIR})
endif(USE_MYSQL)

add_subdirectory(SDK)

include_directories(
        ${CATCH2_INC_DIR}
        ${JSON_INC_DIR}
        ${MEM_INC_DIR}
        ${BOOST_INC_DIR}
        ${SDK_INC_DIRS}
        if(USE_MYSQL)
            ${MYSQL_INC_DIR}
        endif(USE_MYSQL)
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
        SDK
        ${SDK_LIBRARY_TYPE}
        ${SDK_SOURCE_FILES}
)

if(USE_MYSQL)
    link_libraries(SDK mysqlclient)
else(USE_MYSQL)
    link_libraries(SDK)
endif(USE_MYSQL)

add_subdirectory(test)

add_executable(clion_test main.cpp)
target_link_libraries(clion_test ${Boost_LIBRARIES})
