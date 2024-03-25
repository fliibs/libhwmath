# command.gdb
set args  "checker1=cmodel" "checker2=rtl" "rnd_mode=3" "corner1=nan" "corner2=reg" "corner3=reg" "type_all=fp32" "test_times=100" "mul" "assert"

# debug reference model
b tester.cpp:350
b tester.cpp:349
b tester.cpp:319
b tester.cpp:285
b tester.cpp:351

# debug cmodel
b cmodel.cpp:80
b cmodel.cpp:98
b cmodel.cpp:147
b cmodel.cpp:223
b cmodel.cpp:308
b cmodel.cpp:358




# debug pipe 
b pipe.cpp:42
b pipe.cpp:41
b pipe.cpp:44


run 
c

# set in_a.sign=0c
# set in_a.expo=249
# set in_a.mant=1773952
# set in_b.sign=1
# set in_b.expo=104
# set in_b.mant=524288

