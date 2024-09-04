#------------------------------------------------
cmake_minimum_required(VERSION 3.20)

#------------------------------------------------
include("cmake/macros.cmake")
include("cmake/config.cmake")

#------------------------------------------------
set(CMAKE_C_STANDARD            99)
set(CMAKE_C_EXTENSIONS          ON)
set(CMAKE_C_STANDARD_REQUIRED   ON)

set(CMAKE_RULE_MESSAGES         ON)
set(CMAKE_AUTOGEN_VERBOSE       ON)
set(CMAKE_VERBOSE_MAKEFILE      ON)

#------------------------------------------------
enable_language(ASM C)
project(emblib32)

#------------------------------------------------
set(LIB_PATH      "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(TESTS_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(VENDOR_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

#------------------------------------------------
file(GLOB_RECURSE SOURCES_LIB
  "${LIB_PATH}/*.c"
)

file(GLOB_RECURSE SOURCES_VENDOR_UNITY
  "${VENDOR_PATH}/unity/*.c"
)

include_directories(
  "${LIB_PATH}"
  "${TESTS_PATH}"
  "${VENDOR_PATH}/unity"
)

#------------------------------------------------
add_executable("${PROJECT_NAME}_develop"    "${TESTS_PATH}/develop.c" ${SOURCES_LIB})

add_executable("${PROJECT_NAME}_test_cobs"  "${TESTS_PATH}/test_emblib32_cobs.c" ${SOURCES_LIB} ${SOURCES_VENDOR_UNITY})
