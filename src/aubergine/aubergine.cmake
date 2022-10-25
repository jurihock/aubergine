file(GLOB_RECURSE HDR "${CMAKE_CURRENT_LIST_DIR}/*.h")
file(GLOB_RECURSE SRC "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE "${HDR}" "${SRC}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/..")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# TODO easyloggingpp -Wno-range-loop-construct
# target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Werror "$<$<CONFIG:RELEASE>:-Ofast>")
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Werror -Wno-range-loop-construct "$<$<CONFIG:RELEASE>:-Ofast>")

target_link_libraries(${PROJECT_NAME} PUBLIC android log)

find_package(oboe REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME} PUBLIC oboe::oboe)

target_link_libraries(${PROJECT_NAME} PUBLIC easyloggingpp fmt readerwriterqueue)
