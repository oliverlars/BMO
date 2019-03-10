#include <stdio.h>
#include <memory.h>
#include <malloc.h>
extern "C"{
#include "pdfgen.h"
}
#include "bmo.h"
#include "bmo_lexer.cc"
#include "bmo_parser.h"



int main(){
    
    bool parsing = true;
    char test[] = ".font 50\
.margins 4 \
.title right 20\"Hello World.\" \
.title left 10\"Doggo the world\"\
.para \
Dog";
    Lexer l;
    l.pos = test;
    
    pdf_info info = {
        .creator = "",
        .title = "My document",
        .author = "",
        .subject = "comp",
        .date = "Today"
    };
    
    l.doc.pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    pdf_set_font(l.doc.pdf, "Times-Roman");
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