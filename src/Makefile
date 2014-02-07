CXX=g++
RM=rm -f

SRC=$(shell find -name '*.cpp')
OBJ_NORMAL=$(SRC:.cpp=.o)
OBJ_DEBUG=$(OBJ_NORMAL:.o=-debug.o)
OBJ=$(OBJ_NORMAL) $(OBJ_DEBUG)
INC=-I.
BIN=../bin/xlator ../bin/xlator-debug

LINK=$(CXX) -Wall $(INC) -o $@ $^
COMPILE=$(CXX) -Wall -O3 -s -o $@ $(INC) -DNDEBUG -c $<
COMPILE_DEBUG=$(CXX) -Wall -o $@ $(INC) -DDEBUG -c $<

all: $(BIN)

../bin/xlator: $(OBJ_NORMAL)
	$(LINK)

../bin/xlator-debug: $(OBJ_DEBUG)
	$(LINK)

%.o: %.cpp
	$(COMPILE)

%-debug.o: %.cpp
	$(COMPILE_DEBUG)

clean:
	$(RM) $(BIN) $(OBJ)

depend:
	sed -i '/^# DEPENDENCIES/q' Makefile
	$(CXX) $(INC) -MM $(SRC) >> Makefile
	$(CXX) $(INC) -MM $(SRC) | sed 's/\.o/-debug.o/g' >> Makefile

# DEPENDENCIES
main.o: main.cpp resource/ScopedFile.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolIndexer.h \
 meta/exception.h xlator/Interpreter.h xlator/Tokenizer.h \
 xlator/Formatter.h xlator/Translator.h
SymbolIndexer.o: xlator/SymbolIndexer.cpp xlator/SymbolIndexer.h
ParseTree.o: xlator/ParseTree.cpp xlator/ParseTree.h
Tokenizer.o: xlator/Tokenizer.cpp xlator/Tokenizer.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolIndexer.h meta/exception.h
Parser.o: xlator/Parser.cpp xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolIndexer.h meta/exception.h
Formatter.o: xlator/Formatter.cpp xlator/Formatter.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolIndexer.h \
 meta/exception.h xlator/Interpreter.h
Interpreter.o: xlator/Interpreter.cpp xlator/Interpreter.h \
 xlator/ParseTree.h meta/exception.h
Translator.o: xlator/Translator.cpp xlator/Translator.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolIndexer.h meta/exception.h \
 xlator/Interpreter.h
main-debug.o: main.cpp resource/ScopedFile.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolIndexer.h \
 meta/exception.h xlator/Interpreter.h xlator/Tokenizer.h \
 xlator/Formatter.h xlator/Translator.h
SymbolIndexer-debug.o: xlator/SymbolIndexer.cpp xlator/SymbolIndexer.h
ParseTree-debug.o: xlator/ParseTree.cpp xlator/ParseTree.h
Tokenizer-debug.o: xlator/Tokenizer.cpp xlator/Tokenizer.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolIndexer.h meta/exception.h
Parser-debug.o: xlator/Parser.cpp xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolIndexer.h meta/exception.h
Formatter-debug.o: xlator/Formatter.cpp xlator/Formatter.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolIndexer.h \
 meta/exception.h xlator/Interpreter.h
Interpreter-debug.o: xlator/Interpreter.cpp xlator/Interpreter.h \
 xlator/ParseTree.h meta/exception.h
Translator-debug.o: xlator/Translator.cpp xlator/Translator.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolIndexer.h meta/exception.h \
 xlator/Interpreter.h
