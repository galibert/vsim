ALLINC = 
ALLLIB =
DEPLIB = 

PROG = vsim
OBJS = rom.o blocks.o sched.o sram.o vsim.o
SRCS = rom.cc blocks.cc sched.cc sram.cc vsim.cc
JUNK =

OPT=-O9

CC=gcc
CXX=g++
CFLAGS=-g -Wall ${OPT}

all: ${PROG}

.SUFFIXES: .cc .hh .o .a .ui .m.cc .m.o .y
.PRECIOUS: ${JUNK}

${PROG}: ${OBJS} ${DEPLIB}
	${CXX} ${CFLAGS} ${OBJS} ${ALLLIB} ${ALLINC} -o ${PROG}

.cc.o:
	${CXX} -c ${CFLAGS} ${DEFS} ${ALLINC} $<

.c.o:
	${CC} -c ${CFLAGS} ${DEFS} ${ALLINC} $<

.y.c:
	bison -d -o $@ $<

.y.h:
	bison -d -o $@ $<

clean:
	rm -f ${PROG} ${OBJS} ${JUNK}

distclean:
	rm -f ${PROG} ${OBJS} ${JUNK}

###
rom.o: blocks.h state.h
blocks.o: blocks.h state.h
sched.o: sched.h state.h blocks.h
sram.o: blocks.h state.h
vsim.o: blocks.h state.h sched.h
