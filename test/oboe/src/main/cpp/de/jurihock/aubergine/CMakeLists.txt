cmake_minimum_required(VERSION 3.1...3.19)

project(de.jurihock.aubergine)

file(GLOB_RECURSE HDR "${CMAKE_CURRENT_LIST_DIR}/*.h")
file(GLOB_RECURSE SRC "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_library(${PROJECT_NAME} SHARED)
target_sources(${PROJECT_NAME} PRIVATE "${HDR}" "${SRC}")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../../..")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# TODO easyloggingpp -Wno-range-loop-construct
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror "$<$<CONFIG:RELEASE>:-Ofast>")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wno-range-loop-construct "$<$<CONFIG:RELEASE>:-Ofast>")

target_link_libraries(${PROJECT_NAME} PRIVATE android log)

find_package(oboe REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME} PRIVATE oboe::oboe)

include("${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../CMakeLists.txt")
target_link_libraries(de.jurihock.aubergine PRIVATE aubergine)
