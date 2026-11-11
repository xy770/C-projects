#include <assert.h>
#include <ctype.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum{
    TOK_NUM,
    TOK_ADD, TOK_MUL, TOK_SUB, TOK_DIV,
    TOK_LPAREN, TOK_RPAREN,
    TOK_EOF,
}TokenKind;

typedef struct{
    TokenKind kind;
    float num;
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
    float num;
    Node *left;
    Node *right;
};

typedef struct{
    const TokenList *list;
    size_t offset;
    bool is_ok;
    char err_str[64];
}Parser;

typedef struct Arena Arena;
struct Arena{
    size_t capacity;
    size_t used;
    Arena *next;
    alignas(max_align_t) char data[];
};

// expr -> term ('+' term | '-' term)*
// term -> factor ('*' factor | '/' factor)*
// factor -> NUM | '(' expr ')'
#define TOKEN_LIST_END(x) ((x).len - 1)

Node* parse_expr(Arena *arena, Parser *parser);
Node* parse_term(Arena *arena, Parser *parser);
Node* parse_factor(Arena *arena, Parser *parser);

#define MAX_ALIGN 8
#define INITIAL_ARENA_SIZE 1024

size_t align_up(size_t n, size_t align){
    assert(align % 2 == 0);
    return (n + align - 1) & ~(align - 1);
}

void arena_free(Arena **a){
    if(a == NULL || *a == NULL) return;

    Arena *arenas[256] = {0};
    int i = 0;
    arenas[i] = *a;
    while (arenas[i]->next != NULL){
        arenas[i+1] = arenas[i]->next;
        ++i;
    }

    for(int j = i; j >= 0; --j){
        free(arenas[j]);
    }

    *a = NULL;
}

Arena* arena_alloc(void){
    Arena *a = NULL;

    size_t target_cap = INITIAL_ARENA_SIZE;
    align_up(target_cap, MAX_ALIGN);

    a = malloc(sizeof(Arena) + target_cap);
    if(a == NULL) return NULL;

    a->capacity = target_cap;
    a->used = 0;
    a->next = NULL;

    return a;
}

void* arena_pick(Arena *a, size_t size){
    if(size > INITIAL_ARENA_SIZE) return NULL;
    size = align_up(size, MAX_ALIGN);

    Arena *end = a;

    while (end->next != NULL) {
        end = end->next;
    }

    if(end->used + size > end->capacity){
        end->next = arena_alloc();
        if(end->next == NULL) return NULL;
        end = end->next;
    }

    void *ptr = (void*)(end->data + end->used);
    end->used += size;

    return ptr;
}

