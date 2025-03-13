cmake_minimum_required (VERSION 3.5)

project(ancmp LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE ANCMP_SRC ancmp/*.c)

add_library(ancmp ${ANCMP_SRC})
target_include_directories(ancmp PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(ancmp Threads::Threads m)

target_compile_definitions(ancmp PRIVATE ANDROID_X86_LINKER LINKER_DEBUG=0)

if (WIN32)
    target_link_libraries(ancmp ws2_32)
endif()