#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "../LexicalAnalyzer/lexAnalyze.c"
#include "../lexeme.c"
#include "../node/nodetest.c"
#include "../tokcode2str.c"

struct Lexeme tok;
struct Lexeme temp; //temp value that needs to be outside tok_match as to not be destroyed by function return
struct Lexeme tok_ret;
struct Lexeme bad_ret;
struct node_stack parsestack;
stack_ptr st = &parsestack;
FILE *token;
FILE *list;
FILE *file;
FILE *trace;
FILE *nodes;
FILE *ad;

int scope_flag;
int scope_level=0;


//function call variables
int param_count;//parameters present in call
int f_params;//amount of parameters specified by actual function
char f_name[15];

//addressing node variables
int addr;


void decs();
int std_type();
void sub_decs();
int type();
int cmpd_stmt();
int stmt();
int expr_lst();
int expr();

struct Lexeme tok_match(int t,int val){
    //int t = lex.tkn;
    //int type;
    bad_ret.type = TYPEERR;
    memcpy(&temp,&tok,sizeof tok);
    //tok_lex = &temp;//need to memcpy this instead of just straight addressing, or just use a temp lexeme that copies the current token
    printf("tokcmp %s %s\n",tokcode2str(tok.tkn), tokcode2str(t));
        if(t == tok.tkn){
            if(t != EOF && val == 0 && tok.tkn != EOF){
                if(tok.tkn == ID ){
                    printf("lexeme: %s, type: %d\n",tok.word,get_id_type(st,tok.word));
                    tok = get_next_token(file,list,token);
                    return temp; //all good here
                }
                else if(tok.tkn == NUM){
                    printf("lexeme: %s, type: %d\n",tok.word,tok.type);
                    tok = get_next_token(file,list,token);
                    return temp;
                }
                else{
                    tok = get_next_token(file,list,token);
                    return temp; //all good here
                }
            }
            if(t == EOF){
                //exit(0);
            }
            if(val != 0 && tok.attr.val == val){
                tok = get_next_token(file,list,token);
                return temp;
            }
            if(val != 0 && tok.attr.val != val){
                printf("Sync Error!! Expecting %s instead received token %s \n",tokcode2str(t),tok.word);
                fprintf(list,"SYNERR: token mismatch, expecting token %s instead received token %s\n",tokcode2str(t),tok.word);
                tok = get_next_token(file,list,token);
                return bad_ret;
            }
            else{
                //exit(0);
                //end of parse
                //stop parse here some how
            }
        }//end t == tok

        else{
            printf("Sync Error!! Expecting %s instead received token %s \n",tokcode2str(t),tok.word);
            fprintf(list,"SYNERR: token mismatch, expecting token %s instead received token %s\n",tokcode2str(t),tok.word);
            tok = get_next_token(file,list,token);
            return bad_ret;
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
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n","+","-",tok.word);
            while (tok.tkn!=ID && tok.tkn!=NUM && tok.tkn!=NOT && strcmp(tok.word,"(") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "sign: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sign: sync on token %s\n",tok.word );
    }
}

