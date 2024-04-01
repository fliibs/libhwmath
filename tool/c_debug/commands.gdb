# command.gdb
set args  "--checker" "assert" "ref" "cmodel" "--op" "mul" "--test_times" "100000" "--type" "fp16" "--rnd_mode" "1" "--corner" "reg"
b tester.cpp:27
b cmodel_mul.cpp:200
b cmodel_mul.cpp:207
run
c
c
set in_a.sign=0
set in_a.expo=15
set in_a.mant=925
set in_b.sign=1
set in_b.expo=0
set in_b.mant=514