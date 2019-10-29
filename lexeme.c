#ifndef LEXEME_H
#define LEXEME_H

#include <string.h>

#define NOOP          48
#define MNOTREC       49
#define PROGRAM       50
#define VAR		      51
#define ID            52
#define ARRAY         53
#define OF            54
#define INTEGER       55
#define REAL          56
#define LONGREAL      57 //not useful
#define FUNCTION      58
#define PROCEDURE     59 //not useful
#define BEGIN         60
#define END           61
#define ASSIGNOP      62
#define RELOP         63
#define RELOP_EQ      0
#define RELOP_LT      1
#define RELOP_GT      2
#define RELOP_LE      3
#define RELOP_GE      4
#define RELOP_NE      5
#define ADDOP         64
#define ADDOP_PL      0
#define ADDOP_MN      1
#define ADDOP_OR      2
#define MULOP         65
#define MULOP_ML      0
#define MULOP_DVF     1
#define MULOP_DVI     2
#define MULOP_MOD     3
#define MULOP_AND     4
#define WSPACE        66
#define WSPACE_BL     0
#define WSPACE_TB     1
#define WSPACE_NL     2
#define IF            67
#define THEN          68
#define ELSE          69
#define WHILE         70
#define DO            71
#define NOT           72
#define DOTDOT        73
#define DOT           74
#define PAREN         75
#define PAREN_OPEN    0
#define PAREN_CLOSE   1
#define BRACK         76
#define BRACK_OPEN    0
#define BRACK_CLOSE   1
#define LEXERROR      77
#define IDTOOLONG     0
#define INTTOOLONG    1
#define REALFTOOLONG  2
#define REALBTOOLONG  3
#define REALETOOLONG  4
#define LEADINGZERO   5
#define TRAILINGZERO  6
#define UNKNOWNSYMBOL 7
#define PARENMISMATCH 8
#define BRACKMISMATCH 9
#define COMMA         78
#define SEMICOLON     79
#define COLON         80

struct Lexeme
{
int tkn;
char word[15];
union {
    int val;
    void *ptr;//will be used for ID symbol table pointing
    } attr;
};

typedef struct Lexeme *Lex;

struct Lexeme getLexeme(int lexeme){
    struct Lexeme tmp;
    tmp.tkn = 0;
    strcpy(tmp.word,"yeet");
    tmp.attr.val = 0;
    return tmp;
}

#endif
