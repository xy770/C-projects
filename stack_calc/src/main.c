#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    int32_t stack[1024];
    uint32_t sp;
}Stack;

static inline int32_t is_digit(char c){
    return c >= '0' && c <= '9';
}

static inline int32_t to_int(char c){
    return c - '0';
}

int32_t pop(Stack *stack){
    assert(stack->sp >= 1);
    return stack->stack[--stack->sp];
}

int32_t peek(Stack *stack){
    assert(stack->sp >= 1);
    return stack->stack[stack->sp - 1];
}

int8_t is_empty(Stack *stack){
    return stack->sp == 0 ? 1 : 0;
}

void push(Stack *stack, int32_t v){
    stack->stack[stack->sp++] = v;
}

void to_post(char *expr, uint32_t len){
    Stack stack = { 0 };
    char output[256] = { 0 };

    for(int i = 0; i < len;){
        if(is_digit(expr[i])){
            int32_t result = 0;
            while (is_digit(expr[i])) {
                result = result * 10 + to_int(expr[i]);
                ++i;
            }

            sprintf(output, "%s %d", output, result);
        }else if(expr[i] == '+' || expr[i] == '-'){
            if(is_empty(&stack)){
                push(&stack, expr[i]);
            }else {
                while (!is_empty(&stack)) {
                    char op = pop(&stack);
                    sprintf(output, "%s %c", output, op);
                }
                push(&stack, expr[i]);
            }
            ++i;
        }else if(expr[i] == '*' || expr[i] == '/'){
            if(is_empty(&stack)){
                push(&stack, expr[i]);
            }else if(peek(&stack) == '+' || peek(&stack) == '-'){
                push(&stack, expr[i]);
            }else {
                while (!is_empty(&stack) || peek(&stack) == '*' || peek(&stack) == '/') {
                    char op = pop(&stack);
                    sprintf(output, "%s %c", output, op);
                }
                push(&stack, expr[i]);
            }
            ++i;
        }else{
            ++i;
        }
    }

    while (!is_empty(&stack)) {
        char op = pop(&stack);
    
        sprintf(output, "%s %c", output, op);
    }

    sprintf(expr, "%s", output);
}

int32_t eval(const char *expr, uint32_t len){
    Stack stack = { 0 };
    for(int i = 0; i < len;){
        if(is_digit(expr[i])){
            int32_t result = 0;
            while (is_digit(expr[i])) {
                result = result * 10 + to_int(expr[i]);
                ++i;
            }
            push(&stack, result);
        }else if(expr[i] == '+'){
            ++i;
            int32_t rv = pop(&stack);
            int32_t lv = pop(&stack);

            push(&stack, lv + rv);
        }else if(expr[i] == '-'){
            ++i;
            int32_t rv = pop(&stack);
            int32_t lv = pop(&stack);

            push(&stack, lv - rv);
        }else if(expr[i] == '*'){
            ++i;
            int32_t rv = pop(&stack);
            int32_t lv = pop(&stack);

            push(&stack, lv * rv);
        }else if(expr[i] == '/'){
            ++i;
            int32_t rv = pop(&stack);
            int32_t lv = pop(&stack);

            push(&stack, lv / rv);
        }else {
            ++i;
        }
    }

    int32_t result = pop(&stack);
    return result;
}

int main(void){
    char expr[256] = { 0 };

    // 1: use scanf("%s", expr);
    fgets(expr, sizeof(expr), stdin);

    uint32_t len = strlen(expr);
    to_post(expr, len);
    len = strlen(expr);
    int32_t result = eval(expr, len);
    printf("Result: %d\n", result);
}

