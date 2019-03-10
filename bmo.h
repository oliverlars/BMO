struct String{
    char* text;
    int len;
};

struct Text{
    char* text;
    int len;
    String font;
    
    bool is_para;
};

struct State{
    int line_pos;
};

struct Doc {
    pdf_doc* pdf;
    int margin_size;
    int font_size;
    int width;
    int height;
};


#define printstr(x) printf("%.*s", x.len, x.text)



template <typename T>
struct Buffer{
    T* data = nullptr;
    int used = 0;
    int size = 8;
    Buffer(){
        data = (T*)malloc(sizeof(T)*size);
    }
    void push(T item){
        if(size < used + 1){
            int old_size = size;
            size = size < 8 ? 8 : size*2;
            data = (T*)realloc(data, size);
        }
        data[used] = item;
        used++;
    }
    
    void reset(){
        used = 0;
    }
    
    void free(){
        free(data);
    }
};

Buffer<char> temp_str;