CXX=g++
CXXFLAGS=-Wall
RM=rm -f

INC=-I../src

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=graph_test parser_test

LINK=$(CXX) $(CXXFLAGS) -o $@ $^
COMPILE=$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

all: graph_test parser_test

graph_test: graph_test.o
	$(LINK)

parser_test: parser_test.o ../src/Parser.o ../src/GrammarSymbol.o \
 ../src/LightParseTree.o ../src/ParseTree.o ../src/TranslationTree.o

%.o: %.cpp
	$(COMPILE)

clean:
	$(RM) $(BIN) $(OBJ)

depend:
	sed -i '/^# DEPENDENCIES/q' Makefile
	$(CXX) $(INC) -MM $(SRC) >> Makefile

# DEPENDENCIES
graph_test.o: graph_test.cpp ../src/Graph.h
parser_test.o: parser_test.cpp ../src/Parser.h ../src/GrammarSymbol.h \
 ../src/LightParseTree.h ../src/ParseTree.h ../src/TranslationTree.h \
 ../src/Graph.h
