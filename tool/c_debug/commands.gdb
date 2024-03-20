# command.gdb
set args "c_debug" "checker1=ref" "checker2=cmodel" "rnd_mode=1" "corner_all=reg" "type_all=fp32" "test_times=1" "mul" "assert" 
b tester.cpp:333
b tester.cpp:319

# debug reference model
b ref.cpp:9
b ref.cpp:10

#b ref.cpp:41
#b ref.cpp:50
#b ref.cpp:59
#b ref.cpp:62
#b if.cpp:19
#b if.cpp:20
#b if.cpp:21
#b ref.cpp:65

# debug cmodel
b cmodel.cpp:70
b cmodel.cpp:109
b cmodel.cpp:163
b cmodel.cpp:236
b cmodel.cpp:239
#b cal.cpp:46
b cmodel.cpp:267
b cmodel.cpp:335

run 
c

set in_a.sign=0
set in_a.expo=249
set in_a.mant=1773952
set in_b.sign=1
set in_b.expo=104
set in_b.mant=524288

