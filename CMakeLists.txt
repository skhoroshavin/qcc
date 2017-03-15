project(qcc)
cmake_minimum_required(VERSION 2.8)

############################################################
# Options
############################################################

option(QCC_GEN_COVERAGE "Generate tests coverage information" OFF)
option(QCC_VALGRIND "Run tests under valgrind" OFF)
option(QCC_SANITIZER "Compile with sanitizer support" OFF)

############################################################
# General config
############################################################

if(NOT MSVC)
    add_definitions(-Wall -Wextra -Werror)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99")
endif(NOT MSVC)

if(QCC_GEN_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
endif(QCC_GEN_COVERAGE)

if(QCC_VALGRIND)
    add_definitions(-DQCC_VALGRIND)
endif(QCC_VALGRIND)

if(QCC_SANITIZER)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif(QCC_SANITIZER)

############################################################
# qcc
############################################################

include_directories(src)
aux_source_directory(src SRC_LIST)

add_library(qcc STATIC ${SRC_LIST})
link_directories(${PROJECT_BINARY_DIR})

############################################################
# Tests
############################################################

include(CTest)
enable_testing()

if(NOT WIN32)
    if(QCC_GEN_COVERAGE)
        set(QCC_PRE_TEST pre_test_coverage.sh)
        set(QCC_POST_TEST post_test_coverage.sh)
    else(QCC_GEN_COVERAGE)
        set(QCC_PRE_TEST pre_test.sh)
        set(QCC_POST_TEST post_test.sh)
    endif(QCC_GEN_COVERAGE)
    configure_file(CTestCustom.cmake ${CMAKE_BINARY_DIR}/CTestCustom.cmake)
endif(NOT WIN32)

if(QCC_VALGRIND)
    macro(qcc_test test_name)
        add_executable(${test_name} test/${test_name}.c)
        add_dependencies(${test_name} qcc)
        target_link_libraries(${test_name} qcc)
        add_test(${test_name} valgrind --error-exitcode=1 ./${test_name})
    endmacro(qcc_test)
else(QCC_VALGRIND)
    macro(qcc_test test_name)
        add_executable(${test_name} test/${test_name}.c)
        add_dependencies(${test_name} qcc)
        target_link_libraries(${test_name} qcc)
        add_test(${test_name} ${test_name})
    endmacro(qcc_test)
endif(QCC_VALGRIND)

qcc_test(test_arena)
qcc_test(test_common)
qcc_test(test_context)
qcc_test(test_engine)
qcc_test(test_interval)
qcc_test(test_logger)
qcc_test(test_qcc_uint)