TokenList token_list_new(){
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

static inline Node node_new(){
    return (Node){
        .kind = NODE_UNKNOWN,
        .num = 0,
        .left = NULL,
        .right = NULL,
    };
}

Token parser_peek(const Parser *parser){
    Token eof = (Token){ .kind = TOK_EOF };
    if(parser->offset >= parser->list->len) return eof;

    return parser->list->data[parser->offset];
}

Token parser_advance(Parser *parser){
    Token token = parser_peek(parser);
    ++parser->offset;
    return token;
}

Node* parse_factor(Arena *arena, Parser *parser){
    if(!parser->is_ok) return NULL;

    Node* root = NULL;

    TokenKind next = parser_peek(parser).kind;
    if(next == TOK_NUM){
       float num = parser_advance(parser).num;

        root = arena_pick(arena, sizeof(*root));
        if(root == NULL) return NULL;

        root->kind = NODE_NUM;
        root->left = NULL;
        root->right = NULL;
        root->num = num;

        return root;
    }else if(next == TOK_LPAREN){
        parser_advance(parser);

        root = parse_expr(arena, parser);
        if(!parser->is_ok) return NULL;

        TokenKind rparen = parser_advance(parser).kind;
        if(rparen != TOK_RPAREN){
            parser->is_ok = false;
            snprintf(parser->err_str, sizeof(parser->err_str),\
                    "Expect ')' at %d", (int)parser->offset - 1);
            return NULL;
        }

        return root;
    }

    parser->is_ok = false;
    snprintf(parser->err_str, sizeof(parser->err_str),\
            "Not a valid expr!");
    return NULL;
}

Node* parse_term(Arena *arena, Parser *parser){
    if(!parser->is_ok) return NULL;

    Node *root = NULL;
    Node *left = parse_factor(arena, parser);
    if(!parser->is_ok) return NULL;

    while (1) {
        TokenKind next = parser_peek(parser).kind;
        if(next == TOK_MUL || next == TOK_DIV){
            parser_advance(parser);

            root = arena_pick(arena, sizeof(*root));
            if(root == NULL){
                parser->is_ok = false;
                snprintf(parser->err_str, sizeof(parser->err_str),\
                        "%s","Failed to alloc memory");
                return NULL;
            }

            root->kind = next == TOK_MUL ? NODE_MUL : NODE_DIV;
            root->left = left;
            root->right = parse_factor(arena, parser);
            if(!parser->is_ok) return NULL;

            left = root;
            root = NULL;

            continue;
        }

        break;
    }

    return left;
}

Node* parse_expr(Arena *arena, Parser *parser){
    if(!parser->is_ok) return NULL;

    Node* root = NULL;
    Node* left = parse_term(arena, parser);
    if(left == NULL) return NULL;

    while (1) {
        TokenKind next = parser_peek(parser).kind;
        if(next == TOK_ADD || next == TOK_SUB){
            parser_advance(parser);

            root = arena_pick(arena, sizeof(*root));
            if(root == NULL){
                parser->is_ok = false;
                snprintf(parser->err_str, sizeof(parser->err_str),\
                        "%s","Failed to alloc memory");
                return NULL;
            }

            root->kind = next == TOK_ADD ? NODE_ADD : NODE_SUB;
            root->left = left;
            root->right = parse_term(arena, parser);
            if(!parser->is_ok) return NULL;

            left = root;
            root = NULL;

            continue;
        }

        break;
    }

    return left;
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
        }else {
            return false;
        }
    }

    return true;
}

bool eval(float* dest, const Node* node){
    if(dest == NULL || node == NULL) return false;

    float leval = 0;
    float reval = 0;
    if(node->kind == NODE_NUM){
        *dest = node->num;
        return true;
    }else if(node->kind == NODE_UNKNOWN){
        return false; 
    }else {
        bool lr = eval(&leval, node->left);
        bool rr = eval(&reval, node->right);       
        if(!(lr & rr)) return false;

        if(node->kind == NODE_ADD){
            *dest = leval + reval;
        }else if(node->kind == NODE_SUB){
            *dest = leval - reval;
        }else if(node->kind == NODE_MUL){
            *dest = leval * reval; 
        }else if(node->kind == NODE_DIV){
            if(reval == 0) return false;
            *dest = leval / reval;
        }

        return true;
    }
}

int main(void) {
    while (1) {
        fputc('>', stdout);
        fflush(stdout);

        char input_buf[256] = {0};
        if(fgets(input_buf, sizeof(input_buf), stdin) == NULL){
            break;
        }
        size_t len = strlen(input_buf);
        input_buf[--len] = '\0';

        if(len == 1 || input_buf[0] == 'q') break;

        TokenList list = token_list_new();
        bool lexer_result = lexer(input_buf, &list);
        if(!lexer_result){
            printf("Detect invalid character\n");
            continue;
        }

        Node *node = NULL;
        Parser parser = (Parser){
            .list = &list,
            .offset = 0,
            .is_ok = true,
            .err_str = "",
        };
        Arena *arena = arena_alloc();
        node = parse_expr(arena, &parser);
        if(!parser.is_ok){
            printf("Error: %s\n", parser.err_str);
            goto clean_up;
        }

        float value = 0;
        bool result = false;
        result = eval(&value, node);
        if(result){
            printf("Result: %.2f\n", value);
        }else {
            printf("Invalid Input!\n");
        }

clean_up:
        token_list_free(&list);
        arena_free(&arena);
    }

    return 0;
}
