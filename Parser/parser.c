#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "../LexicalAnalyzer/lexAnalyze.c"
#include "../lexeme.c"

struct Lexeme tok;
FILE *token;
FILE *list;
FILE *file;

void decs();
void std_type();
void sub_decs();
void type();
void cmpd_stmt();
void stmt();
void expr_lst();
void expr();

int tok_match(int t,int val){
    //int t = lex.tkn;
    int type;
    printf("tokcmp %d %d\n",tok.tkn, t);
    if(t == tok.tkn){
        if(t != EOF && val == 0){
            if(tok.tkn == ID ){
                type = tok.type;
                printf("lexeme: %s, type: %d\n",tok.word,tok.type);
                tok = get_next_token(file,list,token);
                return type; //all good here
            }
            else if(tok.tkn == NUM){
                type = tok.type;
                printf("lexeme: %s, type: %d\n",tok.word,tok.type);
                tok = get_next_token(file,list,token);
                return type;
            }
            else{
                tok = get_next_token(file,list,token);
                return 0; //all good here
            }
        }
        if(val != 0 && tok.attr.val == val){
            tok = get_next_token(file,list,token);
            return 0;
        }
        if(val != 0 && tok.attr.val != val){
            printf("Sync Error!! Expecting token %d attr %d instead recieved token %s \n",t,val,tok.word);
            //THIS ALSO NEEDS A PRINTOUT TO THE LISTING FILE
            tok = get_next_token(file,list,token);
        }
        else{
            exit(0);
            //end of parse
            //stop parse here some how
        }
    }//end t == tok

    else{
        printf("Sync Error!! Expecting token %d instead recieved token %s \n",t,tok.word);
        //THIS ALSO NEEDS A PRINTOUT TO THE LISTING FILE
        tok = get_next_token(file,list,token);
    }
    return -1;
}



