xlator
======

A programmable natural language translator based on context-free grammars.

The `xlator` program accepts three inputs:

* a context-free grammar defining the source language
* a set of rules for mutating parse trees in the source grammar, defining how
  to translate the source language into the target language
* an input string in the source language

Given these inputs, `xlator` outputs all valid translations of the input string
with respect to the grammar and translation scheme provided. If the input is
not a valid sentence in the input language, the program will report an error
and generate no translations. If the input can be parsed or translated
ambiguously, the program will generate multiple translations.

Quickstart
----------

<pre>% make
% ./bin/xlator <var>grammar-file</var> <var>translation-file</var>
<kbd>this is my input string</kbd>
<samp>output string 1
output string 2
<var>...</var></samp></pre>

Algorithms
----------

The context-free grammar parser uses dynamic programming to identifiy valid
parse trees for the input string. Since we're dealing with natural languages,
the parser is designed to operate on ambigous grammars and strings by simply
producing all possible parse trees. It does this by traversing the entire
search space, recursively testing all replacements, interning partial parse
trees, and pruning the search space for strings which have already been
encountered.

Note that this is far from the best known algorithm for parsing ambiguous
context-free grammars. A better alternative would be Tomita's GLR algorithm.

Limitations
-----------

Naturally, `xlator` cannot parse non-context-free languages. There are, in
fact, instances of natural grammatical structures which cannot be represented
by any context-free grammar; the Mohawk language, for example, contains strings
of the <var>xyz...xyz...</var> variety. Context-free grammars are not able to
model human language in the general case.

Usage
-----

Run `make` to build the `xlator` executable at `bin/xlator`.

The `xlator` executable accepts a grammar file and a translation file as
command line arguments. It reads a single line of space-separated words from
`stdin` and prints results on `stdout`.

The grammar file specifies what strings of tokens are accepted by the input
language. The translation file specifies the rules used for transforming
sections of parse trees in the input language to sections of parse trees in the
output language. This format requires a little bit of explanation and a
rudimentary understanding of how the translation algorithm works. Let's look at
a very simple example of a translation rule for translating a sentence from
English to Japanese. Here, we're saying that for every English sentence of the
form _\[subject\] \[verb\] \[object\]_, the equivalent sentence in Japanese is
in the form _\[subject\] wa \[object\] o \[verb\]_, where the subject, object,
and verb will be translated in turn. The translation rule for this looks like:

    <jp-sentence> { <en-noun> { subj } <en-verb> { verb } <en-noun> { obj } } -> <jp-sentence> { <jp-noun> { subj } "wa" <jp-noun> { obj } "o" <jp-verb> { verb } }

This rule tells the translator that whenever it is trying to construct a
Japanese sentence (`<jp-sentence>`) using the contents of an English sentence,
and the contents of that English sentence match the pattern between the braces
on the left side (`<en-noun> <en-verb> <en-noun>`), then it should rearrange
the sentence according to the pattern on the right side
(`<jp-noun> wa <jp-noun> o <jp-verb>`). Using the identifiers `subj`, `verb`,
and `obj`, the translator knows where to transplant the sub-parse trees from
the source sentence in the newly constructed sentence. The process then
recurses on these yet-to-be-translated parse trees, so that the English
subject, object, and verb will be translated appropriately into Japanese, now
that they are already in their correct positions in the sentence.

Keep in mind that in translation files, two disjoint symbol alphabets are in
play. For clarity, in the above example, any non-terminal symbol (i.e.
`<anything-written-like-this>`) belonging to the input alphabet was prefixed
with `en-`, and any non-terminal belonging to the output alphabet was prefixed
with `jp-`. The braces indicate parse tree hierarchy and can be nested to any
depth. The `->` symbol separates the left and right sides of the rule. Anything
in double quotes is a terminal symbol (which naturally cannot have sub-trees).
The bare `subj`, `verb`, and `obj` symbols which appear on both sides are
wildcards which stand for a string of sub-trees beneath a non-terminal symbol.

The left side of a translation rule is a parse tree _pattern_, which matches
the top of a partially translated input parse tree if all of the terminal and
nonterminal symbols in the input tree match those in the pattern, in the
same tree structure. Anything else in the input tree must be accounted for in
the wildcard variables. Note that non-terminal symbols can never be leaf nodes
&ndash; they must always contain either a string of sub-trees to match or a
placeholder variable for them. The placeholder variables on the right side
refer to those on the left and indicate how the unmatched sub-trees in the
input parse tree are donated to the newly constructed tree on the right side.
In the example, the non-terminals `<en-noun>` and `<en-verb>` were basically
re-named and switched, and it is expected that new translation rules would be
applied recursively to the newly created `<jp-noun>` and `<jp-verb>` symbols,
which have the old child sets of `<en-noun>` and `<en-verb>`.

The root node on the left side belongs to the output alphabet, and anything
beneath it belongs to the input alphabet. This reflects the fact the
translation algorithm works by starting at the root node of the input parse
tree and then working its way down, matching the top of the tree with
translation rules, applying the transformations described by those rules,
donating child nodes of the input tree to those of the output tree, and
continuing the process recursively on the non-terminal leaf nodes of the
transformed tree.

Refer to the examples under `examples/` for additional guidance. Note that
these translation files always start with a rule of the form

    <sentence> { x } -> <*sentence> { x }

where `<sentence>` is in the input alphabet and is also the start variable of
the input grammar, and `<*sentence>` is in the output alphabet. This rule
serves as the first matched rule and starts the recursive translation
procedure. For this reason, whenever a non-terminal variable on the left side
is the root node and also has a wildcard for its children, it is considered to
be in the input alphabet.

The xlator program accepts a single line of input on `stdin` and, barring any
errors in the input files or the syntax of the input line, exhaustively prints
out all possible translations, one per line, to `stdout`.

The input string is read as a sequence of case-sensitive tokens, which are
separated by white space. This tokenization method is currently not very
intelligent and does not even permit sentence capitalization (at least not
in a way that doesn't involve tweaking the input grammar in a ludricous way).

Both the grammar and translation files can include `#`-style comments;
everything after the first `#` character on a line not inside of a token is
ignored.

The program works transparently with Unicode encoded in UTF-8.

Examples
--------

English to Japanese:

```sh
% ./bin/xlator examples/simple-en-jp.{gram,trans}
the man sees the woman
otoko no hito wa onna no hito o mimasu
```

English to Latin:

```sh
% ./bin/xlator examples/simple-en-la.{gram,trans}
the teacher teaches the student
magister discipulum docet
magister discipulam docet
magister docet discipulum
magister docet discipulam
magistra discipulum docet
magistra discipulam docet
magistra docet discipulum
magistra docet discipulam
discipulum docet magister
discipulum docet magistra
discipulam docet magister
discipulam docet magistra
docet discipulum magister
docet discipulum magistra
docet discipulam magister
docet discipulam magistra
```
