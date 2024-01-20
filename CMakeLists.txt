cmake_minimum_required(VERSION 3.22)

project(test)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp)
file(GLOB_RECURSE SIMV_FILES ${CMAKE_CURRENT_SOURCE_DIR}/simv)
set(CMAKE_CXX_STANDARD 11)

set(RND_MODE TEST_RND_NEAREST TEST_RND_ZERO TEST_RND_POS TEST_RND_NEG)
set(CASES TEST_ONE TEST_INF TEST_ZERO_INF TEST_1_NAN TEST_REG) 

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_2)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_2)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build_2)

if (NOT DEFINED TEST_PARA)
    set(TEST_PARA OFF)
endif()

if (NOT DEFINED PARA_RND_MODE)
    set(PARA_RND_MODE TEST_RND_NEAREST)
endif()

if (NOT DEFINED PARA_CONNER)
    set(PARA_CONNER TEST_REG)
endif()

if (NOT DEFINED TEST_MODEL)
    set(TEST_MODEL TEST_MUL)
endif()

message(STATUS "simv_files is ${SIMV_FILES}")
message(STATUS "ROUND_MODE is ${ROUND_MODE}")

enable_testing()

#####Test RTL model 
if(NOT TEST_PARA)
    add_executable(out ${SRC})
    target_compile_definitions(out PRIVATE 
    TEST_TIMES=300000
    SPECIAL_TEST_TIMES=300000
    )
    target_compile_definitions(out PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
    if(TEST_MODEL STREQUAL "TEST_MUL")
        #CMODEL test
        foreach(namespace ${RND_MODE})
            foreach(case ${CASES})
                set(full_case "${namespace}_${case}_mul")
                    add_test(NAME C_MODEL_${full_case} COMMAND out ${namespace} ${case} TEST_MONTE TEST_MUL)
            endforeach()
        endforeach()

        #RTLMODEL test
        foreach(namespace ${RND_MODE})
            foreach(case ${CASES})
                set(full_case "${namespace}_${case}_mul")
                    add_test(NAME RTL_MODEL_${full_case} COMMAND out ${namespace} ${case} TEST_RTL TEST_MONTE TEST_MUL) 
            endforeach()
        endforeach()
    endif()
    #CMODEL test
    if(TEST_MODEL STREQUAL "TEST_ADD")
        foreach(namespace ${RND_MODE})
            foreach(case ${CASES})
                set(full_case "${namespace}_${case}_add")
                    add_test(NAME C_MODEL_${full_case} COMMAND out ${namespace} ${case} TEST_MONTE TEST_ADD)
            endforeach()
        endforeach()
    #RTLMODEL test
    # foreach(namespace ${RND_MODE})
    #     foreach(case ${CASES})
    #         set(full_case "${namespace}_${case}_mul")
    #             add_test(NAME RTL_MODEL_${full_case} COMMAND out ${namespace} ${case} TEST_RTL TEST_MONTE TEST_MUL) 
    #     endforeach()
    # endforeach()
    endif()
endif()

####TEST in parallel
if(TEST_PARA)
    add_executable(out ${SRC})
    target_compile_definitions(out PRIVATE 
    TEST_TIMES=300000
    SPECIAL_TEST_TIMES=100000
    )
    target_compile_definitions(out PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
    SET(UPPER_LIMIT 16)
    SET(CASES_P)
    foreach(i RANGE 1 ${UPPER_LIMIT})
        list(APPEND CASES_P "${PARA_RND_MODE}_${PARA_CONNER}_${i}")
    endforeach()
    foreach(case ${CASES_P})
        add_test(NAME PARA_${case} COMMAND out ${PARA_RND_MODE} ${PARA_CONNER} TEST_RTL TEST_MONTE)
    endforeach(case ${CASES_P})
endif(TEST_PARA)


