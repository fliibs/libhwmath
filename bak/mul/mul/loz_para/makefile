CXX := g++
FILELIST := @filelist.txt

compile_test:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=1 -Dtest_times=1 -Dspecial_test_time=10 	-Dtest_times=100000
compile_debug:
	$(CXX) -o mul $(FILELIST) -DTEST_DEBUG=1 -DDEBUG_ENABLE=1 
run:
	./mul
clean:
	rm mul 
