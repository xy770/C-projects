#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct String String;

struct String{
    char *f;
    int count;
};

String string_new(char *str){
    int count = 0;

    while (str[count] != '\0') {
        count++;
    }

    char *p = (char*)malloc(sizeof(char)*count);
    memcpy(p, str, sizeof(char)*count);

    return (String){
        .f = p,
        .count = count,
    };
}

void string_print(String *str){
    int offset = 0;
    while (str->count >= offset) {
        offset++;

        putchar(str->f[offset - 1]);
    }
}

void string_append(String *str,char *ap_str){
    int ap_str_len = 0;
    
    while (ap_str[ap_str_len] != '\0') {
        ap_str_len++;
    } 

    str->count += ap_str_len;
    str->f = realloc(str->f, sizeof(char)*str->count);
    memcpy(str->f + str->count - ap_str_len,ap_str,sizeof(char)*ap_str_len);
}

void string_sync_mem(String *str){
    str->f = realloc(str->f, sizeof(char)*str->count);
}

void string_trim(String *str,char *target_char){
    int offset = 0;
    int w_offset = 0;
    int count = 0;
    while (offset <= str->count) {
        str->f[w_offset] = str->f[offset];
        count = w_offset;
        offset++;
        w_offset++;
       if (str->f[offset - 1] == *target_char){
           w_offset--;
       }
   }
    str->count = count;
    string_sync_mem(str);
}

void string_free(String *str){
    free(str);
}

String string_copy(String *str){
    return (String){
        .f = str->f,
        .count = str->count
    };
}

bool string_if_equal(String *s1,String *s2){
    if(s1->count != s2->count) return false;

    int offset = 0;
    while ((s1->count <= offset) && (s2->count <= offset)) {
        if(s1->f[offset] != s2->f[offset]) return false;
    }
    return true;
}

bool string_if_contain(String *str,String *child_str){
    if(child_str->count > str->count) return false;
    if(child_str->count == str->count) return string_if_equal(str, child_str);

    int s_offset = 0;
    int c_offset = 0;

    while (str->count - s_offset >= 1) {
        printf("str->count=%d s_offset=%d child_str->count=%d current_char=%c\n",str->count,s_offset,child_str->count,str->f[s_offset]);
        if (str->f[s_offset] == child_str->f[c_offset]){
            c_offset++;
            if(c_offset == child_str->count) return true;
        }
        else {
            s_offset -= c_offset;
            c_offset = 0;
        }
        s_offset++;
    }
    return false;
}

int main(){
    String s1 = string_new("ababc");
    String s2 = string_new("abc");
    printf("\n%d",string_if_contain(&s1,&s2));
    return 0;
}
