cmake_minimum_required(VERSION 3.22)

project(test)
cmake_policy(SET CMP0140 NEW)
cmake_policy(SET CMP0144 NEW)

# to compile the rtl files
execute_process(
    COMMAND mkdir build_rtl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
    COMMAND sh -c "export LIBW_PATH=${CMAKE_CURRENT_SOURCE_DIR}/src/rtl && 	vcs -LDFLAGS -Wl,--no-as-needed +v2k -override_timescale=1ns/100ps -full64 -debug_all -sverilog -l compile.log -f ../src/rtl/tb/filelist.f +define+RTL_DEBUG -fsdb -cc gcc-4.8 -cpp g++-4.8"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_rtl
)

#cmodel and c verification platform
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/tool/*.cpp)

#rtl model
file(GLOB_RECURSE SIMV_FILES ${CMAKE_CURRENT_SOURCE_DIR}/simv)

set(CMAKE_CXX_STANDARD 11)

####input argc relevant
#set rnd mode
set(Rnd_Mode "--rnd_mode 1" "--rnd_mode 2"
"--rnd_mode 3" "--rnd_mode 0")
 
#set corner case
set(Corner_Case "--corner reg" "--corner zero reg"
"--corner inf reg" "--corner zero inf" 
"--corner nan reg")

#set fp type
set (Fp_type "--type fp16" "--type fp32" "--type fp64")

#set test_times 
if(NOT DEFINED test_times)
    set(test_times 1000)
endif()

#set checker 
set (Checker "--checker assert") 

#message 
# message(${SRC})

# message(${SIMV_FILES})

#set testing 
enable_testing()

#function to seperate string 
#cause ctest with add "" for the whole arg,which is unrecognizable for arg passer vector
function(split_string input_string delimiter store_list output_list)
    string(FIND "${input_string}" "${delimiter}" delimiter_position)
    if (delimiter_position GREATER -1)
        string(SUBSTRING "${input_string}" 0 ${delimiter_position} first_part)
        math(EXPR end_position "${delimiter_position} + 1")
        string(SUBSTRING "${input_string}" ${end_position} -1 remaining_part)
        list(APPEND $store_list "${first_part}")
        split_string("${remaining_part}" "${delimiter}" ${store_list} ${output_list})
    else()
        list(APPEND $store_list "${input_string}")
        set(${output_list} ${$store_list})
        set($store_list )
    endif()
    set(${output_list} "${${output_list}}" PARENT_SCOPE)
endfunction()

#set executable 
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(out_rnd_0 ${SRC})
target_link_libraries(out_rnd_0 Boost::program_options)
target_compile_definitions(out_rnd_0 PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
target_compile_options(out_rnd_0 PRIVATE "-g" "-DHALF_ERRHANDLING_FENV=1" "-lboost_program_options" "-DHALF_ROUND_STYLE=0")

#rnd_mode=3 towards_nearest 
add_executable(out_rnd_3 ${SRC})
target_link_libraries(out_rnd_3 Boost::program_options)
target_compile_definitions(out_rnd_3 PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
target_compile_options(out_rnd_3 PRIVATE "-g" "-DHALF_ERRHANDLING_FENV=1" "-lboost_program_options" "-DHALF_ROUND_STYLE=1")

#rnd_mode=2 towards_positive  
add_executable(out_rnd_2 ${SRC})
target_link_libraries(out_rnd_2 Boost::program_options)
target_compile_definitions(out_rnd_2 PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
target_compile_options(out_rnd_2 PRIVATE "-g" "-DHALF_ERRHANDLING_FENV=1" "-lboost_program_options" "-DHALF_ROUND_STYLE=2")

#rnd_mode=1 towards_negative
add_executable(out_rnd_1 ${SRC})
target_link_libraries(out_rnd_1 Boost::program_options)
target_compile_definitions(out_rnd_1 PRIVATE SIMV_EXECUTABLE_PATH="${SIMV_FILES}")
target_compile_options(out_rnd_1 PRIVATE "-g" "-DHALF_ERRHANDLING_FENV=1" "-lboost_program_options" "-DHALF_ROUND_STYLE=3")



function(generate_test untest_model op checker corner type test_time rnd_mode)
    set(store_list )

    if(untest_model STREQUAL rtl)
        set(checker_arg "${Checker} rtl cmodel")
    else()
        set(checker_arg "${Checker} ref cmodel")
    endif()
    
    set(op_arg "--op ${op}")
    
    set(test_times_arg "--test_times ${${test_time}}")
    
    split_string("${checker_arg}" " " "store_list" checker_list)
    split_string("${op_arg}" " " "store_list" op_list)
    split_string("${test_times_arg}" " " "store_list" test_times_list)

#     #fp type
    foreach(type ${${type}})
        set(test_counter 1)
        if(type STREQUAL "--type fp16")
            set (fp_mode fp16) 
        endif()
        if(type STREQUAL "--type fp32")
            set (fp_mode fp32) 
        endif()
        if(type STREQUAL "--type fp64")
            set (fp_mode fp64) 
        endif()

        #rnd_mode
        foreach(rnd ${${rnd_mode}})

            #corner case
            foreach(corner ${${corner}})
                #add tests
                split_string("${type}" " " "store_list" type_list)
                split_string("${rnd}" " " "store_list" rnd_list)
                split_string("${corner}" " " "store_list" corner_list)
                if(rnd STREQUAL "--rnd_mode 1")
                    add_test(NAME ${fp_mode}_${untest_model}_${op}_${test_counter} COMMAND  out_rnd_1 ${checker_list} ${op_list} ${test_times_list} ${type_list} ${rnd_list} ${corner_list}) 
                endif()
                if(rnd STREQUAL "--rnd_mode 0")
                    add_test(NAME ${fp_mode}_${untest_model}_${op}_${test_counter} COMMAND  out_rnd_0 ${checker_list} ${op_list} ${test_times_list} ${type_list} ${rnd_list} ${corner_list}) 
                endif()
                if(rnd STREQUAL "--rnd_mode 2")
                    add_test(NAME ${fp_mode}_${untest_model}_${op}_${test_counter} COMMAND  out_rnd_2 ${checker_list} ${op_list} ${test_times_list} ${type_list} ${rnd_list} ${corner_list}) 
                endif()
                if(rnd STREQUAL "--rnd_mode 3")
                    add_test(NAME ${fp_mode}_${untest_model}_${op}_${test_counter} COMMAND  out_rnd_3 ${checker_list} ${op_list} ${test_times_list} ${type_list} ${rnd_list} ${corner_list}) 
                endif()
                # message("${fp_mode}_${untest_model}_${op}_${test_counter}")
                # message("${checker_list} ${op_list} ${test_times_list} ${type_list} ${rnd_list} ${corner_list}")
                math(EXPR test_counter "${test_counter} + 1" OUTPUT_FORMAT DECIMAL)    
            endforeach()

        endforeach()
         
    endforeach()
endfunction()

generate_test("cmodel" "mul" "Checker" "Corner_Case" "Fp_type" "test_times" "Rnd_Mode")

include(CTestCustom.cmake)
