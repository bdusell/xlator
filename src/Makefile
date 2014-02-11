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
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/ParseForestNode.h \
 meta/exception.h xlator/Interpreter.h xlator/Tokenizer.h \
 xlator/Formatter.h xlator/Translator.h
ParseForestNode.o: xlator/ParseForestNode.cpp xlator/ParseForestNode.h \
 xlator/SymbolInfo.h functional/pointer_cmp.h
ParseTreeEnumerator.o: xlator/ParseTreeEnumerator.cpp \
 xlator/ParseTreeEnumerator.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/ParseForestNode.h
TranslationFileReader.o: xlator/TranslationFileReader.cpp \
 xlator/TranslationFileReader.h xlator/SymbolIndexer.h \
 xlator/SymbolInfo.h xlator/ParseTree.h resource/SharedPointer.h \
 xlator/TranslationTree.h
SymbolIndexer.o: xlator/SymbolIndexer.cpp xlator/SymbolIndexer.h \
 xlator/SymbolInfo.h
ParseTree.o: xlator/ParseTree.cpp xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/ParseForestNode.h
ParserFileReader.o: xlator/ParserFileReader.cpp xlator/ParserFileReader.h \
 xlator/FileReader.h xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/ParseForestNode.h meta/exception.h
SymbolInfo.o: xlator/SymbolInfo.cpp xlator/SymbolInfo.h
FileReader.o: xlator/FileReader.cpp xlator/FileReader.h
Tokenizer.o: xlator/Tokenizer.cpp xlator/Tokenizer.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/ParseForestNode.h meta/exception.h
Parser.o: xlator/Parser.cpp xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/ParseForestNode.h meta/exception.h xlator/ParserFileReader.h \
 xlator/FileReader.h
Formatter.o: xlator/Formatter.cpp xlator/Formatter.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/ParseForestNode.h \
 meta/exception.h xlator/Interpreter.h
Interpreter.o: xlator/Interpreter.cpp xlator/Interpreter.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h meta/exception.h xlator/TranslationFileReader.h \
 xlator/TranslationTree.h
Translator.o: xlator/Translator.cpp xlator/Translator.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/ParseForestNode.h meta/exception.h \
 xlator/Interpreter.h
main-debug.o: main.cpp resource/ScopedFile.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/ParseForestNode.h \
 meta/exception.h xlator/Interpreter.h xlator/Tokenizer.h \
 xlator/Formatter.h xlator/Translator.h
ParseForestNode-debug.o: xlator/ParseForestNode.cpp xlator/ParseForestNode.h \
 xlator/SymbolInfo.h functional/pointer_cmp.h
ParseTreeEnumerator-debug.o: xlator/ParseTreeEnumerator.cpp \
 xlator/ParseTreeEnumerator.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/ParseForestNode.h
TranslationFileReader-debug.o: xlator/TranslationFileReader.cpp \
 xlator/TranslationFileReader.h xlator/SymbolIndexer.h \
 xlator/SymbolInfo.h xlator/ParseTree.h resource/SharedPointer.h \
 xlator/TranslationTree.h
SymbolIndexer-debug.o: xlator/SymbolIndexer.cpp xlator/SymbolIndexer.h \
 xlator/SymbolInfo.h
ParseTree-debug.o: xlator/ParseTree.cpp xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/ParseForestNode.h
ParserFileReader-debug.o: xlator/ParserFileReader.cpp xlator/ParserFileReader.h \
 xlator/FileReader.h xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/ParseForestNode.h meta/exception.h
SymbolInfo-debug.o: xlator/SymbolInfo.cpp xlator/SymbolInfo.h
FileReader-debug.o: xlator/FileReader.cpp xlator/FileReader.h
Tokenizer-debug.o: xlator/Tokenizer.cpp xlator/Tokenizer.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/ParseForestNode.h meta/exception.h
Parser-debug.o: xlator/Parser.cpp xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/ParseForestNode.h meta/exception.h xlator/ParserFileReader.h \
 xlator/FileReader.h
Formatter-debug.o: xlator/Formatter.cpp xlator/Formatter.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/ParseForestNode.h \
 meta/exception.h xlator/Interpreter.h
Interpreter-debug.o: xlator/Interpreter.cpp xlator/Interpreter.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h meta/exception.h xlator/TranslationFileReader.h \
 xlator/TranslationTree.h
Translator-debug.o: xlator/Translator.cpp xlator/Translator.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/ParseForestNode.h meta/exception.h \
 xlator/Interpreter.h