void sign(){
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
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n","+","-",tok.word);
            while (tok.tkn!=ID||tok.tkn!=NUM||tok.tkn!=NOT||strcmp(tok.word,"(") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void factorT(){
    switch (tok.tkn) {
        case MULOP: case ADDOP: case RELOP: case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            break;
        case PAREN:
            if(tok.tkn == PAREN && tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); expr_lst(); tok_match(PAREN,PAREN_CLOSE);
                break;
            }
        case BRACK:
            if(tok.tkn == BRACK && tok.attr.val == BRACK_OPEN){
                tok_match(BRACK,BRACK_OPEN); expr(); tok_match(BRACK,BRACK_CLOSE);
                break;
            }
            if(tok.attr.val == PAREN_CLOSE || tok.attr.val == BRACK_CLOSE){break;}
        default:
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead recieved %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
            while (tok.tkn != MULOP ||tok.tkn != ADDOP || tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || tok.tkn != COMMA || strcmp(tok.word,")") != 0 || strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void factor(){
    switch (tok.tkn) {
        case ID:
            tok_match(ID,0); factorT();
            break;
        case NUM:
            tok_match(NUM,0);
            break;
        case NOT:
            tok_match(NOT,0); factorT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); expr(); tok_match(PAREN,PAREN_CLOSE);
                break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != MULOP ||tok.tkn != ADDOP || tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || tok.tkn != COMMA || strcmp(tok.word,")") != 0 || strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void termT(){
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
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead recieved %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
            while (tok.tkn != ADDOP || tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || tok.tkn != COMMA || strcmp(tok.word,")") != 0 || strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void term(){
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
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != ADDOP || tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END|| tok.tkn != ELSE || tok.tkn != THEN ||  tok.tkn != DO || tok.tkn != COMMA || strcmp(tok.word,")") != 0 || strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void smpl_exprT(){
    switch (tok.tkn) {
        case ADDOP:
            tok_match(ADDOP,0); term(); smpl_exprT();
            break;
        case RELOP: case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            break;
        case BRACK: case PAREN:
            if(tok.attr.val == BRACK_CLOSE || tok.attr.val == PAREN_CLOSE) break;
        default:
            fprintf(list,"tok mismatch expecting %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, instead recieved %s\n","+ - or","< > <= >= <> =",";","end","else","then","do",";","]",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void smpl_expr(){
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
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","+","-",tok.word);
            while (tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO|| tok.tkn != COMMA || strcmp(tok.word,"]") != 0 || strcmp(tok.word,")") != 0 ) {
                tok = get_next_token(file,list,token);
            }
    }
}

void exprT(){
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
            fprintf(list,"tok mismatch expecting %s, instead recieved %s\n","< > <= >= <> = ; , ) ] end else then do",tok.word);
            while (tok.tkn != COMMA || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || strcmp(tok.word, ")") != 0 || strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void expr(){
    //printf("%d,%d,%s\n",tok.tkn,tok.attr.val,tok.word);
    switch (tok.tkn) {
        case NUM: case ID: case NOT:
            smpl_expr(); exprT();
            break;
        case ADDOP:
            if(tok.attr.val == ADDOP_PL){
                smpl_expr(); exprT();
                break;
            }
            if(tok.attr.val == ADDOP_MN){
                smpl_expr(); exprT();
                break;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                smpl_expr(); exprT();
                break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s, %s, %s, %s, %s, %s, instead recieved %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (tok.tkn != COMMA || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || strcmp(tok.word, ")") != 0 || strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void expr_lstT(){
    switch (tok.tkn) {
        case COMMA:
            tok_match(COMMA,0); expr(); expr_lstT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n",",",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void expr_lst(){
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
            fprintf(list,"tok mismatch expecting %s, %s, %s, %s, %s, %s, instead recieved %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void varT(){
    switch (tok.tkn) {
        case ASSIGNOP:
            break;
        case BRACK:
            if(tok.attr.val == BRACK_OPEN){
                tok_match(BRACK,BRACK_OPEN); expr(); tok_match(BRACK,BRACK_CLOSE);
                break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n",":=","[",tok.word);
            while (tok.tkn != ASSIGNOP) {
                //this is the cause of the infinite loop that happens when procedure statements
                //are tried against this grammar or other wacky sentences that begin with an identity
                printf("Possible procedure statement encountered\n");
                tok = get_next_token(file,list,token);
                if (tok.tkn == EOF){
                    exit(0);
                }
            }
    }
}

void var(){
    switch (tok.tkn) {
        case ID:
            tok_match(ID,0); varT();
            break;
        default:
            fprintf(list,"tok mismatch expecting Identifier, instead recieved %s\n",tok.word);
            while (strcmp(tok.word,":=") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void stmtT(){
    switch (tok.tkn) {
        case ELSE:
            tok_match(ELSE,0); stmt();
            break;
        case SEMICOLON: case END:
            break;
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n","else",";",tok.word);
            while (strcmp(tok.word,"else") != 0 || strcmp(tok.word,";") != 0 || strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void stmt(){
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
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s instead recieved %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"else") != 0 || strcmp(tok.word,";") != 0 || strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void stmt_lstT(){
    switch (tok.tkn) {
        case SEMICOLON:
            tok_match(SEMICOLON,0); stmt(); stmt_lstT();
            break;
        case END:
            break;
        default:
            fprintf(list,"tok mismatch expecting %s,%s, instead recieved %s\n",";","end",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void stmt_lst(){
    switch (tok.tkn) {
        case ID: case BEGIN: case IF: case WHILE:
            stmt(); stmt_lstT();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void opt_stmt(){
    switch (tok.tkn) {
        case ID: case BEGIN: case IF: case WHILE:
            stmt_lst();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void cmpd_stmtT(){
    switch (tok.tkn) {
        case END:
            tok_match(END,0);
            break;
        case BEGIN: case IF: case WHILE: case ID:
            opt_stmt(); tok_match(END,0);
            break;
        default:
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s,%s instead recieved %s\n","end","begin","if","while","do",tok.word);
            while (strcmp(tok.word,".") != 0 || strcmp(tok.word,"function") != 0 || strcmp(tok.word,"begin") != 0 || strcmp(tok.word,";") != 0 || strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void cmpd_stmt(){
    switch (tok.tkn) {
        case BEGIN:
            tok_match(BEGIN,0); cmpd_stmtT();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s, instead recieved %s\n","begin",tok.word);
            while (strcmp(tok.word,".") != 0 || strcmp(tok.word,"function") != 0 || strcmp(tok.word,"begin") != 0 || strcmp(tok.word,";") != 0 || strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void param_lstT(){
    switch (tok.tkn) {
        case SEMICOLON:
            tok_match(SEMICOLON,0); tok_match(ID,0); tok_match(COLON,0); type(); param_lstT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n",";",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void param_lst(){
    switch (tok.tkn) {
        case ID:
            tok_match(ID,0); tok_match(COLON,0); type(); param_lstT();
            break;
        default:
            fprintf(list,"tok mismatch expecting Identifier, instead recieved %s\n",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void args(){
    switch (tok.tkn) {
        case PAREN:
            if (tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); param_lst(); tok_match(PAREN,PAREN_CLOSE); break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s, instead recieved %s\n","(",tok.word);
            while (strcmp(tok.word,":") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_headT(){
    switch (tok.tkn) {
        case COLON:
            tok_match(COLON,0); std_type(); tok_match(SEMICOLON,0);
            break;
        case PAREN:
            if (tok.attr.val == PAREN_OPEN){
                args(); tok_match(COLON,0); std_type(); tok_match(SEMICOLON,0); break;
            }
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n",":","(",tok.word);
            while(strcmp(tok.word,"var") != 0 ||strcmp(tok.word,"begin") != 0 ||strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_head(){
    switch (tok.tkn) {
        case FUNCTION:
            tok_match(FUNCTION,0); tok_match(ID,0); sub_headT();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s, instead recieved %s\n","function",tok.word);
            while(strcmp(tok.word,"var") != 0 ||strcmp(tok.word,"begin") != 0 ||strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_decTT(){
    switch (tok.tkn) {
        case FUNCTION:
            sub_decs(); cmpd_stmt();
            break;
        case BEGIN:
            cmpd_stmt();
        break;
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0 ||strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_decT(){
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
            fprintf(list,"tok mismatch expecting %s, %s, %s, instead recieved %s\n","var","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0 ||strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_dec(){
    switch (tok.tkn) {
        case FUNCTION:
            sub_head(); sub_decT();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s instead recieved %s\n","function",tok.word);
            while(strcmp(tok.word,"begin") != 0 ||strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_decsT(){
    switch (tok.tkn) {
        case FUNCTION:
            sub_dec(); tok_match(SEMICOLON,0); sub_decsT();
            break;
        case BEGIN:
            break;
        default:
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void sub_decs(){
    switch (tok.tkn) {
        case FUNCTION:
            sub_dec(); tok_match(SEMICOLON,0); sub_decsT();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s instead recieved %s\n","function",tok.word);
            while(strcmp(tok.word,"begin") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void std_type(){
    switch(tok.tkn){
        case INTEGER:
            tok_match(INTEGER,0);
            break;
        case REAL:
            tok_match(REAL,0);
            break;
        default:
            fprintf(list,"tok mismatch expecting ; instead recieved %s\n",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void type(){
    switch (tok.tkn) {
        case INTEGER: case REAL:
            std_type();
            break;
        case ARRAY:
            //for type checking in p3 the two num values need to be of type integer
            tok_match(ARRAY,0); tok_match(BRACK,BRACK_OPEN); tok_match(NUM,0); tok_match(DOTDOT,0); tok_match(NUM,0); tok_match(BRACK,BRACK_CLOSE);
            tok_match(OF,0); std_type();
            break;
        default:
            fprintf(list,"tok mismatch expecting %s, %s, %s, instead recieved %s\n","integer","real","array",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
            }
    }
}

void decsT(){
    switch(tok.tkn){
        case VAR:
            tok_match(VAR, 0); tok_match(ID,0); tok_match(COLON, 0); type(); tok_match(SEMICOLON,0); decsT();
        break;
        case FUNCTION: case BEGIN:
        break;
        default:
            fprintf(list,"tok mismatch expecting %s %s %s instead recieved %s\n","var","function","begin",tok.word);
            while(strcmp(tok.word,"function") != 0 || strcmp(tok.word,"begin") != 0){
                //print error message
                tok = get_next_token(file,list,token);
            }
    }
}

void decs(){
    switch(tok.tkn){
        case VAR:
            tok_match(VAR, 0); tok_match(ID,0); tok_match(COLON, 0); type(); tok_match(SEMICOLON,0); decsT();
        break;
        default:
            fprintf(list,"tok mismatch expecting %s instead recieved %s\n","var",tok.word);
            while(/*!tok_match(FUNCTION,0) || !tok_match(BEGIN,0) ||*/strcmp(tok.word,"function") != 0 || strcmp(tok.word,"begin") != 0){
                //print error message
                tok = get_next_token(file,list,token);
            }
    }
}

void id_lstT(){
    char syncSet[] = {')'};
    switch(tok.tkn){
        case COMMA:
            //printf("match comma\n");
            tok_match(COMMA,0); tok_match(ID,0); id_lstT();
        break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            //print error message
            fprintf(list,"tok mismatch expecting %s instead recieved %s\n",syncSet,tok.word);
            while(strcmp(tok.word,")") != 0){//paren close match
                //print error message
                tok = get_next_token(file,list,token);
            }
    }
}


void id_lst(){
    char syncSet[] = {')'};
    switch(tok.tkn){
        case ID:
            tok_match(ID,0); id_lstT();
        break;
        default:
            fprintf(list,"tok mismatch expecting %s instead recieved %s\n","Identifier",tok.word);
            while(strcmp(tok.word,")") != 0){
                //print error message
                //printf("no match\n");
                tok = get_next_token(file,list,token);
                //printf("%s\n",tok.word );
            }
    }
}

void prgmTT(){
    switch(tok.tkn){
        case FUNCTION:
            sub_decs(); cmpd_stmt(); tok_match(DOT,0);
        break;
        case BEGIN:
            cmpd_stmt(); tok_match(DOT,0);
        break;
        default:
            fprintf(list, "tok mismatch expecting %s instead recieved %s\n","function begin",tok.word);
            while(tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
            }
    }
}

void prgmT(){
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
            while (tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
            }
    }
}

void prgm(){
    switch(tok.tkn){
        case PROGRAM:
            tok_match(PROGRAM,0); tok_match(ID,0); tok_match(PAREN, PAREN_OPEN); id_lst(); tok_match(PAREN,PAREN_CLOSE); tok_match(SEMICOLON,0); prgmT();
        break;
        default: //this will be the otherwise statement
            while (tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
            }
    }
}



void parse(FILE *f,FILE *l, FILE *t){
    file = f;
    list = l;
    token = t;
    tok = get_next_token(file,list,token);
    prgm();
    tok_match(EOF,0);
}


#endif
