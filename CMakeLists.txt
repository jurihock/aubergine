cmake_minimum_required(VERSION 3.1...3.19)

project(aubergine)

include("${CMAKE_CURRENT_LIST_DIR}/lib/cpm.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/easyloggingpp.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/fmt.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/readerwriterqueue.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/aubergine/aubergine.cmake")
