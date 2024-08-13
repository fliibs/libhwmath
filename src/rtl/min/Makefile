# VCS = vcs -full64 -cpp g++-4.8 -cc gcc-4.8 -LDFLAGS -Wl,--no-as-needed -sverilog -error=20 +lint=TFIPC-L
VCS = vcs -LDFLAGS -Wl,--no-as-needed +v2k -override_timescale=1ns/100ps -full64 -debug_all -sverilog -l compile.log +define+RTL_DEBUG -fsdb -cc gcc-4.8 -cpp g++-4.8
SIMV = ./simv -l simv.log


env:
	export LIBW_PATH=$(pwd)
	echo ${LIBW_PATH}
comp: env
	${VCS} +define+incdir +define+SV -f ${LIBW_PATH}/min_rtl.f
clean:
	rm -rf csrc simv*