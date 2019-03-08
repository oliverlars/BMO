enum KW_Type: int{
    KW_FONT,
    KW_TITLE,
    KW_LINE,
    KW_PARA,
};


enum Token_Type: int{
    TOKEN_FULLSTOP,
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_END
};

struct Token{
    String str;
    Token_Type type;
};



struct Lexer{
    char* pos = nullptr;
    bool error;
    
    void advance();
    Token get_token();
    void eat_whitespace();
    
};

