if exists("b:current_syntax")
  finish
endif

syn match gramOperator '->\||'
syn region gramComment oneline start='#' end='$' contains=gramOperator,gramExtendedNonterminal,gramTerminal,gramTodo
syn region gramNonterminal oneline start='<' end='>'
syn region gramTerminal oneline start='"' end='"'

syn region gramExtendedNonterminal oneline contained start='<' end='>' contains=gramNonterminalParamName,gramNonterminalVariable
syn match gramNonterminalParamName '\w\+\ze:' nextgroup=gramNonterminalParamSep
syn match gramNonterminalParamSep ':'
syn match gramNonterminalVariable contained '@\w\+'

syn match gramTodo 'XXX\|TODO\|FIXME' contained

hi link gramComment Comment
hi link gramNonterminal String
hi link gramTerminal String

hi gramOperator    ctermfg=yellow
hi gramComment     ctermfg=blue
hi gramNonterminal ctermfg=blue
hi gramTerminal    ctermfg=magenta
hi gramTodo        ctermbg=yellow ctermfg=black
hi gramError       ctermbg=red    ctermfg=white

hi gramExtendedNonterminal      ctermfg=green
hi gramNonterminalParamName     ctermfg=cyan
hi link gramNonterminalParamSep gramOperator
hi gramNonterminalVariable      ctermfg=cyan

let b:current_syntax = "gram"

