CXX := g++
FILELIST := @filelist.txt

compile_test_n:
	$(CXX) -o mul_n $(FILELIST) -DTEST_MONTE=1 -DNEGATIVE_ENABLE=1
compile_debug_n:
	$(CXX) -o mul_n $(FILELIST) -DDEBUG_ENABLE=1 -DNEGATIVE_ENABLE=1 -DTEST_DEBUG=1
compile_test_p:
	$(CXX) -o mul_p $(FILELIST) -DTEST_MONTE=1 -DPOSITIVE_ENABLE=1
compile_debug_p:
	$(CXX) -o mul_p $(FILELIST) -DDEBUG_ENABLE=1 -DPOSITIVE_ENABLE=1 -DTEST_DEBUG=1
compile_test_e:
	$(CXX) -o mul_e $(FILELIST) -DTEST_MONTE=1 -DNEAREST_ENABLE=1
compile_debug_e:
	$(CXX) -o mul_e $(FILELIST) -DDEBUG_ENABLE=1 -DNEAREST_ENABLE=1 -DTEST_DEBUG=1
compile_test_0:
	$(CXX) -o mul_0 $(FILELIST) -DTEST_MONTE=1 -DZERO_ENABLE=1
compile_debug_0:
	$(CXX) -o mul_0 $(FILELIST) -DDEBUG_ENABLE=1 -DZERO_ENABLE=1 -DTEST_DEBUG=1
run_n:
	./mul_n 
run_p:
	./mul_p 
run_e:
	./mul_e
run_0:
	./mul_0
clean:
	rm -f mul_0 mul_p mul_e mul_0

all: clean compile_test_n compile_test_p compile_test_e compile_test_0 run_n run_p run_e run_0