int factorT(int i_val){
    int r_val;
    int f_val;
    char brack[15];
    fprintf(trace, "%s %s\n","factorT",tok.word );
    switch (tok.tkn) {
        //start epsilons
        case MULOP:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case ADDOP:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case RELOP:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case SEMICOLON:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case END:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case ELSE:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case THEN:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case DO:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case COMMA:
            r_val=i_val;
            if(i_val == TYPEERR){
                printf("leaving factorT with type error\n");
                fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        //end epsilons
        case PAREN: //function call !!!
            if(tok.tkn == PAREN && tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN);
                r_val = expr_lst(); //type void, but the individual types for parameters must be checked for the function call
                //only a function call is going to use this expr_lst
                tok_match(PAREN,PAREN_CLOSE);
                return r_val;
            }
        case BRACK: //array assign or look up, but all this needs to do is give back type info
            if(tok.tkn == BRACK && tok.attr.val == BRACK_OPEN){
                tok_match(BRACK,BRACK_OPEN);
                f_val = expr();//must be an integer type
                if(f_val == TYPEINT){
                    if(i_val == TYPEARR_INT){
                        r_val = TYPEINT;
                    }
                    else if(i_val == TYPEARR_REAL){
                        r_val = TYPEREAL;
                    }
                    else{
                        fprintf(list,"SEMERR: Non-array identifiers cannot be indexed\n");
                        r_val = TYPEERR;
                    }
                }
                else if(f_val != TYPEINT) {
                    fprintf(list, "SEMERR: Identifiers cannot be indexed with a non-integer\n" );
                    r_val = TYPEERR;
                }

                tok_ret = tok_match(BRACK,BRACK_CLOSE);
                strcpy(brack,tok_ret.word);
                if(i_val == TYPEERR){
                    printf("leaving factorT with type error\n");
                    fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                printf("array type return: %d\n",r_val);
                return r_val;
            }
            if(tok.attr.val == PAREN_CLOSE || tok.attr.val == BRACK_CLOSE){
                //these two cases are also part of epsilons
                r_val=i_val;
                if(i_val == TYPEERR){
                    printf("leaving factorT with type error\n");
                    fprintf(trace, "%s %s\n","factorT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead received %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
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
    char name[15];
    switch (tok.tkn) {
        case ID:
            tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            int i_val = get_id_type(st,tok_ret.word);
            printf("factor get_id_type for %s: %d\n", tok_ret.word,i_val);
            if(i_val == TYPEERR){
                fprintf(list, "SEMERR: No id with name %s\n", name);
            }
            r_val = factorT(i_val);
            return r_val;
        case NUM:
            tok_ret = tok_match(NUM,0);
            return tok_ret.type;
        case NOT:
            tok_match(NOT,0); r_val = factor();
            if(r_val == TYPEBOOL){
                return TYPEBOOL;
            }
            else{
                fprintf(list,"SEMERR: type mismatch, expecting typecode: %d received: %d\n",TYPEBOOL,r_val);
                return TYPEERR;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                tok_match(PAREN,PAREN_OPEN); int r_val = expr(); tok_match(PAREN,PAREN_CLOSE);
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead received %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != MULOP && tok.tkn != ADDOP && tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "factor: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "factor: sync on token %s\n", tok.word);
    }
    return TYPEERR;
}

int termT(int i_val){
    //the i_val is the factor type information
    int r_val;
    int f_val;
    char mulop[15];
    fprintf(trace, "%s %s\n","termT",tok.word );
    switch (tok.tkn) {
        case MULOP:
            tok_ret = tok_match(MULOP,0); strcpy(mulop,tok_ret.word);
            f_val = factor();
            printf("termT i_val: %d f_val: %d mulop: %s\n",i_val,f_val,mulop);
                    if(strcmp(mulop,"and") == 0){
                        // "and" needs bool
                        if(i_val == TYPEBOOL && f_val == TYPEBOOL){
                            r_val = TYPEBOOL;
                        }
                        else if(i_val == TYPEERR || f_val == TYPEERR){
                            r_val = TYPEERR;
                        }
                        else{
                            fprintf(list, "SEMERR: invalid operands for operator and, require type boolean\n" );
                            r_val = TYPEERR;
                        }
                        printf("mulop AND encountered\n");
                    }
                    else if(strcmp(mulop,"mod") == 0){
                        // "mod"
                        if(i_val == TYPEINT && f_val == TYPEINT){
                            r_val = TYPEINT;
                        }
                        else{
                            fprintf(list, "SEMERR: invalid operands for operator mod, require type integer\n" );
                            r_val = TYPEERR;
                        }
                        printf("mulop MOD encountered\n");
                    }
                    else if(strcmp(mulop,"*") == 0){
                        // "*"
                        if(i_val == TYPEINT && f_val == TYPEINT){
                            r_val = TYPEINT;
                        }
                        else if(i_val == TYPEINT && f_val == TYPEREAL){
                            r_val = TYPEREAL;
                        }
                        else if(i_val == TYPEREAL && f_val == TYPEINT){
                            r_val = TYPEREAL;
                        }
                        else if(i_val == TYPEREAL && f_val == TYPEREAL){
                            r_val = TYPEREAL;
                        }
                        else{
                            fprintf(list, "SEMERR: invalid operands for operator *, require either type real or integer\n");
                            r_val = TYPEERR;
                        }
                        printf("mulop * encountered\n");
                    }
                    else if(strcmp(mulop,"/") == 0){
                        // "/"
                        if(i_val == TYPEINT && f_val == TYPEINT){
                            r_val = TYPEREAL;
                        }
                        else if(i_val == TYPEINT && f_val == TYPEREAL){
                            //r_val = TYPEREAL;
                            r_val = TYPEERR;
                            fprintf(list, "SEMERR: Mixed mode arithmetic is not supported\n" );
                        }
                        else if(i_val == TYPEREAL && f_val == TYPEINT){
                            //r_val = TYPEREAL;
                            r_val = TYPEERR;
                            fprintf(list, "SEMERR: Mixed mode arithmetic is not supported\n" );
                        }
                        else if(i_val == TYPEREAL && f_val == TYPEREAL){
                            r_val = TYPEREAL;
                        }
                        else{
                            fprintf(list, "SEMERR: invalid operands for operator /, require either type real or integer\n" );
                            r_val = TYPEERR;
                        }
                        printf("mulop / encountered\n");
                    }
                    else if(strcmp(mulop,"div") == 0){
                        //DIV
                        if(i_val == TYPEINT && f_val == TYPEINT){
                            r_val = TYPEINT;
                        }
                        else{
                            fprintf(list, "SEMERR: invalid operands for operator div, require type integer\n" );
                            r_val = TYPEERR;
                        }
                        printf("mulop DIV encountered\n");
                    }
            r_val = termT(r_val);
            if(i_val == TYPEERR){
                printf("leaving termT with type error\n");
                fprintf(trace, "%s %s\n","termT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case ADDOP: case RELOP: case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            r_val = i_val;
            if(i_val == TYPEERR){
                printf("leaving termT with type error\n");
                fprintf(trace, "%s %s\n","termT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case BRACK: case PAREN:
            if(tok.attr.val == BRACK_CLOSE || tok.attr.val == PAREN_CLOSE){
                r_val = i_val;
                if(i_val == TYPEERR){
                    printf("leaving termT with type error\n");
                    fprintf(trace, "%s %s\n","termT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s, instead received %s\n","* / div mod and","+ - or","< > <= >= <> ==",";","end","else","then","do",",","]",")",tok.word);
            while (tok.tkn != ADDOP && tok.tkn != RELOP && strcmp(tok.word,";") != 0 && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "termT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "termT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int term(){
    int r_val;
    int f_val;
    fprintf(trace, "%s %s\n","term" ,tok.word);
    switch (tok.tkn) {
        case ID: case NUM: case NOT:;
            f_val = factor(); r_val = termT(f_val);
            return r_val;
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                f_val = factor(); r_val = termT(f_val);
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead received %s\n","Identifier","Number","not","(",tok.word);
            while (tok.tkn != ADDOP && tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN &&  tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,")") != 0 && strcmp(tok.word,"]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "term: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "term: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int smpl_exprT(int i_val){
    int r_val;
    int f_val;
    char addop[15];
    fprintf(trace, "%s %s\n","smpl_exprT",tok.word );
    switch (tok.tkn) {
        case ADDOP:
            tok_ret = tok_match(ADDOP,0); strcpy(addop,tok_ret.word);
            f_val = term();
            printf("ADDOP: %s i_val: %d f_val: %d\n",addop,i_val,f_val);
            if(strcmp(addop,"or")==0){
                if(i_val == TYPEBOOL && f_val == TYPEBOOL){
                    r_val = TYPEBOOL;
                }
                else if(i_val == TYPEERR || f_val == TYPEERR){
                    r_val = TYPEERR;
                }
                else{
                    fprintf(list, "SEMERR: invalid simple expression involving the or of non boolean element\n" );
                    r_val = TYPEERR;
                }
            }
            if(strcmp(addop,"+") == 0 || strcmp(addop,"-") == 0){
                if(i_val == TYPEINT && f_val == TYPEINT){
                    r_val = TYPEINT;
                }
                else if(i_val == TYPEREAL && f_val == TYPEREAL){
                    r_val = TYPEREAL;
                }
                else if(i_val == TYPEREAL && f_val == TYPEINT){
                    //r_val = TYPEREAL;
                    r_val = TYPEERR;
                    fprintf(list, "SEMERR: Mixed mode arithmetic is not supported\n" );

                }
                else if(i_val == TYPEINT && f_val == TYPEREAL){
                    //r_val = TYPEREAL;
                    r_val = TYPEERR;
                    fprintf(list, "SEMERR: Mixed mode arithmetic is not supported\n" );
                }
                else if(i_val == TYPEERR || f_val == TYPEERR){
                    r_val = TYPEERR;
                }
                else{
                    fprintf(list, "SEMERR: invalid simple expression involving a boolean with an non-boolean operator (+,-)\n" );
                    r_val = TYPEERR;
                }
            }
            r_val = smpl_exprT(r_val);
            if(i_val == TYPEERR){
                printf("leaving smpl_exprT with type error\n");
                fprintf(trace, "%s %s\n","smpl_exprT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case RELOP: case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            r_val = i_val;
            if(i_val == TYPEERR){
                printf("leaving smpl_exprT with type error\n");
                fprintf(trace, "%s %s\n","smpl_exprT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case BRACK: case PAREN:
            if(tok.attr.val == BRACK_CLOSE || tok.attr.val == PAREN_CLOSE){
                r_val = i_val;
                if(i_val == TYPEERR){
                    printf("leaving smpl_exprT with type error\n");
                    fprintf(trace, "%s %s\n","smpl_exprT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, instead received %s\n","+ - or","< > <= >= <> =",";","end","else","then","do",";","]",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "smpl_exprT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "smpl_exprT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int smpl_expr(){
    fprintf(trace, "%s %s\n","smpl_expr",tok.word );
    int f_val;
    int r_val;
    switch (tok.tkn) {
        case ID: case NUM: case NOT:
            f_val = term(); r_val = smpl_exprT(f_val);
            return r_val;
        case ADDOP://no booleans allowed
            if(tok.attr.val == ADDOP_PL || tok.attr.val == ADDOP_MN){
                sign(); f_val = term(); r_val = smpl_exprT(f_val);
                if(f_val == TYPEBOOL || r_val == TYPEBOOL){
                    fprintf(list, "SEMERR: Boolean with +/- has no valid meaning\n");
                    r_val = TYPEERR;
                }
                printf("smpl_expr +- r_val: %d\n",r_val );
                return r_val;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                f_val = term(); r_val = smpl_exprT(f_val);
                printf("smpl_expr ( r_val: %d\n",r_val );
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s, instead received %s\n","Identifier","Number","not","+","-",tok.word);
            while (tok.tkn != RELOP && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && tok.tkn != COMMA && strcmp(tok.word,"]") != 0 && strcmp(tok.word,")") != 0 ) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "smpl_expr: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "smpl_expr: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int exprT(int i_val){
    int f_val;
    int r_val;
    char relop[15];
    fprintf(trace, "%s %s\n","exprT" ,tok.word);
    switch (tok.tkn) {
        case RELOP:
            tok_ret = tok_match(RELOP,0);
            strcpy(relop,tok_ret.word);
            f_val = smpl_expr();
            printf("relop: %s i_val: %d f_val: %d\n",relop,i_val,f_val);
            if(strcmp(relop,"=")==0){
                //printf("going into = relop with types i_val: %d f_val: %s\n", i_val,f_val);
                if(i_val == TYPEBOOL && f_val == TYPEBOOL){
                    r_val = TYPEBOOL;
                }
                else if ((i_val == TYPEINT && f_val == TYPEINT) || (i_val == TYPEREAL || f_val == TYPEREAL)) {
                    r_val = TYPEBOOL;
                }
                else {
                    fprintf(list, "SEMERR: invalid operands for operator =, operands must be of type boolean or operands must be of type integer/real\n");
                    r_val = TYPEERR;
                }
            }
            else {
                if(i_val == TYPEINT && f_val == TYPEINT){
                    r_val = TYPEBOOL;
                }
                else if(i_val == TYPEREAL && f_val == TYPEINT){
                    //r_val = TYPEBOOL;
                    r_val = TYPEERR;
                    fprintf(list, "SEMERR: Mixed mode arithmetic is not supported\n" );
                }
                else if(i_val == TYPEINT && f_val == TYPEREAL){
                    //r_val = TYPEBOOL;
                    r_val = TYPEERR;
                    fprintf(list, "SEMERR: Mixed mode arithmetic is not supported\n" );
                }
                else if(i_val == TYPEREAL && f_val == TYPEREAL){
                    r_val = TYPEBOOL;
                }
                else if(i_val == TYPEERR || f_val == TYPEERR){
                    //THIS MAKES SURE ERRORS ARE NOT SPAMMED
                    r_val = TYPEERR;
                }
                else {
                    fprintf(list,"SEMERR: invalid operands for inequality operator %s, operands must be of the type integer or real\n",relop);
                    r_val = TYPEERR;
                }
            }
            if(i_val == TYPEERR){
                printf("leaving exprT with type error\n");
                fprintf(trace, "%s %s\n","exprT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case SEMICOLON: case END: case ELSE: case THEN: case DO: case COMMA:
            r_val = i_val;
            if(i_val == TYPEERR){
                printf("leaving exprT with type error\n");
                fprintf(trace, "%s %s\n","exprT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case BRACK:
            if(tok.attr.val == BRACK_CLOSE){
                r_val = i_val;
                if(i_val == TYPEERR){
                    printf("leaving exprT with type error\n");
                    fprintf(trace, "%s %s\n","exprT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                return r_val;
            }
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                r_val = i_val;
                if(i_val == TYPEERR){
                    printf("leaving exprT with type error\n");
                    fprintf(trace, "%s %s\n","exprT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: SYNERR: tok mismatch expecting %s, instead received %s\n","< > <= >= <> = ; , ) ] end else then do",tok.word);
            while (tok.tkn != COMMA && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && strcmp(tok.word, ")") != 0 && strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "exprT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "exprT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int expr(){
    int r_val;
    int f_val;
    fprintf(trace, "%s %s\n","expr",tok.word );
    printf("expr: %d,%s\n",tok.tkn,tok.word);
    printf("%s %s\n","expr",tok.word);
    switch (tok.tkn) {
        case NUM: case ID: case NOT:
            f_val = smpl_expr(); r_val = exprT(f_val);
            return r_val;
        case ADDOP:
            if(strcmp(tok.word,"+")==0){
                f_val = smpl_expr(); r_val = exprT(f_val);
                return r_val;
            }
            if(strcmp(tok.word,"-")==0){
                f_val = smpl_expr(); r_val = exprT(f_val);
                return r_val;
            }
        case PAREN:
            if(strcmp(tok.word,"(")==0){
                f_val = smpl_expr(); r_val = exprT(f_val);
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, %s, %s, %s, instead received %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (tok.tkn != COMMA && tok.tkn != SEMICOLON && tok.tkn != END && tok.tkn != ELSE && tok.tkn != THEN && tok.tkn != DO && strcmp(tok.word, ")") != 0 && strcmp(tok.word, "]") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "expr: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "expr: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int expr_lstT(int i_val){
    int f_val;
    int r_val;
    int exp_type;
    fprintf(trace, "%s %s\n","expr_lstT",tok.word );
    switch (tok.tkn) {
        case COMMA:
            param_count++;
            exp_type = get_func_param_type(st,f_name,param_count);
            tok_match(COMMA,0); f_val = expr();
            if(exp_type != f_val){
                fprintf(list, "SEMERR: Function parameter type mismatch expecting %s, received %s\n", tokcode2str(exp_type),tokcode2str(f_val));
                f_val = TYPEERR;
            }
            r_val = expr_lstT(f_val);

            if(i_val == TYPEERR){
                printf("leaving expr_lstT with type error\n");
                fprintf(trace, "%s %s\n","expr_lstT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case PAREN://end of function call, check for param_count match
            if(tok.attr.val == PAREN_CLOSE){

                if(param_count > f_params){
                    fprintf(list, "SEMERR: function call has too many arguements %d given, %d required\n", param_count,f_params);
                }
                if(param_count < f_params){
                    fprintf(list, "SEMERR: Function call has too few arguements %d given, %d required\n", param_count,f_params);
                }

                if(i_val == TYPEERR){
                    printf("leaving expr_lstT with type error\n");
                    fprintf(trace, "%s %s\n","expr_lstT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                r_val = i_val;
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n",",",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "expr_lstT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "expr_lstT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int expr_lst(){
    int f_val;
    int r_val;
    int exp_type;
    int f_found=1;
    strcpy(f_name,tok_ret.word);
    int f_type = get_id_type(st,f_name);
    param_count=0;
    f_params = get_func_param_count(st,f_name);
    fprintf(trace, "%s %s\n","expr_lst" ,tok.word);
    if(in_scope(st,f_name)==-1){
            fprintf(list,"SEMERR: function %s either not declared or not within scope\n",f_name);
            f_found =0;
    }
    switch (tok.tkn) {
        case NUM: case ID: case NOT:
            param_count++;
            exp_type = get_func_param_type(st,f_name,param_count);
            f_val = expr();
            if(exp_type != f_val){
                if(f_found)
                    fprintf(list, "SEMERR: Function parameter type mismatch expecting %s, received %s\n", tokcode2str(exp_type),tokcode2str(f_val));
                f_val = TYPEERR;
            }
            r_val = expr_lstT(f_val);
            if(r_val != TYPEERR){
                r_val = f_type;
            }
            return r_val;
        case ADDOP:
            if(tok.attr.val == ADDOP_PL){
                param_count++;
                exp_type = get_func_param_type(st,f_name,param_count);
                f_val = expr();
                if(exp_type != f_val){
                    if(f_found)
                        fprintf(list, "SEMERR: Function parameter type mismatch expecting %s, received %s\n", tokcode2str(exp_type),tokcode2str(f_val));
                    f_val = TYPEERR;
                }
                r_val = expr_lstT(f_val);
                if(r_val != TYPEERR){
                    r_val = f_type;
                }
                return r_val;
            }
            if(tok.attr.val == ADDOP_MN){
                param_count++;
                exp_type = get_func_param_type(st,f_name,param_count);
                f_val = expr();
                if(exp_type != f_val){
                    if(f_found)
                        fprintf(list, "SEMERR: Function parameter type mismatch expecting %s, received %s\n", tokcode2str(exp_type),tokcode2str(f_val));
                    f_val = TYPEERR;
                }
                r_val = expr_lstT(f_val);
                if(r_val != TYPEERR){
                    r_val = f_type;
                }
                return r_val;
            }
        case PAREN:
            if(tok.attr.val == PAREN_OPEN){
                param_count++;
                exp_type = get_func_param_type(st,f_name,param_count);
                f_val = expr();
                if(exp_type != f_val){
                    if(f_found)
                        fprintf(list, "SEMERR: Function parameter type mismatch expecting %s, received %s\n", tokcode2str(exp_type),tokcode2str(f_val));
                    f_val = TYPEERR;
                }
                r_val = expr_lstT(f_val);
                if(r_val != TYPEERR){
                    r_val = f_type;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, %s, %s, %s, instead received %s\n","Identifier","Number","not","+","-","(",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "expr_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "expr_lst: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int varT(int i_val){
    int r_val;
    int f_val;
    fprintf(trace, "%s %s\n","varT",tok.word );
    switch (tok.tkn) {
        case ASSIGNOP:
            r_val = i_val;
            if(i_val == TYPEERR){
                printf("leaving varT with type error\n");
                fprintf(trace, "%s %s\n","varT","IMM TYPEERR RET");
                r_val = TYPEERR;
                return r_val;
            }
            return r_val;
        case BRACK:
            if(tok.attr.val == BRACK_OPEN){
                f_val = expr();//must be an integer type
                if(f_val == TYPEINT){
                    if(i_val == TYPEARR_INT){
                        r_val = TYPEINT;
                    }
                    else if(i_val == TYPEARR_REAL){
                        r_val = TYPEREAL;
                    }
                    else{
                        r_val = TYPEERR;
                        fprintf(list,"SEMERR: Non-array identifiers cannot be indexed\n");
                    }
                }
                else if(f_val != TYPEINT) {
                    fprintf(list, "SEMERR: Array cannot be indexed with non-integer\n" );
                    r_val = TYPEERR;
                }

                tok_match(BRACK,BRACK_CLOSE);
                if(i_val == TYPEERR){
                    printf("leaving varT with type error\n");
                    fprintf(trace, "%s %s\n","varT","IMM TYPEERR RET");
                    r_val = TYPEERR;
                    return r_val;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n",":=","[",tok.word);
            while (tok.tkn != ASSIGNOP) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "varT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "varT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int var(){
    int r_val;
    char name[15];
    fprintf(trace, "%s %s\n","var",tok.word );
    switch (tok.tkn) {
        case ID:
            tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            int i_val = get_id_type(st,name);
            printf("stmt before expr1\n");
            r_val = varT(i_val);
            if(i_val == TYPEERR){
                fprintf(list, "SEMERR: Identifier %s is either not defined or not within scope\n", name);
            }
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting Identifier, instead received %s\n",tok.word);
            while (strcmp(tok.word,":=") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "var: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "var: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int stmtT(int i_val){//need to return type something on correct stmtT
    int r_val;
    int f_val;
    fprintf(trace, "%s %s\n","stmtT",tok.word );
    switch (tok.tkn) {
        case ELSE:
            tok_match(ELSE,0); f_val = stmt();
            if(f_val == TYPEVOID){
                r_val = TYPEVOID;
            }
            else {
                r_val = TYPEERR;
                //print error in stmt function
            }
            return r_val;
        case SEMICOLON: case END:
            r_val = i_val;
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n","else",";",tok.word);
            while (strcmp(tok.word,"else") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmtT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "stmtT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

int stmt(){//need to return type void on correct stmt
    int r_val;
    int f_val;
    int f2_val;
    fprintf(trace, "%s %s\n","stmt",tok.word );
    //printf("%d,%d,%s\n",tok.tkn,tok.attr.val,tok.word);
    switch (tok.tkn) {
        case ID:
            f_val = var();printf("stmt before expr1\n"); tok_match(ASSIGNOP,0);
            printf("stmt before expr2\n");
            f2_val = expr();
            if(f_val == TYPEINT && f2_val == TYPEINT){
                r_val = TYPEVOID;
            }
            else if(f_val == TYPEREAL && f2_val == TYPEREAL){
                r_val = TYPEVOID;
            }
            else{
                if(f_val != TYPEERR){
                    fprintf(list, "SEMERR: Type mismatch on assignment expecting type %s received type %s\n",tokcode2str(f_val),tokcode2str(f2_val));
                }
                else{
                    //this semantic error is handled within the var function
                }
                r_val = TYPEERR;
            }

            return r_val;
        case BEGIN:
            f_val = cmpd_stmt();
            if(f_val == TYPEVOID){
                r_val = TYPEVOID;
            }
            return r_val;
        case IF:
            tok_match(IF,0); f_val = expr(); tok_match(THEN,0); f2_val = stmt(); r_val = stmtT(f2_val);
            if(f_val == TYPEBOOL){
                if(r_val == TYPEVOID){
                    //for r_val to be type void, f2_val must be type void
                    r_val = TYPEVOID;
                }
                else if(f2_val != TYPEVOID){
                    r_val = TYPEERR;
                }
                else if(r_val != TYPEVOID){
                    r_val = TYPEERR;
                }
            }
            else{
                r_val = TYPEERR;
                fprintf(list, "SEMERR: if statement cannot be evaluated with non-boolean expression\n");
            }
            return r_val;
        case WHILE:
            tok_match(WHILE,0); expr(); tok_match(DO,0); stmt();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s instead received %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"else") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmt: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "stmt: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

int stmt_lstT(int i_val){
    int f_val;
    int r_val;
    fprintf(trace, "%s %s\n","stmt_lstT" ,tok.word);
    switch (tok.tkn) {
        case SEMICOLON:
            tok_match(SEMICOLON,0); f_val = stmt(); r_val = stmt_lstT(f_val);
            return r_val;
        case END:
            fprintf(trace, "%s\n","stmt_lstT end break no tok consumption" );
            r_val = i_val;
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s, instead received %s\n",";","end",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmt_lstT: trying to sync on %s\n", tok.word);
                printf("Stuck here with tok: %d\n",tok.tkn);
            }
            fprintf(trace, "stmt_lstT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int stmt_lst(){
    int r_val;
    int f_val;
    fprintf(trace, "%s %s\n","stmt_lst",tok.word );
    switch (tok.tkn) {
        case ID: case BEGIN: case IF: case WHILE:
            f_val = stmt(); r_val = stmt_lstT(f_val);
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead received %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "stmt_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "stmt_lst: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int opt_stmt(){
    int r_val;
    fprintf(trace, "%s %s\n","opt_stmt",tok.word );
    switch (tok.tkn) {
        case ID: case BEGIN: case IF: case WHILE:
            r_val = stmt_lst();
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s, instead received %s\n","Identifier","begin","if","while",tok.word);
            while (strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "opt_stmt: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "opt_stmt: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int cmpd_stmtT(){
    int r_val;
    fprintf(trace, "%s %s\n","cmpd_stmtT" ,tok.word);
    switch (tok.tkn) {
        case END:
            tok_match(END,0);
            r_val = TYPEVOID;
            return r_val;
        case BEGIN: case IF: case WHILE: case ID:
            r_val = opt_stmt(); tok_match(END,0);
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s,%s,%s,%s,%s,%s instead received %s\n","Identifier","end","begin","if","while","do",tok.word);
            while (strcmp(tok.word,".") != 0 && strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "cmpd_stmtT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "cmpd_stmtT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

int cmpd_stmt(){
    int r_val;
    fprintf(trace, "%s %s\n","cmpd_stmt",tok.word );
    switch (tok.tkn) {
        case BEGIN:
            tok_match(BEGIN,0); r_val = cmpd_stmtT();
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, instead received %s\n","begin",tok.word);
            while (strcmp(tok.word,".") != 0 && strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,";") != 0 && strcmp(tok.word,"end") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "cmpd_stmt: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "cmpd_stmt: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}
//THIS IS A BLUE NODE FABRICATOR
void param_lstT(){
    int f_val;
    int gn=1;
    int scope_in =0;
    char name[15];
    fprintf(trace, "%s %s\n","param_lstT",tok.word);
    switch (tok.tkn) {
        case SEMICOLON:
            tok_match(SEMICOLON,0);
            if(scope_flag == 1){
                scope_in = 1;//go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            tok_match(COLON,0); f_val = type();
            if(f_val != TYPEERR){
                gn = check_add_node(st,name,f_val,YELLOW,scope_in);
            }
            if(gn != 0){
                fprintf(list, "SEMERR: variable %s already declared in scope\n",name );
                if(scope_in == 1){
                    scope_flag = 1;
                }
            }
            if(f_val != TYPEERR){
                fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(f_val),near_parent(st,tok_ret.word));
            }
            param_lstT();
            break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n",";",")",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "param_lstT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "param_lstT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}
//THIS IS A BLUE NODE FABRICATOR
void param_lst(){
    int f_val;
    int scope_in = 0;
    int gn=1;
    char name[15];
    fprintf(trace, "%s %s\n","param_lst",tok.word );
    switch (tok.tkn) {
        case ID:
            if(scope_flag == 1){
                scope_in = 1;//go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            tok_match(COLON,0); f_val = type();
            if(f_val != TYPEERR){
                gn = check_add_node(st,name,f_val,YELLOW,scope_in);
            }
            if(gn != 0){
                fprintf(list, "SEMERR: variable %s already declared in scope\n",name );
                if(scope_in == 1){
                    scope_flag = 1;
                }
            }
            if(f_val != TYPEERR){
                fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(f_val),near_parent(st,tok_ret.word));
            }
            param_lstT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting Identifier, instead received %s\n",tok.word);
            while (strcmp(tok.word,")") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "param_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "param_lst: sync on token %s\n", tok.word);
            //return TYPEERR;
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
            fprintf(list,"SYNERR: tok mismatch expecting %s, instead received %s\n","(",tok.word);
            while (strcmp(tok.word,":") != 0) {
                tok = get_next_token(file,list,token);
                fprintf(trace, "args: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "args: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}


//GREEN NODE FABRICATOR
int sub_headT(){
    fprintf(trace, "%s %s\n","sub_headT",tok.word);
    int r_val;
    int f_val;
    switch (tok.tkn) {
        case COLON:
            tok_match(COLON,0); f_val = std_type(); tok_match(SEMICOLON,0);
            if(f_val == TYPEINT ){
                r_val = TYPEINT;
            }
            else if(f_val == TYPEREAL){
                r_val = TYPEINT;
            }
            else{
                fprintf(list,"SEMERR: Invalid type for function, valid types are %s or %s\n",tokcode2str(TYPEINT),tokcode2str(TYPEREAL));
                r_val = TYPEERR;
            }
            return r_val;
        case PAREN:
            if (tok.attr.val == PAREN_OPEN){
                args(); tok_match(COLON,0); f_val = std_type(); tok_match(SEMICOLON,0);
                if(f_val == TYPEINT ){
                    r_val = TYPEINT;
                }
                else if(f_val == TYPEREAL){
                    r_val = TYPEREAL;
                }
                else{
                    fprintf(list,"SEMERR: Invalid type for function, valid types are %s or %s\n",tokcode2str(TYPEINT),tokcode2str(TYPEREAL));
                    r_val = TYPEERR;
                }
                return r_val;
            }
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n",":","(",tok.word);
            while(strcmp(tok.word,"var") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_headT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_headT: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}
//GREEN NODE FABRICATOR
int sub_head(){
    fprintf(trace, "%s %s\n","sub_head" ,tok.word);
    int f_val; //function return type
    int r_val;
    int scope_in = 0;
    char func_name[15];
    switch (tok.tkn) {
        case FUNCTION:
            if(scope_flag == 1){
                scope_in = 1;//go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            tok_match(FUNCTION,0); tok_ret = tok_match(ID,0);
            strcpy(func_name,tok_ret.word);
            check_add_node(st,tok_ret.word,TYPEVOID,GREEN,scope_in);
            fprintf(nodes,"%-15s %-15s %-15s\n",func_name,"TBD",near_parent(st,tok_ret.word));
            scope_flag=1;
            f_val = sub_headT();
            set_id_type(st,func_name,f_val);
            if(f_val == TYPEVOID){
                r_val = TYPEVOID;
            }
            else{
                r_val = TYPEERR;
            }
            return r_val;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, instead received %s\n","function",tok.word);
            while(strcmp(tok.word,"var") != 0 && strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_head: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_head: sync on token %s\n", tok.word);
            return TYPEERR;
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
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decTT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decTT: sync on token %s\n", tok.word);
            //return TYPEERR;
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
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, instead received %s\n","var","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

void sub_dec(){
    fprintf(trace, "%s %s\n","sub_dec"  ,tok.word);
    switch (tok.tkn) {
        case FUNCTION:
            sub_head(); sub_decT();//upscope here!!!!
            if(scope_level > 1 && scope_flag == 0){
                up_scope(st);
            }
            scope_flag = 0;
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead received %s\n","function",tok.word);
            while(strcmp(tok.word,"begin") != 0 && strcmp(tok.word,"function") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_dec: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_dec: sync on token %s\n", tok.word);
            //return TYPEERR;
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
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, instead received %s\n","function","begin",tok.word);
            while(strcmp(tok.word,"begin") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decsT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decsT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

void sub_decs(){
    fprintf(trace, "%s %s\n","sub_decs" ,tok.word );
    switch (tok.tkn) {
        case FUNCTION:
            sub_dec(); tok_match(SEMICOLON,0); sub_decsT();
            break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead received %s\n","function",tok.word);
            while(strcmp(tok.word,"begin") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "sub_decs: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "sub_decs: sync on token %s\n", tok.word);
            //return TYPEERR;
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
            fprintf(list,"SYNERR: tok mismatch expecting ; instead received %s\n",tok.word);
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
    int err_flag=0;
    switch (tok.tkn) {
        case INTEGER: case REAL:
            s = std_type();
            return s;
        case ARRAY:
            //for type checking in p3 the two num values need to be of type integer
            tok_match(ARRAY,0); tok_match(BRACK,BRACK_OPEN);
            arr_check = tok_match(NUM,0).type;
            if(arr_check != TYPEINT){
                err_flag=1;
                fprintf(list, "SEMERR: non integer number used for array creation\n" );
            }
            tok_match(DOTDOT,0);
            arr_check = tok_match(NUM,0).type;
            if(arr_check != TYPEINT){
                err_flag=1;
                fprintf(list, "SEMERR: non integer number used for array creation\n" );
            }
            tok_match(BRACK,BRACK_CLOSE);
            tok_match(OF,0); s = std_type();
            if (s != TYPEERR){
                s = s + 3;//type is transformed into respective array type
            }
            if(err_flag){
                s=TYPEERR;
            }
            return s;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s, %s, %s, instead received %s\n","integer","real","array",tok.word);
            while(strcmp(tok.word,";") != 0){
                tok = get_next_token(file,list,token);
                fprintf(trace, "type: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "type: sync on token %s\n", tok.word);
            return TYPEERR;
    }
}

void decsT(){
    int f_val;
    fprintf(trace, "%s %s\n","decsT"  ,tok.word);
    int node_color = BLUE;
    char name[15];
    int gn=1;
    int scope_in = 0;
    switch(tok.tkn){
        case VAR://grab a variable declaration from here
            if(scope_flag == 1){
                scope_in = 1;//go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            tok_match(VAR, 0); tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            tok_match(COLON, 0); f_val = type();
            printf("TEST: f_val: %d\n",f_val );
            if(f_val != TYPEERR){
                gn = check_add_node(st,name,f_val,BLUE,0);//create blue node
            }
            if(gn < 0){
                fprintf(list, "SEMERR: variable %s already declared in scope\n",name );
                if(scope_in == 1){
                    scope_flag = 1;
                }
            }
            if(f_val != TYPEERR){
                fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(f_val),near_parent(st,tok_ret.word));
            }
            tok_match(SEMICOLON,0);
            decsT();
        break;
        case FUNCTION: case BEGIN:
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s %s %s instead received %s\n","var","function","begin",tok.word);
            while(strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "decsT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "decsT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

void decs(){
    int f_val;
    int scope_in = 0;
    char name[15];
    int gn=1;
    fprintf(trace, "%s %s\n","decs" ,tok.word);
    switch(tok.tkn){
        case VAR:
            //function parameters present
            if(scope_flag == 1){
                scope_in = 1;//go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            //create a blue node here and attach it to what the last function (green node) to put it into scope
            tok_match(VAR, 0); tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            tok_match(COLON, 0); f_val = type();
            //down scope value must be 1 on the first declaration
            if(f_val != TYPEERR){
                gn = check_add_node(st,name,f_val,BLUE,scope_in);//create blue node
            }
            if(gn != 0){
                fprintf(list, "SEMERR: variable %s already declared in scope\n",name );
                if(scope_in == 1){
                    scope_flag = 1;
                }
            }
            if(f_val != TYPEERR){
                fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(f_val),near_parent(st,tok_ret.word));
            }
            tok_match(SEMICOLON,0);
            decsT();
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead received %s\n","var",tok.word);
            while(strcmp(tok.word,"function") != 0 && strcmp(tok.word,"begin") != 0){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "decs: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "decs: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

void id_lstT(){
    fprintf(trace, "%s %s\n","id_lstT"  ,tok.word);
    char syncSet[] = {')'};
    char name[15];
    int scope_in =0;
    switch(tok.tkn){
        case COMMA:
            if(scope_flag == 1){
                scope_in = 1;//does not go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            tok_match(COMMA,0); tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            if(tok_ret.type != TYPEERR){
                check_add_node(st,tok_ret.word,TYPEVOID,BLUE,scope_in);
                fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(TYPEVOID),near_parent(st,tok_ret.word));
            }
            else{
                if(scope_in==1){
                    scope_flag = 1;
                }
            }
            id_lstT();
        break;
        case PAREN:
            if(tok.attr.val == PAREN_CLOSE){
                break;
            }
        default:
            //print error message
            fprintf(list,"SYNERR: tok mismatch expecting %s instead received %s\n",syncSet,tok.word);
            while(strcmp(tok.word,")") != 0){//paren close match
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "id_lstT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "id_lstT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}


void id_lst(){
    fprintf(trace, "%s %s\n","id_lst"  ,tok.word);
    char syncSet[] = {')'};
    int scope_in = 0;
    char name[15];
    switch(tok.tkn){
        case ID:
            if(scope_flag == 1){
                scope_in = 1;//does not go down a level in scope
                scope_level++;
                addr = 0;
                scope_flag = 0;
            }
            tok_ret = tok_match(ID,0); //need to go down scope otherwise
            strcpy(name,tok_ret.word);
            if(tok_ret.type != TYPEERR){
                check_add_node(st,tok_ret.word,TYPEVOID,BLUE,scope_in);
                fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(TYPEVOID),near_parent(st,tok_ret.word));
            }
            else{
                if(scope_in == 1){
                    scope_flag =1;
                }
            }
            id_lstT();
        break;
        default:
            fprintf(list,"SYNERR: tok mismatch expecting %s instead received %s\n","Identifier",tok.word);
            while(strcmp(tok.word,")") != 0){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "id_lst: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "id_lst: sync on token %s\n", tok.word);
            //return TYPEERR;
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
            fprintf(list, "SYNERR: tok mismatch expecting %s instead received %s\n","function begin",tok.word);
            while(tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "prgmTT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "prgmTT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

void prgmT(){
    fprintf(trace, "%s %s\n","prgmT" ,tok.word);
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
            fprintf(list, "SYNERR: tok mismatch expecting %s instead received %s\n","var, function, begin",tok.word);
            while (tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "prgmT: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "prgmT: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}

void prgm(){
    fprintf(trace, "%s %s\n","prgm" ,tok.word);
    char name[15];
    switch(tok.tkn){
        case PROGRAM:
            scope_flag = 1;
            tok_match(PROGRAM,0); tok_ret = tok_match(ID,0);
            strcpy(name,tok_ret.word);
            tok_match(PAREN, PAREN_OPEN);
            check_add_node(st,name,TYPEVOID,GREEN,0);//create program node here
            fprintf(nodes,"%-15s %-15s %-15s\n",name,tokcode2str(TYPEVOID),near_parent(st,tok_ret.word));
            id_lst(); tok_match(PAREN,PAREN_CLOSE); tok_match(SEMICOLON,0);
            prgmT();
        break;
        default: //this will be the otherwise statement
            fprintf(list, "SYNERR: tok mismatch expecting %s instead received %s\n","program",tok.word);
            while (tok.tkn != EOF){
                //print error message
                tok = get_next_token(file,list,token);
                fprintf(trace, "prgm: trying to sync on %s\n", tok.word);
            }
            fprintf(trace, "prgm: sync on token %s\n", tok.word);
            //return TYPEERR;
    }
}



void parse(FILE *f,FILE *l, FILE *t, FILE *tr,FILE *n,FILE *a){
    file = f;
    list = l;
    token = t;
    trace = tr;
    nodes = n;
    ad = a;
    fprintf(nodes, "%-15s %-15s %-15s\n","nodes:","type:","NP:" );
    tok = get_next_token(file,list,token);
    prgm();
    tok_match(EOF,0);
    print_stack(st);
}


#endif
