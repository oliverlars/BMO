bool match_token(Token token, char* str){
    char* pos = str;
    
    for(int i = 0; i < token.str.len; i++, pos++){
        if((pos[0] == 0) || (token.str.text[i] != pos[0])){
            return false;
        }
    }
    
    return pos[0] == 0;
}


int str_to_int(String str){
    int value = 0;
    for(int i = 0; i < str.len; i++){
        value = value*10 + str.text[i] - '0';
    }
    return value;
}

Token require_token(Lexer* l, Token_Type type){
    
    Token token = l->get_token();
    if(token.type != type){
        printstr(token.str);
        l->error = true;
    }
    return token;
}

#define is_arg(x) ((x).type != TOKEN_FULLSTOP || (x).type != TOKEN_STRING)

Token get_arg(Lexer* l){
    Token arg = l->peek_token();
    if(is_arg(arg)){
        l->get_token();
        return arg;
    }
}


void string_to_cstr(String str){
    for(int i = 0; i < str.len; i ++){
        temp_str.push(str.text[i]);
    }
    temp_str.push(0);
}


void parse_identifier(Lexer* l, Token token){
    if(match_token(token, "title")){
        Token arg = l->peek_token();
        if(is_arg(arg)){
            l->get_token();
            if(match_token(arg, "right")){
                arg = l->peek_token();
                if(is_arg(arg)){
                    l->doc.font_size = str_to_int(arg.str);
                    l->get_token();
                }
                Token block = l->get_token();
                string_to_cstr(block.str);
                pdf_add_text_wrap(l->doc.pdf, NULL, temp_str.data,
                                  l->doc.font_size,
                                  l->doc.margin_size,
                                  200,
                                  PDF_BLACK,
                                  120,
                                  PDF_ALIGN_RIGHT
                                  );
                temp_str.reset();
            }else if(match_token(arg, "left")){
                printf("yaboy");
                arg = l->peek_token();
                if(is_arg(arg)){
                    l->doc.font_size = str_to_int(arg.str);
                    l->get_token();
                }
                Token block = l->get_token();
                string_to_cstr(block.str);
                pdf_add_text_wrap(l->doc.pdf, NULL, temp_str.data,
                                  l->doc.font_size,
                                  l->doc.margin_size,
                                  200,
                                  PDF_BLACK,
                                  120,
                                  PDF_ALIGN_LEFT
                                  );
                temp_str.reset();
            }else if(match_token(arg, "centre")){
            }
        }
    }else if(match_token(token, "para")){
        l->get_token();
    }else if(match_token(token, "margins")){
        Token number = require_token(l, TOKEN_NUMBER);
        l->doc.margin_size = str_to_int(number.str);
    }else if(match_token(token, "font")){
        Token number = require_token(l, TOKEN_NUMBER);
        l->doc.font_size = str_to_int(number.str);
    }
}