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



int get_new_linepos(Doc doc, char* text){
    int width = pdf_get_font_text_width(doc.pdf, doc.font_name, text, doc.font_size);
    char* pos = text;
    int offset = 1;
    int wrap_increment = 0;
    while(pos && *pos){
        if(is_newline(pos[0])){
            offset += 1;
        }
        pos++;
    }
    int height = (width / (doc.width - 2*doc.margin_size));
    if(offset){
        return (height+ offset)*doc.font_size;
    }else{
        return (height + 1)*doc.font_size;
    }
}

int format_and_render(Doc doc, State state, String str){
    int new_height = pdf_add_text_wrap(doc.pdf, NULL, str.text, str.len,
                                       doc.font_size,
                                       doc.margin_size,
                                       state.line_pos - doc.font_size,
                                       PDF_BLACK,
                                       doc.width - 2*doc.margin_size,
                                       state.align
                                       );
    return new_height;
}

void parse_identifier(Lexer* l, Token token){
    if(match_token(token, "bold")){
        pdf_set_font(l->doc.pdf, "Times-Bold");
    }else if(match_token(token, "title")){
    }
    else if(match_token(token, "right")){
        l->state.align = PDF_ALIGN_RIGHT;
    }else if (match_token(token, "left")){
        l->state.align = PDF_ALIGN_LEFT;
    }else if (match_token(token, "centre")){
        l->state.align = PDF_ALIGN_CENTER;
    }else if(match_token(token, "para")){
    }else if(match_token(token, "margins")){
    }else if(match_token(token, "font")){
    }
    else if(match_token(token, "ghost")){
        l->state.is_ghost = true;
    }
    else if(match_token(token, "line")){
        int lwidth = 1;
        l->state.line_pos -= lwidth*3;
        pdf_add_line(l->doc.pdf, NULL, 
                     l->doc.margin_size, 
                     l->state.line_pos,
                     l->doc.width - l->doc.margin_size, 
                     l->state.line_pos,
                     lwidth, PDF_BLACK);
        l->state.line_pos -= lwidth*3;
    }
    else{
        l->doc.font_size = str_to_int(token.str);
    }
}