CXX=g++
CXXFLAGS=-Wall
RM=rm -f

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=xlator

LINK=$(CXX) $(CXXFLAGS) -o $@ $^
COMPILE=$(CXX) $(CXXFLAGS) -o $@ -c $<

xlator: $(OBJ)
	$(LINK)

%.o: %.cpp
	$(COMPILE)

clean:
	$(RM) $(BIN) $(OBJ)

depend:
	sed -i '/^# DEPENDENCIES/q' Makefile
	$(CXX) -MM $(SRC) >> Makefile

# DEPENDENCIES
GrammarSymbol.o: GrammarSymbol.cpp GrammarSymbol.h
Interpreter.o: Interpreter.cpp Interpreter.h ParseTree.h GrammarSymbol.h \
 LightParseTree.h TranslationTree.h
LightParseTree.o: LightParseTree.cpp LightParseTree.h GrammarSymbol.h
main.o: main.cpp Translator.h Parser.h GrammarSymbol.h LightParseTree.h \
 ParseTree.h TranslationTree.h Graph.h Interpreter.h
Parser.o: Parser.cpp Parser.h GrammarSymbol.h LightParseTree.h \
 ParseTree.h TranslationTree.h Graph.h
ParseTree.o: ParseTree.cpp LightParseTree.h GrammarSymbol.h ParseTree.h \
 TranslationTree.h
Translation.o: Translation.cpp Translation.h ParseTree.h GrammarSymbol.h \
 LightParseTree.h TranslationTree.h
TranslationTree.o: TranslationTree.cpp TranslationTree.h ParseTree.h \
 GrammarSymbol.h LightParseTree.h
Translator.o: Translator.cpp Translator.h Parser.h GrammarSymbol.h \
 LightParseTree.h ParseTree.h TranslationTree.h Graph.h Interpreter.h
