CXX 	 :=g++
FILELIST :=@filelist.txt
SV_FILE  :=./filelist.f
OUTPUT	  =simv
TOP_MODULE=mul_tb

compile_test:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=1 -Dtest_times=10000 -Dspecial_test_time=0

compile_debug:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=0 -DDEBUG_ENABLE=0 -DTEST_BUG=1

run:
	./mul

com:
	vcs +v2k -override_timescale=1ns/100ps -full64 -debug_all -sverilog -l compile.log -f filelist.f -fsdb 

sim:
	./simv +ntb_random_seed_automatic +vpdfile +simv.vpd -l simv.log

verdi:
	verdi -top mul_tb -nologo -sv -f filelist.f -ssf *.fsdb +DUMP_FSDB

clean:
	rm -rf ./csrs *_daidir ./crsc	\
	*.log *.vpd *.vdb *.key	\
	*race.out* novas* verdi* *fsdb mul_in.txt mul_out.txt c_out.txt

clean_t:
	rm -rf ./csrs *_daidir ./crsc	\
	*.log *.vpd *.vdb simv* *.key	\
	*race.out* novas* verdi* *fsdb mul_in.txt mul_out.txt c_out.txt

build:
	touch mul_in.txt mul_out.txt c_out.txt

pre: clean build