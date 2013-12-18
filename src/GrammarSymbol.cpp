#include "GrammarSymbol.h"

GrammarSymbol::GrammarSymbol() : name(), thisIsTerminal() {
}

GrammarSymbol::GrammarSymbol(const string &initValue, bool initType, bool initMark) :
	name(initValue), thisIsTerminal(initType), mark(initMark) {
}

const string &GrammarSymbol::getName() const {
	return name;
}

bool GrammarSymbol::isVariable() const {
	return !thisIsTerminal;
}

bool GrammarSymbol::isTerminal() const {
	return thisIsTerminal;
}

void GrammarSymbol::setMark(bool m) {
	mark = m;
}

bool GrammarSymbol::isMarked() const {
	return mark;
}

bool GrammarSymbol::operator<(const GrammarSymbol &other) const {
	if(name < other.name) return true;
	else if(name == other.name) {
		if(thisIsTerminal && !other.thisIsTerminal) return true;
		else return !mark && other.mark;
	}
	return false;
}

bool GrammarSymbol::operator==(const GrammarSymbol &other) const {
	return thisIsTerminal == other.thisIsTerminal && name == other.name && mark == other.mark;
}

bool GrammarSymbol::operator!=(const GrammarSymbol &other) const {
	return !(*this == other);
}

ostream &operator<<(ostream &output, const GrammarSymbol &val) {
	return output << (val.isTerminal() ? val.getName() : ('<' + val.getName() + '>'))
		<< (val.isMarked() ? "*" : "");
}
