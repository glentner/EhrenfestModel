compile = g++
debug   = -g
ccflags  = -fopenmp -std=c++11
exe     = bin/EhrenfestModel
bin     = bin
inc     = inc
lib     = lib
obj     = obj

link: main archives
	${compile} ${debug} -o ${exe} main.o  -I${inc} \
		-L. -lFramework ${ccflags}
	mv *.a *.o ${obj}/

archives: libFramework

Framework:   Model.o Parser.o Random.o Monitor.o
Framework_=  Model.o Parser.o Random.o Monitor.o

# library construction template
lib%: %
	ar -rv $@.a ${$*_}
	ranlib $@.a

# main has no header file
main: ${lib}/main.cc
	${compile} ${debug} -c ${lib}/main.cc -I${inc} ${ccflags}

# all objects compiled with this template
%.o: ${lib}/%.cc ${inc}/%.hh
	${compile} ${debug} -c ${lib}/$*.cc -I${inc} ${ccflags}

.PHONY: clean

# clear all objects and archives
clean:
	@rm -rf ${obj}{*.o,*.a} ${exe}

