#include <stdio.h>


char table[40][20] = {
"NOOP",//0
"MNOTREC",//1
"program",//2
"var",//3
"Identifier",//4
"array",//5
"of",//6
"integer",//7
"real",//8
"number",//9
"function",//10
"procedure",//11
"begin",//12
"end",//13
"assignment operator",//14
"relation operator",//15
"addition operator",//16
"multiplication op",//17
"WSPACE",//18
"if",//19
"then",//20
"else",//21
"while",//22
"do",//23
"not",//24
"..",//25
".",//26
"( or )",//27
"[ or ]",//28
"LEXERROR",//29
",",//30
";",//31
":",//32
"integer",//33
"real",//34
"boolean",//35
"integer array",//36
"real array",//37
"type error",//38
"type void"//39
};

char * tokcode2str(int s){
    int lookup = s-48;
    return table[lookup];
}
