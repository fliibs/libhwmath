# command.gdb
set args  "checker1=ref" "checker2=cmodel" "rnd_mode=0" "corner1=zero" "corner2=inf" "corner3=reg" "type_all=fp16" "test_times=10000" "mul" "assert"

b cmodel_mul.cpp:103
b cmodel_mul.cpp:343