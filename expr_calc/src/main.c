#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
    TOK_NUM,
    TOK_ADD, TOK_MUL, TOK_SUB, TOK_DIV,
    TOK_LPAREN, TOK_RPAREN,
    TOK_EOF,
}TokenKind;

typedef struct{
    TokenKind kind;
    int num;
}Token;

typedef struct{
    Token *data;
    size_t len;
    size_t capacity;
}TokenList;

TokenList token_list_alloc(size_t capacity){
    assert(capacity > 0);

    TokenList list = (TokenList){
        .data = NULL,
        .len = 0,
        .capacity = 0,
    };

    list.data = malloc(sizeof(Token) * capacity);

    if(list.data == NULL) return list;

    list.capacity = capacity;
    return list;
}

bool token_list_append(TokenList *in, Token token){
   if(in == NULL) return false;

   if(in->len >= in->capacity){
       Token *tmp = NULL;
       size_t capacity_new = in->capacity ? in->capacity * 2 : 32;
       tmp = realloc(in->data, sizeof(Token) * capacity_new);

       if(tmp == NULL) return false;

       in->data = tmp;
       in->capacity = capacity_new;
   }

   size_t len = in->len;
   in->data[len++] = token;

   return true;
}

void token_list_free(TokenList *list){
    if(list == NULL) return;

    free(list->data);

    list->data = NULL;
    list->len = 0;
    list->capacity = 0;
}

char* lexer_num(const char *in, Token *out){
    if(in == NULL || out == NULL) return (char*)in;

    assert(isdigit(in[0]));

    out->num = 0;
    out->kind = TOK_NUM;

    for(int i = 0; isdigit(in[i]); ++i){
        int digit = in[i] - '0';
        out->num = 10 * out->num + digit;
    }
}

bool lexer(const char* in, TokenList *out){
    if(in == NULL || out == NULL) return false;

    
}

int main(void) {
    return 0;
}
