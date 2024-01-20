
PHONY:com sim cov clean debug

TOP_MODULE = tb
FILE_LIST = ./verif.f
OUTPUT=simv
ALL_DEFINE=+define +DUMP 
testcase=test_vtb_dphy_ipi16_02

#code coverage command
CM=-cm line +cond +fsm +branch +tgl
CM_NAME=-cm_name $(OUTPUT)
CM_DIR=-cm_dir ./$(OUTPUT).vdb

#vpd file name
VPD_NAME=+vpdfile +$(OUTPUT).vpd

#compile command
VCS=vcs +v2k -override_timescale=1ns/100ps\
	-full64	\
	-fsdb	\
	-debug_all	\
	-sverilog	\
	+plusarg_save	\
	+TESTCASE=${testcase}	\
	+nospecify	\
	+vcs +flush +all\
	$(CM)		\
	$(CM_NAME)     \
	$(CM_DIR)       \
	$(ALL_DEFINE)   \
	-o $(OUTPUT)    \
	-l compile.log  
#	$(VPD_NAME)     \

#simulation command
SIM=./$(OUTPUT)	\
	+ntb_random_seed_automatic	\
	$(CM) $(CM_NAME) $(CM_DIR)	\
	$(VPD_NAME)	\
	$(ALL_DEFINE)	\
	-l $(OUTPUT).log

#start compile
com: 
	$(VCS) -f $(FILE_LIST)

#start simulation
sim:
	$(SIM)

#show the coverage
cov:
	dve -covdir *vdb &

debug:
	dve -vpd $(OUTPUT).vpd &

#start clean
clean:
	rm -rf ./csrs *_daidir ./crsc	\
	*.log *.vpd *.vdb simv* *.key	\
	*race.out* novas* verdi* *fsdb apb2apb_async

verdi:	
	verdi -top ${TOP_MODULE} -nologo -sv -f ${FILE_LIST} -ssf *.fsdb +DUMP_FSDB

all: clean com sim verdi
