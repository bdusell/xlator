CXX=g++
RM=rm -f

SRC=$(shell find * -name '*.cpp')
OBJ_NORMAL=$(SRC:.cpp=.o)
OBJ_DEBUG=$(OBJ_NORMAL:.o=-debug.o)
OBJ=$(OBJ_NORMAL) $(OBJ_DEBUG)
INC=-I.
BIN=../bin/xlator ../bin/xlator-debug

LINK=$(CXX) -Wall $(INC) -o $@ $^
COMPILE=$(CXX) -Wall -O3 -s -o $@ $(INC) -DNDEBUG -c $<
COMPILE_DEBUG=$(CXX) -Wall -g -o $@ $(INC) -DDEBUG -c $<

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
	($(foreach src,$(SRC),$(CXX) $(INC) -MT $(src:.cpp=.o) -MM $(src); $(CXX) $(INC) -MT $(src:.cpp=-debug.o) -MM $(src);)) >> Makefile

# DEPENDENCIES
main.o: main.cpp resource/ScopedFile.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/Indexer.h \
 xlator/ParseForestNode.h meta/exception.h xlator/Interpreter.h \
 xlator/ParseTreeMatcher.h xlator/TranslationTree.h xlator/Tokenizer.h \
 xlator/Formatter.h xlator/Translator.h
main-debug.o: main.cpp resource/ScopedFile.h xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/Indexer.h \
 xlator/ParseForestNode.h meta/exception.h xlator/Interpreter.h \
 xlator/ParseTreeMatcher.h xlator/TranslationTree.h xlator/Tokenizer.h \
 xlator/Formatter.h xlator/Translator.h
xlator/ParseForestNode.o: xlator/ParseForestNode.cpp \
 xlator/ParseForestNode.h xlator/SymbolInfo.h functional/pointer_cmp.h
xlator/ParseForestNode-debug.o: xlator/ParseForestNode.cpp \
 xlator/ParseForestNode.h xlator/SymbolInfo.h functional/pointer_cmp.h
xlator/ParseTreeEnumerator.o: xlator/ParseTreeEnumerator.cpp \
 xlator/ParseTreeEnumerator.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/ParseForestNode.h
xlator/ParseTreeEnumerator-debug.o: xlator/ParseTreeEnumerator.cpp \
 xlator/ParseTreeEnumerator.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/ParseForestNode.h
xlator/TranslationFileReader.o: xlator/TranslationFileReader.cpp \
 xlator/TranslationFileReader.h xlator/FileReader.h xlator/Indexer.h \
 xlator/SymbolIndexer.h xlator/SymbolInfo.h xlator/ParseTree.h \
 resource/SharedPointer.h xlator/TranslationTree.h xlator/Interpreter.h \
 xlator/ParseTreeMatcher.h meta/exception.h
xlator/TranslationFileReader-debug.o: xlator/TranslationFileReader.cpp \
 xlator/TranslationFileReader.h xlator/FileReader.h xlator/Indexer.h \
 xlator/SymbolIndexer.h xlator/SymbolInfo.h xlator/ParseTree.h \
 resource/SharedPointer.h xlator/TranslationTree.h xlator/Interpreter.h \
 xlator/ParseTreeMatcher.h meta/exception.h
xlator/SymbolIndexer.o: xlator/SymbolIndexer.cpp xlator/SymbolIndexer.h \
 xlator/Indexer.h xlator/SymbolInfo.h
xlator/SymbolIndexer-debug.o: xlator/SymbolIndexer.cpp \
 xlator/SymbolIndexer.h xlator/Indexer.h xlator/SymbolInfo.h
xlator/ParseTree.o: xlator/ParseTree.cpp xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/ParseForestNode.h
xlator/ParseTree-debug.o: xlator/ParseTree.cpp xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/ParseForestNode.h
xlator/ParserFileReader.o: xlator/ParserFileReader.cpp \
 xlator/ParserFileReader.h xlator/FileReader.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/Indexer.h xlator/ParseForestNode.h \
 meta/exception.h
