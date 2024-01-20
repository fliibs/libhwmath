CXX 	 :=g++
FILELIST :=@filelist.txt
SV_FILE  :=./filelist.f
OUTPUT	  =simv
TOP_MODULE=mul_tb

compile_test:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=1 -DTEST_TIMES=100000 -DSPECIAL_TEST_TIMES=100000 -DDEBUG_SV=1 -DTEST_REG=1 -DTEST_ONE=1 -DTEST_INF=1 -DTEST_ZERO_INF=1 -DTEST_1_NAN=1

compile_debug:
	$(CXX) -o mul $(FILELIST) -DTEST_MONTE=0 -DDEBUG_ENABLE=0 -DTEST_BUG=1 -DDEBUG_SV=1

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