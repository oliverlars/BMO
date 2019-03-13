#include <stdio.h>
#include <memory.h>
#include <malloc.h>
extern "C"{
#include "pdfgen.h"
}
#include "bmo.h"
#include "bmo_lexer.cc"
#include "bmo_parser.h"

static char* open_source(char* filename){
    char* result = 0;
    FILE* file_pointer = fopen(filename, "r");
    
    if(file_pointer){
        fseek(file_pointer, 0, SEEK_END);
        size_t size = ftell(file_pointer);
        fseek(file_pointer, 0, SEEK_SET);
        
        result = (char*)calloc(size+2, sizeof(char));
        result[size] = 0;
        result[size-1] = '\n';
        fread(result, size, 1, file_pointer);
        fclose(file_pointer);
    }else {
        printf("Unable to open source");
    }
    return result;
}


int main(int argc, char** args){
    
    bool parsing = true;
    char test[] = ".font 14\
.margins 50 \
.title right 20\"Hello World.\" \
.title right 30\"Bigger hello world\"\
.line\
.para 12 \
 \"Lorem ipsum dolor sit amet, \b consectetur adipiscing elit, sed do eiusmod tempor \b incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum\" \
.title right 50 \"THIS HELLO, WORLD IS HUGE\"";
    char* source = open_source(args[1]);
    Lexer l;
    l.pos = source;
    
    pdf_info info = {
        .creator = "",
        .title = "My document",
        .author = "",
        .subject = "comp",
        .date = "Today"
    };
    l.doc.pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    l.doc.width = PDF_A4_WIDTH;
    l.doc.height = PDF_A4_HEIGHT;
    l.doc.aspect = PDF_A4_WIDTH/PDF_A4_HEIGHT;
    l.doc.margin_size = 50;
    l.state.line_pos = PDF_A4_HEIGHT - l.doc.margin_size;
    l.doc.font_name = "Helvetica";
    l.prev_line = source;
    pdf_set_font(l.doc.pdf,l.doc.font_name);
    pdf_append_page(l.doc.pdf);
    
    int font_size = 12;
    int is_bold = false;
    char* font_name = "Helvetica";
    int align = PDF_ALIGN_LEFT;
    int is_ghost = false;
    
    while(!l.error){
        Token token = l.get_token();
        switch(token.type){
            case TOKEN_END:{
                l.error = true;
            }break;
            case TOKEN_FULLSTOP:{
                token = l.get_token();
                parse_identifier(&l, token);
            }break;
            case TOKEN_BACKSLASH:{
            }break;
            case TOKEN_ID:{
            }break;
            case TOKEN_STRING:{
                int height;
                if(l.state.is_ghost){
                    l.state.is_ghost = false;
                    height = format_and_render(l.doc, l.state, token.str);
                }else{
                    height = format_and_render(l.doc, l.state, token.str);
                    l.state.line_pos -= height;
                }
                pdf_set_font(l.doc.pdf, "Helvetica");
                l.doc.font_size = font_size;
                l.doc.is_bold = is_bold;
                l.state.align = align;
                l.doc.margin_size = 50;
            }break;
        }
    }
    
    pdf_save(l.doc.pdf, "output.pdf");
    pdf_destroy(l.doc.pdf);
    return 0;
}