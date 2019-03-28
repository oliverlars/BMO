enum KW_Type: int{
    KW_FONT,
    KW_TITLE,
    KW_LINE,
    KW_PARA,
};


enum Token_Type: int{
    TOKEN_INVALID,
    TOKEN_FULLSTOP,
    TOKEN_BACKSLASH,
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_END
};

char* tokentypes[] = {
    "Invalid",
    "Fullstop",
    "Backslash",
    "Identifier",
    "Number",
    "String",
    "End",
};

struct Token{
    String str;
    Token_Type type;
};



struct Lexer{
    char* pos = nullptr;
    bool error = false;
    
    void advance();
    Token get_token();
    Token peek_token();
    void eat_whitespace();
    
    State state;
    Doc doc;
    int line = 1;
    char* prev_line;
    char* end_line;
};


