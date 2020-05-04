#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "../LexicalAnalyzer/lexAnalyze.c"
#include "../lexeme.c"
#include "../node/nodetest.c"

struct Lexeme tok;
Lex tok_lex;
Lex tok_ret;
struct node_stack parsestack;
stack_ptr st = &parsestack;
FILE *token;
FILE *list;
FILE *file;
FILE *trace;

void decs();
int std_type();
void sub_decs();
int type();
void cmpd_stmt();
void stmt();
void expr_lst();
int expr();

Lex tok_match(int t,int val){
    //int t = lex.tkn;
    //int type;
    tok_lex = &tok;
    printf("tokcmp %d %d\n",tok.tkn, t);
        if(t == tok.tkn){
            if(t != EOF && val == 0 && tok.tkn != EOF){
                if(tok.tkn == ID ){
                    printf("lexeme: %s, type: %d\n",tok.word,tok.type);
                    tok = get_next_token(file,list,token);
                    return tok_lex; //all good here
                }
                else if(tok.tkn == NUM){
                    printf("lexeme: %s, type: %d\n",tok.word,tok.type);
                    tok = get_next_token(file,list,token);
                    return tok_lex;
                }
                else{
                    tok = get_next_token(file,list,token);
                    return tok_lex; //all good here
                }
            }
            if(t == EOF){
                exit(0);
            }
            if(val != 0 && tok.attr.val == val){
                tok = get_next_token(file,list,token);
                return tok_lex;
            }
            if(val != 0 && tok.attr.val != val){
                printf("Sync Error!! Expecting token %d attr %d instead recieved token %s \n",t,val,tok.word);
                fprintf(list,"SYNERR: tok mismatch, expecting token code %d instead recieved token %s\n",t,tok.word);
                tok = get_next_token(file,list,token);
                return NULL;
            }
            else{
                exit(0);
                //end of parse
                //stop parse here some how
            }
        }//end t == tok

        else{
            printf("Sync Error!! Expecting token %d instead recieved token %s \n",t,tok.word);
            fprintf(list,"SYNERR: tok mismatch, expecting token code %d instead recieved token %s\n",t,tok.word);
            tok = get_next_token(file,list,token);
            return NULL;
        }
}



