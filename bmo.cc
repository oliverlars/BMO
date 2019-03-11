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
    l.doc.font_name = "Times-Roman";
    printf("height %d\n", l.state.line_pos);
    pdf_set_font(l.doc.pdf,l.doc.font_name);
    pdf_append_page(l.doc.pdf);
    while(parsing){
        Token token = l.get_token();
        switch(token.type){
            case TOKEN_END:{
                parsing = false;
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
                printstr(token.str);
            }break;
        }
    }
    
    pdf_save(l.doc.pdf, "output.pdf");
    pdf_destroy(l.doc.pdf);
    return 0;
}