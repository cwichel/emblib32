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
set(DEVELOP_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/develop")
set(LIB_PATH      "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(TESTS_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(VENDOR_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

#------------------------------------------------
set(INCLUDES_DEVELOP
  "${DEVELOP_PATH}"
)
file(GLOB_RECURSE SOURCES_DEVELOP
  "${DEVELOP_PATH}/*.c"
)

set(INCLUDES_LIB
    "${LIB_PATH}"
)
file(GLOB_RECURSE SOURCES_LIB
  "${LIB_PATH}/*.c"
)

set(INCLUDES_TESTING
  "${TESTS_PATH}"
  "${VENDOR_PATH}/unity"
)
file(GLOB_RECURSE SOURCES_TESTING
  "${VENDOR_PATH}/unity/*.c"
)

#------------------------------------------------
add_executable("${PROJECT_NAME}_develop" ${SOURCES_LIB} ${SOURCES_DEVELOP})
target_include_directories("${PROJECT_NAME}_develop" PRIVATE ${INCLUDES_LIB} ${INCLUDES_DEVELOP})

#------------------------------------------------
add_executable("${PROJECT_NAME}_test_bitmask"  "${TESTS_PATH}/test_emblib32_bitmask.c" ${SOURCES_LIB} ${SOURCES_TESTING})
target_include_directories("${PROJECT_NAME}_test_bitmask" PRIVATE ${INCLUDES_LIB} ${INCLUDES_TESTING})

add_executable("${PROJECT_NAME}_test_buffer"  "${TESTS_PATH}/test_emblib32_buffer.c" ${SOURCES_LIB} ${SOURCES_TESTING})
target_include_directories("${PROJECT_NAME}_test_buffer" PRIVATE ${INCLUDES_LIB} ${INCLUDES_TESTING})

add_executable("${PROJECT_NAME}_test_cobs"  "${TESTS_PATH}/test_emblib32_cobs.c" ${SOURCES_LIB} ${SOURCES_TESTING})
target_include_directories("${PROJECT_NAME}_test_cobs" PRIVATE ${INCLUDES_LIB} ${INCLUDES_TESTING})
