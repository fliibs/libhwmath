# command.gdb
#set args  "checker1=ref" "checker2=cmodel" "rnd_mode=3" "corner1=zero" "corner2=inf" "corner3=reg" "type_all=fp32" "test_times=1000" "mul" "assert"
set args "--checker" "assert" "ref" "cmodel" "--op" "mul" "--test_times" "1" "--type" "fp32" "--rnd_mode" "3" "--corner" "zero" "inf"
# debug reference model
b tester.cpp:29
b cmodel_mul.cpp:39
#b checker.cpp:2
run 

#set in_a.sign=0
#set in_a.expo=0
#set in_a.mant=0
#set in_b.sign=1
#set in_b.expo=255
#set in_b.mant=1

