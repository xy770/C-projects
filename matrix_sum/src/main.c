#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t get_num_by_index(char *buf, uint32_t index){
    uint32_t len = strlen(buf);
    uint32_t result = 0;

    uint32_t offset = 0;

    if(index == 0){
        sscanf(buf, "%d", &result);
        return result;
    }

    while (offset < len) {
        if(buf[offset] == ' '){
            --index;
            if(index == 0){
                sscanf(buf + offset, " %d", &result);
                return result;
            }
        }
        ++offset;
    }
    return result;
}

int cmp(const void *a, const void *b){
    uint32_t _a = *(uint32_t*)a;
    uint32_t _b = *(uint32_t*)b;
    if(_a == _b) return 0;
    if(_a > _b) return -1;
    return 1;
}

int main(void) {
    uint32_t m = 0;
    scanf("%d\n", &m);
    uint32_t main_index = 0;
    uint32_t co_index = m - 1;

    uint32_t main_sum = 0;
    uint32_t co_sum = 0;
    uint32_t *res = calloc(m + 2,sizeof(uint32_t) * (m + 2));
    
    while (main_index <= m - 1) {
        char buffer[256] = {0};
        fgets(buffer, 256, stdin);

        main_sum += get_num_by_index(buffer, main_index);
        co_sum += get_num_by_index(buffer, co_index);

        for(uint32_t i = 0; i < m; ++i){
            res[main_index] += get_num_by_index(buffer, i);
        }
        
        ++main_index;
        --co_index;
    }

    res[m] = main_sum;
    res[m + 1] = co_sum;
    qsort(res, m + 2, sizeof(uint32_t), cmp);    
    for(uint32_t i = 0; i < m + 2; ++i){
        if(i == m + 1){
            printf("%d\n", res[i]);
            break;
        }
        printf("%d ", res[i]);
    }
    return 0;
}
