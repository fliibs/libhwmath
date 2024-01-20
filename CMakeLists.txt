cmake_minimum_required(VERSION 3.22)

project(test)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp)
file(GLOB_RECURSE SIMV_FILES ${CMAKE_CURRENT_SOURCE_DIR}/simv)

set(CMAKE_CXX_STANDARD 11)

set(RND_MODE TEST_RND_NEAREST TEST_RND_ZERO TEST_RND_POS TEST_RND_NEG)
set(CASES TEST_ONE TEST_INF TEST_ZERO_INF TEST_1_NAN TEST_REG) 

if (NOT DEFINED TEST_PARA)
    set(TEST_PARA OFF)
endif()

if (NOT DEFINED PARA_RND_MODE)
    set(PARA_RND_MODE TEST_RND_NEAREST)
endif()

if (NOT DEFINED PARA_CONNER)
    set(PARA_CONNER TEST_REG)
endif()
message(STATUS "simv_files is ${SIMV_FILES}")
message(STATUS "ROUND_MODE is ${ROUND_MODE}")

enable_testing()

#####Test RTL model 
if(NOT TEST_PARA)
    add_executable(mul ${SRC})
    target_compile_definitions(mul PRIVATE 
    TEST_TIMES=300000
    SPECIAL_TEST_TIMES=300000
    )
    target_compile_definitions(mul PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
    #CMODEL test
    foreach(namespace ${RND_MODE})
        foreach(case ${CASES})
            set(full_case "${namespace}_${case}")
                add_test(NAME C_MODEL_${full_case} COMMAND mul ${namespace} ${case} TEST_MONTE)
        endforeach()
    endforeach()
    #RTLMODEL test
    foreach(namespace ${RND_MODE})
        foreach(case ${CASES})
            set(full_case "${namespace}_${case}")
                add_test(NAME RTL_MODEL_${full_case} COMMAND mul ${namespace} ${case} TEST_RTL TEST_MONTE)
        endforeach()
    endforeach()
endif()

####TEST in parallel
if(TEST_PARA)
    add_executable(mul ${SRC})
    target_compile_definitions(mul PRIVATE 
    TEST_TIMES=300000
    SPECIAL_TEST_TIMES=100000
    )
    target_compile_definitions(mul PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
    SET(UPPER_LIMIT 16)
    SET(CASES_P)
    foreach(i RANGE 1 ${UPPER_LIMIT})
        list(APPEND CASES_P "${PARA_RND_MODE}_${PARA_CONNER}_${i}")
    endforeach()
    foreach(case ${CASES_P})
        add_test(NAME PARA_${case} COMMAND mul ${PARA_RND_MODE} ${PARA_CONNER} TEST_RTL TEST_MONTE)
    endforeach(case ${CASES_P})
endif(TEST_PARA)


