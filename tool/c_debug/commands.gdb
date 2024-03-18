# command.gdb
set args "c_debug" "checker1=ref" "checker2=cmodel" "rnd_mode=2" "corner_all=reg" "type_all=fp32" "test_times=1" "mul" "assert" 
b tester.cpp:319
b tester.cpp:336

# debug reference model
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
b cal.cpp:46
b cmodel.cpp:267

run 
c
set in_a.sign=1
set in_a.expo=20
set in_a.mant=2536199
set in_b.sign=1
set in_b.expo=28
set in_b.mant=8139365 