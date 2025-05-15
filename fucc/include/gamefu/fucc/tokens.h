#ifndef TK
#    define TK(id)
#endif

#ifndef TK_PUNCT
#    define TK_PUNCT(id, spelling) TK(id)
#endif

#ifndef TK_KEYWORD
#    define TK_KEYWORD(id, spelling) TK(id)
#endif

TK(INVALID)
TK(END_OF_FILE)

TK(IDENTIFIER)

TK_PUNCT(LPAREN, "(")
TK_PUNCT(RPAREN, ")")
TK_PUNCT(LSQUARE, "[")
TK_PUNCT(RSQUARE, "]")
TK_PUNCT(LCURLY, "{")
TK_PUNCT(RCURLY, "}")
TK_PUNCT(SEMI, ";")
TK_PUNCT(COMMA, ",")
TK_PUNCT(DOT, ".")
TK_PUNCT(EQUAL, "=")

TK_KEYWORD(BREAK, "break")
TK_KEYWORD(BYTE, "byte")
TK_KEYWORD(ELSE, "else")
TK_KEYWORD(ENUM, "enum")
TK_KEYWORD(FOR, "for")
TK_KEYWORD(HALF, "half")
TK_KEYWORD(IF, "if")
TK_KEYWORD(RETURN, "return")
TK_KEYWORD(STRUCT, "struct")
TK_KEYWORD(WORD, "word")
TK_KEYWORD(WHILE, "while")

#undef TK_KEYWORD
#undef TK
