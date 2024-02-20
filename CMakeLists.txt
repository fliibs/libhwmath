cmake_minimum_required(VERSION 3.22)

project(test)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp)
file(GLOB_RECURSE SIMV_FILES ${CMAKE_CURRENT_SOURCE_DIR}/simv)
set(CMAKE_CXX_STANDARD 11)

set(RND_MODE TEST_RND_NEAREST TEST_RND_ZERO TEST_RND_POS TEST_RND_NEG)
set(M_A_CASES TEST_ZERO TEST_INF TEST_ZERO_INF TEST_1_NAN TEST_REG)
set(FMA_CASES TEST_REG) 

if (NOT DEFINED DEBUG_C)
    set(DEBUG_C OFF)
endif()

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

enable_testing()
######debug set
if(DEBUG_C)
    set(debug_c "DEBUG_C")
else()
    set(debug_c)
endif()
#####FUNCTION 
#####fp32_model:ADD MUL FMA
#####test_model:RTL CMODEL
#####TEST_CORNER:M_A_CASES FMA_CASES 
function(generate_tests fp32_model test_model TEST_CORNER)
    if(test_model STREQUAL RTL)
        set(model_input TEST_RTL)
    else()
        set(model_input)
    endif()

    set(test_counter 1)

    foreach(namespace ${RND_MODE})
        foreach(case ${${TEST_CORNER}})
            if (case STREQUAL "TEST_ZERO")
                set(case_arg "in1=ZERO" "in2=REG" "in3=REG")
            elseif (case STREQUAL "TEST_INF")
                set(case_arg "in1=INF" "in2=REG" "in3=REG")
            elseif (case STREQUAL "TEST_ZERO_INF")
                set(case_arg "in1=ZERO" "in2=INF" "in3=REG")
            elseif (case STREQUAL "TEST_1_NAN")
                set(case_arg "in1=INF" "in2=REG" "in3=REG")
            elseif (case STREQUAL "TEST_REG")
                set(case_arg "in1=REG" "in2=REG" "in3=REG")
            endif()

            set(test_input)
            foreach(arg ${case_arg})
                list(APPEND test_input ${arg})
            endforeach()

            message(STATUS "test_input is ${test_input}")

            add_test(NAME "${fp32_model}_${test_model}_${test_counter}" COMMAND out ${namespace} ${test_input} TEST_MONTE ${fp32_model} ${model_input} ${test_times} ${debug_c})
            set_tests_properties("${fp32_model}_${test_model}_${test_counter}" PROPERTIES LABELS "${namespace}_${test_input}")
            math(EXPR test_counter "${test_counter} + 1")  

        endforeach(case ${CASE})
    endforeach(namespace ${RND_MODE})
endfunction()
    
#####TESTS GENRATES
if(NOT TEST_PARA)
    add_executable(out ${SRC})
    set(test_times TEST_TIMES=1)
    target_compile_definitions(out PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
    # generate_tests("MUL" "RTL" "M_A_CASES")
    # generate_tests("MUL" "CMODEL" "M_A_CASES")
    # generate_tests("ADD" "CMODEL" "M_A_CASES")
    # generate_tests("ADD" "RTL" "M_A_CASES")
    generate_tests("FMA" "CMODEL" "FMA_CASES")
endif()

####TEST the same corner for 16 times in parallel
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


