#include "bmo_lexer.h"


inline bool is_newline(char c){
    return c == '\n' || c== '\r';
}

inline bool is_whitespace(char c){
    return  c == ' ' || c == '\t' || c == '\v' ||c =='\f' || is_newline(c);
}

inline bool is_lower_alpha(char c){
    return c >= 'a' && c <= 'z';
}

inline bool is_upper_alpha(char c){
    return c >= 'A' && c <= 'Z';
}

inline bool is_alpha(char c){
    return is_upper_alpha(c) || is_lower_alpha(c);	
}

inline bool is_digit(char c){
    return c <= '9' && c >= '0';
}

void Lexer::eat_whitespace(){
    while(is_whitespace(*pos)){
        advance();
    }
}


void Lexer::advance(){
    pos++;
}


Token Lexer::get_token(){
    eat_whitespace();
    
    char c = pos[0];
    Token token = {};
    token.str.text = pos;
    token.str.len = 1;
    advance();
    
    switch(c){
        case '\0':{token.type = TOKEN_END;}break;
        case '\n':{
            prev_line = pos;
            line++;
        }
        case '.':{token.type = TOKEN_FULLSTOP;}break;
        default:{
            token.type = TOKEN_ID;
            if(is_alpha(c)){
                while(is_alpha(pos[0])){advance();}
                token.str.len = pos - token.str.text;
            }
            else if(is_digit(c)){
                token.type = TOKEN_NUMBER;
                while(is_digit(pos[0])){advance();}
                token.str.len = pos - token.str.text;
            }
            else if(c == '"'){
                token.type = TOKEN_STRING;
                advance();
                token.str.text++;
                while(pos[0] != '"'){
                    advance();
                    if(is_whitespace(pos[0])) line++;
                }
                advance();
                token.str.len = pos - token.str.text-1;
            }
        }
    }
    return token;
}

Token Lexer::peek_token(){
    char* temp_pos = pos;
    Token result = get_token();
    pos = temp_pos;
    return result;
}