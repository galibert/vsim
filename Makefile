ALLINC = 
ALLLIB =
DEPLIB = 

PROG = vsim
OBJS = blocks.o sched.o vsim.o
SRCS = blocks.cc sched.cc vsim.cc
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
blocks.o: blocks.h state.h
sched.o: sched.h state.h blocks.h
vsim.o: blocks.h state.h sched.h
