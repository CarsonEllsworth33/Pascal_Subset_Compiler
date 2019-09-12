#define MNOTREC       49
#define PROGRAM       50
#define VAR		      51
#define ID            52
#define ARRAY         53
#define OF            54
#define INTEGER       55
#define REAL          56
#define LONGREAL      57
#define FUNCTION      58
#define PROCEDURE     59
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
#define MULOP_MOD      3
#define MULOP_AND      4
#define WSPACE        66
#define WSPACE_BL     0
#define WSPACE_TB     1
#define WSPACE_NL     2
#define LEXERROR      67
#define IDTOOLONG     0
#define INTTOOLONG    1
#define REALTOOLONG   2
#define LEADINGZERO   3
#define UNKNOWNSYMBOL 4



struct Lexeme
{
int tkn;
union {
    int val;
    char *ptr;//will be used for ID symbol table pointing
    } attr;
};

struct Lexeme getLexeme(int lexeme){
    struct Lexeme tmp = {100, {1000} };
    return tmp;
}
