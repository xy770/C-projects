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

typedef enum{
    NODE_ADD, NODE_MUL, NODE_SUB, NODE_DIV,
    NODE_NUM,
    NODE_UNKNOWN,
}NodeKind;

typedef struct Node Node;
struct Node{
    NodeKind kind;
    int num;
    Node *left;
    Node *right;
};

typedef struct{
    const TokenList *list;
    size_t from;
    size_t to;
    size_t offset;
}Parser;

// expr -> term ('+' term | '-' term)*
// term -> factor ('*' factor | '/' factor)*
// factor -> NUM | '(' expr ')'
#define TOKEN_LIST_END(x) ((x).len - 1)

bool parse_expr(Node *dest, Parser parser);
bool parse_term(Node *dest, Parser parser);
bool parse_factor(Node *dest, Parser parser);

TokenList token_list_alloc(){
    TokenList list = (TokenList){
        .data = NULL,
        .len = 0,
        .capacity = 0,
    };

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

   in->data[in->len++] = token;

   return true;
}

void token_list_free(TokenList *list){
    if(list == NULL) return;

    free(list->data);

    list->data = NULL;
    list->len = 0;
    list->capacity = 0;
}

Parser parser_new(const TokenList *list, size_t from, size_t to){
    if(list == NULL){ from = 0; to = 0;}
    if(list->len == 0){ from = 0; to = 0;}

    size_t to_max = list->len - 1;
    to = to < to_max ? to : to_max;

    return (Parser){
        .list = list,
        .from = from,
        .to = to,
        .offset = 0,
    };
}

static inline void parser_reset(Parser *parser){
    parser->offset = 0;
}

bool is_parser_valid(Parser *parser){
    const size_t from = parser->from;
    const size_t to = parser->to;

    if(to >= parser->list->len || from > to || !parser->list->len) return false;

    return true;
}

size_t parser_get_list_count(Parser *parser){
    if(!is_parser_valid(parser)) return 0;

    return parser->to - parser->from + 1;
}

bool parser_at(Token *dest, Parser *parser, size_t idx){
    if(dest == NULL || parser == NULL || parser->list == NULL) return false;

    const size_t from = parser->from;
    const size_t to = parser->to;

    if(!is_parser_valid(parser)) return false;
    if(idx + from > to) return false;

    *dest = parser->list->data[from + idx];

    return true;
}

bool parser_next(Token *dest, Parser *parser){
    if(dest == NULL || parser == NULL || parser->list == NULL) return false;

    const size_t from = parser->from;
    const size_t to = parser->to;
    size_t *offset = &parser->offset;

    if(!is_parser_valid(parser)) return false;

    if(*offset + from > to) return false;

    *dest = parser->list->data[from + (*offset)++];
    return true;
}

static inline Node node_new(){
    return (Node){
        .kind = NODE_UNKNOWN,
        .num = 0,
        .left = NULL,
        .right = NULL,
    };
}

char* lexer_num(char *in, Token *out){
    if(in == NULL || out == NULL) return in;

    assert(isdigit(in[0]));

    out->num = 0;
    out->kind = TOK_NUM;

    int i = 0;
    for(i = 0; isdigit(in[i]); ++i){
        int digit = in[i] - '0';
        out->num = 10 * out->num + digit;
    }

    return in + i;
}

static inline bool is_valid_single(char c){
    if(c == '+') return true;
    if(c == '-') return true;
    if(c == '*') return true;
    if(c == '/') return true;
    if(c == '(') return true;
    if(c == ')') return true;

    return false;
}

char* lexer_single(char *in, Token *out){
    if(in == NULL || out == NULL) return in;

    if(in[0] == '+'){
        out->kind = TOK_ADD;
    }else if(in[0] == '-'){
        out->kind = TOK_SUB; 
    }else if(in[0] == '*'){
        out->kind = TOK_MUL; 
    }else if(in[0] == '/'){
        out->kind = TOK_DIV; 
    }else if(in[0] == '('){
        out->kind = TOK_LPAREN; 
    }else if(in[0] == ')'){
        out->kind = TOK_RPAREN; 
    }

    return in + 1;
}

bool lexer(char* in, TokenList *out){
    if(in == NULL || out == NULL) return false;

    char *p = in;
    while (*p != '\0') {
        Token tok = {0};
        if(isdigit(*p)){
            p = lexer_num(p, &tok);
            token_list_append(out, tok);
            continue;
        }else if(*p == ' '){
            ++p;
            continue;
        }else if(is_valid_single(*p)){
            p = lexer_single(p, &tok); 
            token_list_append(out, tok);
            continue;
        }
    }

    return true;
}

bool parse_factor(Node *dest, Parser parser){
    if(dest == NULL) return false;

    Token token;
    size_t count = parser_get_list_count(&parser);

    if(count == 1){
        if(!parser_next(&token, &parser)) return false;
        if(token.kind == TOK_NUM){
            dest->kind = NODE_NUM;
            dest->num = token.num;
            dest->left = NULL;
            dest->right = NULL;
            return true;
        }else {
            return false;
        }
    }else if(count > 1){
        parser_at(&token, &parser, 0);
        if(token.kind != TOK_LPAREN) return false;
        parser_at(&token, &parser, parser.to);
        if(token.kind != TOK_RPAREN) return false;

        parser_reset(&parser);
        ++parser.from;
        --parser.to;

        bool is_expr = parse_expr(dest, parser);
        
        if(is_expr) return true;
    }

    return false;
}

bool parse_term(Node *dest, Parser parser){
    if(dest == NULL) return false;

    Token token;
    Node *left = malloc(sizeof(left[0]));
    Node *right = malloc(sizeof(right[0]));

    while (parser_next(&token, &parser)) {
        if(token.kind == TOK_MUL || token.kind == TOK_DIV){
            dest->kind = token.kind == TOK_MUL ? NODE_MUL : NODE_DIV;

            Parser parser_left = parser_new(parser.list, parser.from, parser.from + parser.offset - 2);
            Parser parser_right = parser_new(parser.list, parser.from + parser.offset, parser.to);

            parse_term(left, parser_left);
            bool is_factor = parse_factor(right, parser_right);
            parser_reset(&parser);
            bool is_term = parse_term(right, parser_right);

            dest->left = left;
            dest->right = right;

            if(!is_term && !is_factor) return false;
            return true;
        }
    }

    parser_reset(&parser);
    bool is_factor = parse_factor(dest, parser);

    return is_factor;
}

bool parse_expr(Node *dest, Parser parser){
    if(dest == NULL) return false;

    Token token;
    Node *left = malloc(sizeof(left[0]));
    Node *right = malloc(sizeof(right[0]));

    while (parser_next(&token, &parser)) {
        if(token.kind == TOK_ADD || token.kind == TOK_SUB){
            dest->kind = token.kind == TOK_ADD ? NODE_ADD : NODE_SUB;

            Parser parser_left = parser_new(parser.list, parser.from, parser.from + parser.offset - 2);
            Parser parser_right = parser_new(parser.list, parser.from + parser.offset, parser.to);

            parse_term(left, parser_left);
            bool is_term = parse_term(right, parser_right);
            parser_reset(&parser);
            bool is_expr = parse_expr(right, parser_right);

            dest->left = left;
            dest->right = right;

            if(!is_expr && !is_term) return false;
            return true;
        }
    }

    return false;
}

int main(void) {
    char str[] = "1+2*   (2 - 1)";

    TokenList list = token_list_alloc();
    lexer(str, &list);
    Node node = node_new();
    Parser parser = parser_new(&list, 0, TOKEN_LIST_END(list));
    parse_expr(&node, parser);
    
    return 0;
}
