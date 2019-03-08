#include <stdio.h>
extern "C"{
#include "pdfgen.h"
}
#include "bmo.h"
#include "bmo_parser.cc"


int main(){
    printf("Hello, world!");
    pdf_info info = {
        .creator = "Test",
        .title = "My document",
        .author = "oliver",
        .subject = "comp",
        .date = "Today"
    };
    
    pdf_doc* pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    pdf_set_font(pdf, "Times-Roman");
    pdf_append_page(pdf);
    pdf_add_text(pdf, NULL, "Oliver Cruickshank", 24, 0, PDF_A4_HEIGHT-24, PDF_BLACK);
    pdf_add_line(pdf, NULL, 50, 24, 150, 24, 3, PDF_BLACK);
    pdf_save(pdf, "output.pdf");
    pdf_destroy(pdf);
    return 0;
}