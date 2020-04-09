#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "../LexicalAnalyzer/lexAnalyze.c"
#include "../lexeme.c"
#include "../node/nodetest.c"

struct Lexeme tok;
Lex tok_lex;
struct node_stack parsestack;
stack_ptr st = &parsestack;
FILE *token;
FILE *list;
FILE *file;

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
        if(t != EOF && val == 0){
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
        if(val != 0 && tok.attr.val == val){
            tok = get_next_token(file,list,token);
            return tok_lex;
        }
        if(val != 0 && tok.attr.val != val){
            printf("Sync Error!! Expecting token %d attr %d instead recieved token %s \n",t,val,tok.word);
            fprintf(list,"tok mismatch, expecting token code %d instead recieved token %s\n",t,tok.word);
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
        fprintf(list,"tok mismatch, expecting token code %d instead recieved token %s\n",t,tok.word);
        tok = get_next_token(file,list,token);
    }
    return NULL;
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

int factorT(){
    switch (tok.tkn) {
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
        case PAREN:
            if(tok.tkn == PAREN && tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); expr_lst(); tok_match(PAREN,PAREN_CLOSE);
                return 0;
            }
        case BRACK:
            if(tok.tkn == BRACK && tok.attr.val == BRACK_OPEN){
                tok_match(BRACK,BRACK_OPEN); expr(); tok_match(BRACK,BRACK_CLOSE);
                return 0;
            }
            if(tok.attr.val == PAREN_CLOSE || tok.attr.val == BRACK_CLOSE){return 0;}
        default:
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead recieved %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
            while (tok.tkn != MULOP ||tok.tkn != ADDOP || tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || tok.tkn != COMMA || strcmp(tok.word,")") != 0 || strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
            }
            return TYPEERR;
    }
}
//NEED TO FIX RVALUE tok_match stuff
int factor(){
    int r_val = 0;
    Lex tok_ret;
    switch (tok.tkn) {
        case ID:
            tok_ret = tok_match(ID,0); factorT();

            break;
        case NUM:
            tok_ret = tok_match(NUM,0);
            r_val = get_id_type(st,tok_ret->word);
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
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s, instead recieved %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != MULOP ||tok.tkn != ADDOP || tok.tkn != RELOP || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || tok.tkn != COMMA || strcmp(tok.word,")") != 0 || strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
            }
    }
    return TYPEERR;
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

int expr(){
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
            fprintf(list,"tok mismatch expecting %s, %s, %s, %s, %s, %s, instead recieved %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (tok.tkn != COMMA || tok.tkn != SEMICOLON || tok.tkn != END || tok.tkn != ELSE || tok.tkn != THEN || tok.tkn != DO || strcmp(tok.word, ")") != 0 || strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
            }
            return 0;
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
            fprintf(list,"tok mismatch expecting %s,%s,%s,%s,%s,%s instead recieved %s\n","Identifier","end","begin","if","while","do",tok.word);
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
//THIS IS A BLUE NODE FABRICATOR
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
//THIS IS A BLUE NODE FABRICATOR
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
//GREEN NODE FABRICATOR
int sub_headT(){
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
            fprintf(list,"tok mismatch expecting %s, %s, instead recieved %s\n",":","(",tok.word);
            while(strcmp(tok.word,"var") != 0 ||strcmp(tok.word,"begin") != 0 ||strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
            }
            return TYPEERR;
    }
}
//GREEN NODE FABRICATOR
void sub_head(){
    int s; //function return type
    switch (tok.tkn) {
        case FUNCTION:
            tok_match(FUNCTION,0); tok_match(ID,0); s = sub_headT();
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

int std_type(){
    switch(tok.tkn){
        case INTEGER://assign identifiers type integer or real here
            tok_match(INTEGER,0);
            return TYPEINT;
        case REAL:
            tok_match(REAL,0);
            return TYPEREAL;
        default:
            fprintf(list,"tok mismatch expecting ; instead recieved %s\n",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
            }
            return TYPEERR;

    }
}

int type(){
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
            fprintf(list,"tok mismatch expecting %s, %s, %s, instead recieved %s\n","integer","real","array",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
            }
            return TYPEERR;
    }
}

void decsT(){
    int node_color = BLUE;
    switch(tok.tkn){
        case VAR://grab a variable declaration from here
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
            //create a blue node here and attach it to what the last function (green node) to put it into scope
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
