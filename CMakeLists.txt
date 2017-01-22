project(qcc)
cmake_minimum_required(VERSION 2.8)

############################################################
# Options
############################################################

option(QCC_GEN_COVERAGE "Generate tests coverage information" OFF)

############################################################
# General config
############################################################

add_definitions(-Wall -Wextra -Werror)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99")

if(QCC_GEN_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
endif(QCC_GEN_COVERAGE)

############################################################
# qcc
############################################################

include_directories(src)
aux_source_directory(src SRC_LIST)

add_library(qcc SHARED ${SRC_LIST})
link_directories(${PROJECT_BINARY_DIR})

############################################################
# Tests
############################################################

include_directories(test)
aux_source_directory(test TEST_LIST)

add_executable(test_qcc ${TEST_LIST})
add_dependencies(test_qcc qcc)
target_link_libraries(test_qcc -lqcc)

if(QCC_GEN_COVERAGE)
    add_custom_target(tests
        COMMAND cppcheck ${PROJECT_SOURCE_DIR}/src/*
        COMMAND find CMakeFiles/qcc.dir/ -name *.gcda -type f -delete || true
        COMMAND rm -rvf coverage*
        COMMAND valgrind ./test_qcc
        COMMAND lcov --capture --directory CMakeFiles/qcc.dir/ --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory coverage
        DEPENDS test_qcc)
else(QCC_GEN_COVERAGE)
    add_custom_target(tests
        COMMAND cppcheck ${PROJECT_SOURCE_DIR}/src/*
        COMMAND valgrind ./test_qcc
        DEPENDS test_qcc)
endif(QCC_GEN_COVERAGE)