void sign(){
    fprintf(trace, "%s %s\n","sign", tok.word);
    switch (tok.tkn) {
        case ADDOP:
            if(tok.attr.val == ADDOP_PL){
                tok_match(ADDOP,ADDOP_PL);
                break;
            }
            if(tok.attr.val == ADDOP_MN){
                tok_match(ADDOP,ADDOP_MN);
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n","+","-",tok.word);
            while (tok.tkn!=ID && tok.tkn!=NUM && tok.tkn!=NOT && strcmp(tok.word,"(") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "sign: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sign: sync on token %s\n",tok.word );
    }
}

int factorT(){
    fprintf(trace, "%s %s\n","factorT",tok.word );
    switch (tok.tkn) {
        //start epsilons
        case MULOP:
            return 0;
        case ADDOP:
            return 0;
        case RELOP:
            return 0;
        case SEMICOLON:
            return 0;
        case END:
            return 0;
        case ELSE:
            return 0;
        case THEN:
            return 0;
        case DO:
            return 0;
        case COMMA:
            return 0;
        //end epsilons
        case PAREN: //function call !!!
            if(tok.tkn == PAREN && tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); expr_lst(); tok_match(PAREN,PAREN_CLOSE);
                return 0;
            }
        case BRACK: //array assign or look up
            if(tok.tkn == BRACK && tok.attr.val == BRACK_OPEN){
                tok_match(BRACK,BRACK_OPEN); expr(); tok_match(BRACK,BRACK_CLOSE);
                return 0;
            }
            if(tok.attr.val == PAREN_CLOSE || tok.attr.val == BRACK_CLOSE){return 0;}
        default:
            fprintf(list,"SYNERR: SYNERR: tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead recieved %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
            while (tok.tkn != MULOP && tok.tkn != ADDOP && tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "factorT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "factorT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}
//NEED TO FIX RVALUE tok_match stuff
int factor(){
    fprintf(trace, "%s %s\n","factor",tok.word );
    int r_val = 0;
    Lex tok_ret;
    switch (tok.tkn) {
        case ID:
            tok_ret = tok_match(ID,0); factorT();
            break;
        case NUM:
            tok_ret = tok_match(NUM,0);
            r_val = get_id_type(st,tok_ret->word);//source of segfault
            r_val = 0;
            return r_val;
        case NOT:
            tok_match(NOT,0); int r_val = factorT();
            if(r_val == TYPEBOOL){
                return TYPEBOOL;
            }
            else{
                printf("type mismatch, expecting typecode: %d recieved: %d\n",TYPEBOOL,r_val);
                return TYPEERR;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); int r_val = expr(); tok_match(PAREN,PAREN_CLOSE);
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != MULOP && tok.tkn != ADDOP && tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "factor: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "factor: sync on token %s\n", tok.word);
    }
    return TYPEERR;
}

void termT(){
    fprintf(trace, "%s %s\n","termT",tok.word );
    switch (tok.tkn) {
        case MULOP:
            tok_match(MULOP,0); factor(); termT();
            break;
        case ADDOP: case RELOP: case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            break;
        case BRACK: case PAREN:
            if(tok.attr.val == BRACK_CLOSE || tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead recieved %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
            while (tok.tkn != ADDOP && tok.tkn != RELOP && strcmp(tok.word,";") != 0 && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "termT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "termT: sync on token %s\n", tok.word);
    }
}

void term(){
    fprintf(trace, "%s %s\n","term" ,tok.word);
    switch (tok.tkn) {
        case ID: case NUM: case NOT:
            factor(); termT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                factor(); termT();
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != ADDOP && tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN &&  tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "term: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "term: sync on token %s\n", tok.word);
    }
}

void smpl_exprT(){
    fprintf(trace, "%s %s\n","smpl_exprT",tok.word );
    switch (tok.tkn) {
        case ADDOP:
            tok_match(ADDOP,0); term(); smpl_exprT();
            break;
        case RELOP: case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            break;
        case BRACK: case PAREN:
            if(tok.attr.val == BRACK_CLOSE || tok.attr.val == PAREN_CLOSE) break;
        default:
            fprintf(list,"SYNERR: SYNERR: tok mismatch expecting %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, instead recieved %s\n","+ - or","< > <= >= <> =",";","end","else","then","do",";","]",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "smpl_exprT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "smpl_exprT: sync on token %s\n", tok.word);
    }
}

void smpl_expr(){
    fprintf(trace, "%s %s\n","smpl_expr",tok.word );
    switch (tok.tkn) {
        case ID: case NUM: case NOT:
            term(); smpl_exprT();
            break;
        case ADDOP:
            if(tok.attr.val == ADDOP_PL || tok.attr.val == ADDOP_MN){
                sign(); term(); smpl_exprT();
                break;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                term(); smpl_exprT();
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","+","-",tok.word);
            while (tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,"]") != 0 && strcmp(tok.word,")") != 0 ) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "smpl_expr: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "smpl_expr: sync on token %s\n", tok.word);
    }
}

void exprT(){
    fprintf(trace, "%s %s\n","exprT" ,tok.word);
    switch (tok.tkn) {
        case RELOP:
            tok_match(RELOP,0); smpl_expr();
            break;
        case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            break;
        case BRACK:
            if(tok.attr.val == BRACK_CLOSE){break;}
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){break;}
        default:
            fprintf(list,"SYNERR: SYNERR: tok mismatch expecting %s, instead recieved %s\n","< > <= >= <> = ; , ) ] end else then do",tok.word);
            while (tok.tkn != COMMA && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && strcmp(tok.word, ")") != 0 && strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "exprT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "exprT: sync on token %s\n", tok.word);
    }
}

int expr(){
    fprintf(trace, "%s %s\n","expr",tok.word );
    //printf("%d,%d,%s\n",tok.tkn,tok.attr.val,tok.word);
    switch (tok.tkn) {
        case NUM: case ID: case NOT:
            smpl_expr(); exprT();
            return 0;
        case ADDOP:
            if(tok.attr.val == ADDOP_PL){
                smpl_expr(); exprT();
                return 0;
            }
            if(tok.attr.val == ADDOP_MN){
                smpl_expr(); exprT();
                return 0;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                smpl_expr(); exprT();
                return 0;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, %s, %s, %s, instead recieved %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (tok.tkn != COMMA && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && strcmp(tok.word, ")") != 0 && strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "expr: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "expr: sync on token %s\n", tok.word);
            return 0;
    }
}

void expr_lstT(){
    fprintf(trace, "%s %s\n","expr_lstT",tok.word );
    switch (tok.tkn) {
        case COMMA:
            tok_match(COMMA,0); expr(); expr_lstT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            fprintf(list,"SYNERR: SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n",",",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "expr_lstT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "expr_lstT: sync on token %s\n", tok.word);
    }
}

void expr_lst(){
    fprintf(trace, "%s %s\n","expr_lst" ,tok.word);
    switch (tok.tkn) {
        case NUM: case ID: case NOT:
            expr(); expr_lstT();
            break;
        case ADDOP:
            if(tok.attr.val == ADDOP_PL){
                expr(); expr_lstT();
                break;
            }
            if(tok.attr.val == ADDOP_MN){
                expr(); expr_lstT();
                break;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                expr(); expr_lstT();
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, %s, %s, %s, instead recieved %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "expr_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "expr_lst: sync on token %s\n", tok.word);
    }
}

void varT(){
    fprintf(trace, "%s %s\n","varT",tok.word );
    switch (tok.tkn) {
        case ASSIGNOP:
            break;
        case BRACK:
            if(tok.attr.val == BRACK_OPEN){
                tok_match(BRACK,BRACK_OPEN); expr(); tok_match(BRACK,BRACK_CLOSE);
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n",":=","[",tok.word);
            while (tok.tkn != ASSIGNOP) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "varT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "varT: sync on token %s\n", tok.word);
    }
}

void var(){
    fprintf(trace, "%s %s\n","var",tok.word );
    switch (tok.tkn) {
        case ID:
            tok_match(ID,0); varT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting Identifier, instead recieved %s\n",tok.word);
            while (strcmp(tok.word,":=") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "var: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "var: sync on token %s\n", tok.word);
    }
}

void stmtT(){
    fprintf(trace, "%s %s\n","stmtT",tok.word );
    switch (tok.tkn) {
        case ELSE:
            tok_match(ELSE,0); stmt();
            break;
        case SEMICOLON: case END:
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n","else",";",tok.word);
            while (strcmp(tok.word,"else") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmtT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "stmtT: sync on token %s\n", tok.word);
    }
}

void stmt(){
    fprintf(trace, "%s %s\n","stmt",tok.word );
    //printf("%d,%d,%s\n",tok.tkn,tok.attr.val,tok.word);
    switch (tok.tkn) {
        case ID:
            var(); tok_match(ASSIGNOP,0); expr();
            break;
        case BEGIN:
            cmpd_stmt();
            break;
        case IF:
            tok_match(IF,0); expr(); tok_match(THEN,0); stmt(); stmtT();
            break;
        case WHILE:
            tok_match(WHILE,0); expr(); tok_match(DO,0); stmt();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s instead recieved %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"else") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmt: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "stmt: sync on token %s\n", tok.word);
    }
}

void stmt_lstT(){
    fprintf(trace, "%s %s\n","stmt_lstT" ,tok.word);
    switch (tok.tkn) {
        case SEMICOLON:
            tok_match(SEMICOLON,0); stmt(); stmt_lstT();
            break;
        case END:
            fprintf(trace, "%s\n","stmt_lstT end break no tok consumption" );
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s, instead recieved %s\n",";","end",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmt_lstT: trying to sync on %s\n", tok.word);
                printf("Stuck here with tok: %d\n",tok.tkn);
            }
            fprintf(trace, "stmt_lstT: sync on token %s\n", tok.word);
    }
}

void stmt_lst(){
    fprintf(trace, "%s %s\n","stmt_lst",tok.word );
    switch (tok.tkn) {
        case ID: case BEGIN: case IF: case WHILE:
            stmt(); stmt_lstT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmt_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "stmt_lst: sync on token %s\n", tok.word);
    }
}

void opt_stmt(){
    fprintf(trace, "%s %s\n","opt_stmt",tok.word );
    switch (tok.tkn) {
        case ID: case BEGIN: case IF: case WHILE:
            stmt_lst();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "opt_stmt: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "opt_stmt: sync on token %s\n", tok.word);
    }
}

void cmpd_stmtT(){
    fprintf(trace, "%s %s\n","cmpd_stmtT" ,tok.word);
    switch (tok.tkn) {
        case END:
            tok_match(END,0);
            break;
        case BEGIN: case IF: case WHILE: case ID:
            opt_stmt(); tok_match(END,0);
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s,%s instead recieved %s\n","Identifier","end","begin","if","while","do",tok.word);
            while (strcmp(tok.word,".") != 0 && strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "cmpd_stmtT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "cmpd_stmtT: sync on token %s\n", tok.word);
    }
}

void cmpd_stmt(){
    fprintf(trace, "%s %s\n","cmpd_stmt",tok.word );
    switch (tok.tkn) {
        case BEGIN:
            tok_match(BEGIN,0); cmpd_stmtT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, instead recieved %s\n","begin",tok.word);
            while (strcmp(tok.word,".") != 0 && strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "cmpd_stmt: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "cmpd_stmt: sync on token %s\n", tok.word);
    }
}
//THIS IS A BLUE NODE FABRICATOR
void param_lstT(){
    fprintf(trace, "%s %s\n","param_lstT",tok.word);
    switch (tok.tkn) {
        case SEMICOLON:
            tok_match(SEMICOLON,0); tok_match(ID,0); tok_match(COLON,0); type(); param_lstT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n",";",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "param_lstT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "param_lstT: sync on token %s\n", tok.word);
    }
}
//THIS IS A BLUE NODE FABRICATOR
void param_lst(){
    fprintf(trace, "%s %s\n","param_lst",tok.word );
    switch (tok.tkn) {
        case ID:
            tok_match(ID,0); tok_match(COLON,0); type(); param_lstT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting Identifier, instead recieved %s\n",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "param_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "param_lst: sync on token %s\n", tok.word);
    }
}

void args(){
    fprintf(trace, "%s %s\n","args",tok.word);
    switch (tok.tkn) {
        case PAREN:
            if (tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); param_lst(); tok_match(PAREN,PAREN_CLOSE); break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, instead recieved %s\n","(",tok.word);
            while (strcmp(tok.word,":") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "args: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "args: sync on token %s\n", tok.word);
    }
}
//GREEN NODE FABRICATOR
int sub_headT(){
    fprintf(trace, "%s %s\n","sub_headT",tok.word);
    int s;
    switch (tok.tkn) {
        case COLON:
            tok_match(COLON,0); s = std_type(); tok_match(SEMICOLON,0);
            return s;
        case PAREN:
            if (tok.attr.val == PAREN_OPEN){
                args(); tok_match(COLON,0); s = std_type(); tok_match(SEMICOLON,0);
                return s;
            }
            return TYPEERR;//this is the case for a non-openParen
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n",":","(",tok.word);
            while(strcmp(tok.word,"var") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_headT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_headT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}
//GREEN NODE FABRICATOR
void sub_head(){
    fprintf(trace, "%s %s\n","sub_head" ,tok.word);
    int s; //function return type
    switch (tok.tkn) {
        case FUNCTION:
            tok_match(FUNCTION,0); tok_match(ID,0); s = sub_headT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, instead recieved %s\n","function",tok.word);
            while(strcmp(tok.word,"var") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_head: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_head: sync on token %s\n", tok.word);
    }
}

void sub_decTT(){
    fprintf(trace, "%s %s\n","sub_decTT",tok.word);
    switch (tok.tkn) {
        case FUNCTION:
            sub_decs(); cmpd_stmt();
            break;
        case BEGIN:
            cmpd_stmt();
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decTT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decTT: sync on token %s\n", tok.word);
    }
}

void sub_decT(){
    fprintf(trace, "%s %s\n","sub_decT" ,tok.word);
    switch (tok.tkn) {
        case VAR:
            decs(); sub_decTT();
            break;
        case FUNCTION:
            sub_decs(); cmpd_stmt();
            break;
        case BEGIN:
            cmpd_stmt();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, instead recieved %s\n","var","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decT: sync on token %s\n", tok.word);
    }
}

void sub_dec(){
    fprintf(trace, "%s %s\n","sub_dec"  ,tok.word);
    switch (tok.tkn) {
        case FUNCTION:
            sub_head(); sub_decT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead recieved %s\n","function",tok.word);
            while(strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_dec: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_dec: sync on token %s\n", tok.word);
    }
}

void sub_decsT(){
    fprintf(trace, "%s %s\n","sub_decsT"  ,tok.word);
    switch (tok.tkn) {
        case FUNCTION:
            sub_dec(); tok_match(SEMICOLON,0); sub_decsT();
            break;
        case BEGIN:
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead recieved %s\n","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decsT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decsT: sync on token %s\n", tok.word);
    }
}

void sub_decs(){
    fprintf(trace, "%s %s\n","sub_decs" ,tok.word );
    switch (tok.tkn) {
        case FUNCTION:
            sub_dec(); tok_match(SEMICOLON,0); sub_decsT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead recieved %s\n","function",tok.word);
            while(strcmp(tok.word,"begin") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decs: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decs: sync on token %s\n", tok.word);
    }
}

int std_type(){
    fprintf(trace, "%s %s\n","std_type" ,tok.word);
    switch(tok.tkn){
        case INTEGER://assign identifiers type integer or real here
            tok_match(INTEGER,0);
            return TYPEINT;
        case REAL:
            tok_match(REAL,0);
            return TYPEREAL;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting ; instead recieved %s\n",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "std_type: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "std_type: sync on token %s\n", tok.word);
            return TYPEERR;

    }
}

int type(){
    fprintf(trace, "%s %s\n","type"  ,tok.word);
    int s;
    int arr_check;
    switch (tok.tkn) {
        case INTEGER: case REAL:
            s = std_type();
            return s;
        case ARRAY:
            //for type checking in p3 the two num values need to be of type integer
            tok_match(ARRAY,0); tok_match(BRACK,BRACK_OPEN);
            arr_check = tok_match(NUM,0)->type;
            if(arr_check != TYPEINT){
                //throw some error
            }
            tok_match(DOTDOT,0);
            arr_check = tok_match(NUM,0)->type;
            if(arr_check != TYPEINT){
                //error or something
            }
            tok_match(BRACK,BRACK_CLOSE); //require int type
            tok_match(OF,0); s = std_type();
            if (s != TYPEERR){
                s = s + 3;//type is transformed into respective array type
            }
            return s;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, instead recieved %s\n","integer","real","array",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "type: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "type: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

void decsT(){
    fprintf(trace, "%s %s\n","decsT"  ,tok.word);
    int node_color = BLUE;
    switch(tok.tkn){
        case VAR://grab a variable declaration from here
            tok_match(VAR, 0); tok_match(ID,0); tok_match(COLON, 0); type(); tok_match(SEMICOLON,0); decsT();
        break;
        case FUNCTION: case BEGIN:
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s %s %s instead recieved %s\n","var","function","begin",tok.word);
            while(strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "decsT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "decsT: sync on token %s\n", tok.word);
    }
}

void decs(){
    fprintf(trace, "%s %s\n","decs" ,tok.word);
    switch(tok.tkn){
        case VAR:
            //create a blue node here and attach it to what the last function (green node) to put it into scope
            tok_match(VAR, 0); tok_match(ID,0); tok_match(COLON, 0); type(); tok_match(SEMICOLON,0); decsT();
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead recieved %s\n","var",tok.word);
            while(/*!tok_match(FUNCTION,0) || !tok_match(BEGIN,0) ||*/strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "decs: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "decs: sync on token %s\n", tok.word);
    }
}

void id_lstT(){
    fprintf(trace, "%s %s\n","id_lstT"  ,tok.word);
    char syncSet[] = {')'};
    switch(tok.tkn){
        case COMMA:
            //printf("match comma\n");
            tok_match(COMMA,0); tok_ret = tok_match(ID,0);
            printf("id_lstT tok_ret -> word: %s\n", tok_ret->word);
            check_add_node(st,tok_ret->word,TYPEVOID,BLUE,0);
            id_lstT();
        break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            //print error message
            fprintf(list,"SYNERR: tok mismatch expecting %s instead recieved %s\n",syncSet,tok.word);
            while(strcmp(tok.word,")") != 0){//paren close match
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "id_lstT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "id_lstT: sync on token %s\n", tok.word);
    }
}


void id_lst(){
    fprintf(trace, "%s %s\n","id_lst"  ,tok.word);
    char syncSet[] = {')'};
    switch(tok.tkn){
        case ID:
            tok_ret = tok_match(ID,0); //need to go down scope otherwise
            printf("id_lst tok_ret -> word: %s\n", tok_ret->word);
            check_add_node(st,tok_ret->word,TYPEVOID,BLUE,1);
            id_lstT();
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead recieved %s\n","Identifier",tok.word);
            while(strcmp(tok.word,")") != 0){
                //print error message
                //printf("no match\n");
                tok = get_next_token(file,list,token);
                fprintf(trace, "id_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "id_lst: sync on token %s\n", tok.word);
    }
}

void prgmTT(){
    fprintf(trace, "%s %s\n","prgmTT" ,tok.word );
    switch(tok.tkn){
        case FUNCTION:
            sub_decs(); cmpd_stmt(); tok_match(DOT,0);
        break;
        case BEGIN:
            cmpd_stmt(); tok_match(DOT,0);
        break;
        default:
            fprintf(list, "SYNERR: tok mismatch expecting %s instead recieved %s\n","function begin",tok.word);
            while(tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "prgmTT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "prgmTT: sync on token %s\n", tok.word);
    }
}

void prgmT(){
    fprintf(trace, "%s %s\n","prgmT" ,tok.word);
    //printf("tok.tkn in prgmT %d %d\n",tok.tkn, tok.attr.val );
    switch(tok.tkn){
        case VAR:
            decs(); prgmTT();
        break;
        case FUNCTION:
            sub_decs(); cmpd_stmt(); tok_match(DOT,0);
        break;
        case BEGIN:
            cmpd_stmt(); tok_match(DOT,0);
        break;
        default: // this is the otherwise statement
            fprintf(list, "SYNERR: tok mismatch expecting %s instead recieved %s\n","var, function, begin",tok.word);
            while (tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "prgmT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "prgmT: sync on token %s\n", tok.word);
    }
}

void prgm(){
    fprintf(trace, "%s %s\n","prgm" ,tok.word);
    switch(tok.tkn){
        case PROGRAM:
            tok_ret = tok_match(PROGRAM,0); tok_match(ID,0); tok_match(PAREN, PAREN_OPEN); check_add_node(st,tok_ret->word,TYPEVOID,GREEN,0);//create program node here
            id_lst(); tok_match(PAREN,PAREN_CLOSE); tok_match(SEMICOLON,0);
            prgmT();
        break;
        default: //this will be the otherwise statement
            fprintf(list, "SYNERR: tok mismatch expecting %s instead recieved %s\n","program",tok.word);
            while (tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "prgm: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "prgm: sync on token %s\n", tok.word);
    }
}



void parse(FILE *f,FILE *l, FILE *t, FILE *tr){
    file = f;
    list = l;
    token = t;
    trace = tr;
    tok = get_next_token(file,list,token);
    prgm();
    tok_match(EOF,0);
    print_stack(st);
}


#endif