xlator/ParserFileReader-debug.o: xlator/ParserFileReader.cpp \
 xlator/ParserFileReader.h xlator/FileReader.h xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/Indexer.h xlator/ParseForestNode.h \
 meta/exception.h
xlator/SymbolInfo.o: xlator/SymbolInfo.cpp xlator/SymbolInfo.h
xlator/SymbolInfo-debug.o: xlator/SymbolInfo.cpp xlator/SymbolInfo.h
xlator/ParseTreeMatcher.o: xlator/ParseTreeMatcher.cpp \
 xlator/ParseTreeMatcher.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/TranslationTree.h
xlator/ParseTreeMatcher-debug.o: xlator/ParseTreeMatcher.cpp \
 xlator/ParseTreeMatcher.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/TranslationTree.h
xlator/FileReader.o: xlator/FileReader.cpp xlator/FileReader.h
xlator/FileReader-debug.o: xlator/FileReader.cpp xlator/FileReader.h
xlator/Tokenizer.o: xlator/Tokenizer.cpp xlator/Tokenizer.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/Indexer.h \
 xlator/ParseForestNode.h meta/exception.h
xlator/Tokenizer-debug.o: xlator/Tokenizer.cpp xlator/Tokenizer.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/Indexer.h \
 xlator/ParseForestNode.h meta/exception.h
xlator/Parser.o: xlator/Parser.cpp xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/Indexer.h xlator/ParseForestNode.h meta/exception.h \
 xlator/ParserFileReader.h xlator/FileReader.h
xlator/Parser-debug.o: xlator/Parser.cpp xlator/Parser.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/Indexer.h xlator/ParseForestNode.h \
 meta/exception.h xlator/ParserFileReader.h xlator/FileReader.h
xlator/Formatter.o: xlator/Formatter.cpp xlator/Formatter.h \
 xlator/Translator.h xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/Indexer.h xlator/ParseForestNode.h meta/exception.h \
 xlator/Interpreter.h xlator/ParseTreeMatcher.h xlator/TranslationTree.h
xlator/Formatter-debug.o: xlator/Formatter.cpp xlator/Formatter.h \
 xlator/Translator.h xlator/Parser.h xlator/ParseTree.h \
 xlator/SymbolInfo.h resource/SharedPointer.h xlator/SymbolIndexer.h \
 xlator/Indexer.h xlator/ParseForestNode.h meta/exception.h \
 xlator/Interpreter.h xlator/ParseTreeMatcher.h xlator/TranslationTree.h
xlator/Interpreter.o: xlator/Interpreter.cpp xlator/Interpreter.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/Indexer.h xlator/ParseTreeMatcher.h \
 xlator/TranslationTree.h meta/exception.h xlator/TranslationFileReader.h \
 xlator/FileReader.h algorithm/alternations.h
xlator/Interpreter-debug.o: xlator/Interpreter.cpp xlator/Interpreter.h \
 xlator/ParseTree.h xlator/SymbolInfo.h resource/SharedPointer.h \
 xlator/SymbolIndexer.h xlator/Indexer.h xlator/ParseTreeMatcher.h \
 xlator/TranslationTree.h meta/exception.h xlator/TranslationFileReader.h \
 xlator/FileReader.h algorithm/alternations.h
xlator/Translator.o: xlator/Translator.cpp xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/Indexer.h \
 xlator/ParseForestNode.h meta/exception.h xlator/Interpreter.h \
 xlator/ParseTreeMatcher.h xlator/TranslationTree.h
xlator/Translator-debug.o: xlator/Translator.cpp xlator/Translator.h \
 xlator/Parser.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h xlator/SymbolIndexer.h xlator/Indexer.h \
 xlator/ParseForestNode.h meta/exception.h xlator/Interpreter.h \
 xlator/ParseTreeMatcher.h xlator/TranslationTree.h
xlator/TranslationTree.o: xlator/TranslationTree.cpp \
 xlator/TranslationTree.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h
xlator/TranslationTree-debug.o: xlator/TranslationTree.cpp \
 xlator/TranslationTree.h xlator/ParseTree.h xlator/SymbolInfo.h \
 resource/SharedPointer.h
