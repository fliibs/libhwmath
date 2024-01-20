CXX := g++
FILELIST := @filelist.txt

compile_test:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=1 -Dtest_times=1 -Dspecial_test_time=10 	-Dtest_times=100000
compile_debug:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=0 -DDEBUG_ENABLE=0 -Dspecial_test_time=0 -DTEST_DEBUG=1
run:
	./mul
