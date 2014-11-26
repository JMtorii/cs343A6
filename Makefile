OPT:=-O2 -multi

CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD ${OPT} 
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = main.o config.o printer.o bank.o bottle.o nameserver.o parent.o truck.o vending.o watcard.o watoffice.o soda.o
EXEC1 = vote

OBJECTS = ${OBJECTS1} 				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1}

.PHONY : all clean

all : ${EXECS}					# build all executables

#############################################################

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ImplType ImplKind
