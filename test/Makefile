CXX=g++
RM=rm -f

SRC=$(shell find -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
INC=-I../src
BIN=$(OBJ:.o=.test)

LINK=$(CXX) -Wall $(INC) -o $@ $^
COMPILE=$(CXX) -Wall -o $@ $(INC) -c $<

all: $(BIN)

%.test: %.o
	$(LINK)

%.o: %.cpp
	$(COMPILE)

clean:
	$(RM) $(BIN) $(OBJ)

depend:
	sed -i '/^# DEPENDENCIES/q' Makefile
	($(foreach src,$(SRC),$(CXX) $(INC) -MT $(src:.cpp=.o) -MM $(src);)) >> Makefile

# DEPENDENCIES
algorithm/alternations.o: algorithm/alternations.cpp \
 ../src/algorithm/alternations.h
