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
        printf("Expectected %s, got %s \'%.*s\' on line %d", 
               tokentypes[type], tokentypes[token.type],
               token.str.len, token.str.text, l->line);
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


void point_to_error(Lexer* l){
    printf("Line: %.*s", l->pos - l->prev_line, l->prev_line);
}


int draw_table(Lexer* l, int xdivs ,int ydivs,int lwidth){
    int deltay = 15;
    int height = deltay*ydivs;
    int bottom = l->state.line_pos - height;
    int start = l->state.line_pos;
    int deltax = 100;
    int width  = deltax*xdivs;
    
    Token arg;
    int height_to_shift = 0;
    for(int y = l->state.line_pos - deltay; y > bottom-deltay; y -= deltay){
        for(int x = l->doc.width/2 - width/2; x < l->doc.width/2 + width/2; x += deltax){
            arg = l->get_token();
            height_to_shift +=pdf_add_text_wrap(l->doc.pdf, NULL,
                                                arg.str.text,
                                                arg.str.len,
                                                l->doc.font_size,
                                                x,
                                                y +(deltay - l->doc.font_size),
                                                PDF_BLACK,
                                                deltax,
                                                PDF_ALIGN_CENTER);
        }
    }
    
    pdf_add_rectangle(l->doc.pdf, NULL,
                      l->doc.width/2 - width/2,
                      l->state.line_pos-height,
                      width,
                      height,
                      lwidth,
                      PDF_BLACK);
    for(int y = l->state.line_pos - deltay; y > bottom; y -= deltay){
        pdf_add_line(l->doc.pdf, NULL,
                     l->doc.width/2 - width/2,
                     y,
                     l->doc.width/2 + width/2,
                     y,
                     lwidth,
                     PDF_BLACK);
    }
    
    
    for(int x = l->doc.width/2 - width/2; x < l->doc.width/2 + width/2; x += deltax){
        pdf_add_line(l->doc.pdf, NULL,
                     x,
                     bottom,
                     x,
                     l->state.line_pos,
                     lwidth,
                     PDF_BLACK);
    }
    
    return height + height_to_shift;
}

void parse_identifier(Lexer* l, Token token){
    if(match_token(token, "bold")){
        pdf_set_font(l->doc.pdf, "Helvetica-Bold");
    }else if(match_token(token, "title")){
    }else if(match_token(token, "table")){
        
        int xdivs = str_to_int(require_token(l, TOKEN_NUMBER).str);
        int ydivs  = str_to_int(require_token(l, TOKEN_NUMBER).str);
        int lwidth = 1;
        l->state.line_pos -= lwidth*3;
        l->state.line_pos -= draw_table(l,xdivs, ydivs, lwidth);
        
    }else if(match_token(token, "right")){
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
    }else if(match_token(token, "shift")){
        l->doc.margin_size += 100;
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
        if(token.type == TOKEN_ID){
            l->error = true;
            printf("Unexpected token on line %d\n", l->line);
            point_to_error(l);
        }else{
            l->doc.font_size = str_to_int(token.str);
        }
    }
}