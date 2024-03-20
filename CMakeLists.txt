cmake_minimum_required(VERSION 3.22)

project(test)

#cmodel and c verification platform
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/tool/*.cpp)

#rtl model
file(GLOB_RECURSE SIMV_FILES ${CMAKE_CURRENT_SOURCE_DIR}/simv)

set(CMAKE_CXX_STANDARD 11)

####input argc relevant
#set rnd mode
set(Rnd_Mode Rnd_Nearest Rnd_Zero Rnd_Pos Rnd_Neg)

#set corner case
set(Corner_Case 1_ZERO 1_INF ZERO_INF 1_NAN All_REG)

#set fp type
set (Fp_type fp64 fp32 fp16)
# set (Fp_type fp64)

#set test_times 
if(NOT DEFINED test_times)
    set(test_times 1000000)
endif()

set(test_times_arg "test_times=${test_times}")

#message 
message(${SRC})

message(${SIMV_FILES})

message(${test_times_arg})


#set testing 
enable_testing()

#set executable 
add_executable(out ${SRC})

target_compile_definitions(out PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
target_compile_options(out PRIVATE "-g")

#function
function(list_to_str list_var)
    set(str_var)
    foreach(list_item ${${list_var}})
        list(APPEND str_var ${list_item})
        set(str_var_out ${str_var} PARENT_SCOPE)
    endforeach()
endfunction()

function(map_rnd_mode rnd_mode)
    if (rnd_mode STREQUAL "Rnd_Nearest")
        set(arg "rnd_mode=3")
    elseif (rnd_mode STREQUAL "Rnd_Zero")
        set(arg "rnd_mode=0")
    elseif (rnd_mode STREQUAL "Rnd_Pos")
        set(arg "rnd_mode=1")
    elseif (rnd_mode STREQUAL "Rnd_Neg")
        set(arg "rnd_mode=2")
    else()
        message(FATAL_ERROR "Invalid rnd mode: ${rnd}")
    endif()
    set(rnd_arg ${arg} PARENT_SCOPE)
endfunction()

function(map_corner corner )
    if (corner STREQUAL "1_ZERO")
        set(arg "corner1=zero" "corner2=reg" "corner3=reg")
    elseif (corner STREQUAL "1_INF")
        set(arg "corner1=inf" "corner2=reg" "corner3=reg")
    elseif (corner STREQUAL "ZERO_INF")
        set(arg "corner1=zero" "corner2=inf" "corner3=reg")
    elseif (corner STREQUAL "1_NAN")
        set(arg "corner1=nan" "corner2=reg" "corner3=reg")
    elseif (corner STREQUAL "All_REG")
        set(arg "corner_all=reg")
    endif()
    set(corner_arg ${arg} PARENT_SCOPE)
endfunction()

function(map_type type)
    if (type STREQUAL "fp64")
        set(arg "type_all=fp64")
    elseif (type STREQUAL "fp32")
        set(arg "type_all=fp32")
    elseif (type STREQUAL "fp16")
        set(arg "type_all=fp16")
    else()
        message(STATUS "no input type arg")
    endif()
    set(type_arg ${arg} PARENT_SCOPE)
endfunction()

function(generate_test fp_mode test_model test_rnd test_corner test_type)
    if(test_model STREQUAL rtl)
        set(checker_arg "checker1=cmodel" "checker2=rtl")
    else()
        set(checker_arg "checker1=ref" "checker2=cmodel")
    endif()
    list_to_str("checker_arg")
    set(checker_input ${str_var_out})

    #fp type
    foreach(type ${${test_type}})
        map_type(${type})
        set(test_counter 1)

        #rnd_mode
        foreach(rnd ${${test_rnd}})
            map_rnd_mode(${rnd})

            #corner case
            foreach(corner ${${test_corner}})
                map_corner(${corner})
                list_to_str("corner_arg")
                set(corner_input ${str_var_out})

                #add tests
                add_test(NAME ${fp_mode}_${test_model}_${type}_${test_counter} COMMAND out ${checker_input} ${rnd_arg} ${corner_input} ${type_arg} ${test_times_arg} ${fp_mode} "assert")
                # message(STATUS "${fp_mode}_${test_model}_${type}_${test_counter}")
                # message(STATUS "${checker_input} ${rnd_arg} ${corner_input} ${type_arg} ${test_times_arg} ${test_model} assert")
                math(EXPR test_counter "${test_counter} + 1" OUTPUT_FORMAT DECIMAL)
                endforeach()
            endforeach() 
    endforeach()
endfunction()

generate_test("mul" "cmodel" "Rnd_Mode" "Corner_Case" "Fp_type")
include(CTestCustom.cmake)
 