// string.cpp

struct small_string {
    array<23, char> string;
    char empty;
}

struct heap_string {
    char *data;
    size_t size;
    size_t capacity;

}

union string {

    
}