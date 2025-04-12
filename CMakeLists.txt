cmake_minimum_required (VERSION 3.5)

project(ancmp LANGUAGES C)

file(GLOB_RECURSE ANCMP_SRC ancmp/*.c)

add_library(ancmp ${ANCMP_SRC})
target_include_directories(ancmp PUBLIC ${PROJECT_SOURCE_DIR})

if (NOT WIN32)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(ancmp Threads::Threads m)
endif()

target_compile_definitions(ancmp PRIVATE LINKER_DEBUG=0)

if(MSVC)
    target_compile_definitions(ancmp PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

if (WIN32)
    target_link_libraries(ancmp ws2_32)
endif()