cmake_minimum_required(VERSION 3.1...3.19)

set(AubergineName aubergine)
set(AubergineVersion 1.0)
set(AubergineApi 1)

set(AubergineSrc "${CMAKE_CURRENT_LIST_DIR}/../src")

file(GLOB_RECURSE AubergineHeaders "${AubergineSrc}/aubergine/*.h")
file(GLOB_RECURSE AubergineSources "${AubergineSrc}/aubergine/*.cpp")

project(${AubergineName})

add_library(${AubergineName})

set_target_properties(
  ${AubergineName}
  PROPERTIES VERSION ${AubergineVersion})

set_target_properties(
  ${AubergineName}
  PROPERTIES SOVERSION ${AubergineApi})

set_property(
  TARGET ${AubergineName}
  PROPERTY INTERFACE_${AubergineName}_MAJOR_VERSION ${AubergineApi})

set_property(
  TARGET ${AubergineName} APPEND
  PROPERTY COMPATIBLE_INTERFACE_STRING ${AubergineName}_MAJOR_VERSION)

set_target_properties(
  ${AubergineName}
  PROPERTIES OUTPUT_NAME ${AubergineName})

target_sources(
  ${AubergineName}
  PRIVATE "${AubergineHeaders}" "${AubergineSources}")

set_target_properties(
  ${AubergineName}
  PROPERTIES PUBLIC_HEADER "${AubergineHeaders}")

target_include_directories(
  ${AubergineName}
  PUBLIC    "$<BUILD_INTERFACE:${AubergineSrc}>"
  INTERFACE "$<INSTALL_INTERFACE:include/${AubergineName}>")

target_compile_features(
  ${AubergineName}
  PUBLIC cxx_std_20)



# TODO easyloggingpp -Wno-range-loop-construct
# target_compile_options(${AubergineName} PUBLIC -Wall -Werror -Wno-range-loop-construct "$<$<CONFIG:RELEASE>:-Ofast>")

target_compile_options(${AubergineName} PUBLIC -Wall -Werror "$<$<CONFIG:RELEASE>:-Ofast>")

target_link_libraries(${AubergineName} PUBLIC android log)

find_package(oboe REQUIRED CONFIG)
target_link_libraries(${AubergineName} PUBLIC oboe::oboe)

target_link_libraries(${AubergineName} PUBLIC readerwriterqueue)
