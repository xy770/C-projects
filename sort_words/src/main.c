#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b){
    return strcmp(a, b);
}

int main(void) {
    char words[64][16] = { 0 };
    int word_count = 0;
    int word_len = 0;
    char buf[512] = { 0 };

    fgets(buf, sizeof(buf), stdin); // 对于考试题目，输入一定合法，所以可以不用处理返回值

    char *p = buf; // 数组退化成指针，p指向的是buf第一个字符的地址
    while (*p != '\0') {
        if(*p == ' ' || *p == '\n' || *p == '\r'){
            ++p;
            continue;
        }

        word_len = 0;
        while (*p != '\0' && *p != ' ' && *p != '\n' && *p != '\r') {
            words[word_count][word_len++] = *p++; // *p++这里有优先级问题，++的优先级比*要高。
        }
        words[word_count][word_len] = '\0';
        ++word_count;
    }

    qsort(words, word_count, sizeof(words[0]), cmp);

    for (int i = 0; i < word_count; ++i) {
        printf("%01d: %s\n", i, words[i]);
    }
    
    return 0;
}
