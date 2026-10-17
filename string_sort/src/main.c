#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp_ch(const void* a,const void* b){
    return *(char*)a - *(char*)b;
}

int powi(int x,int times){
    int r = 1;
    while (times > 0) {
        r *= x; 
        times--;
    }
    return r;
}

int get_unique_v(const char *str){
    int ret = 0;
    char buffer[128] = {0};
    snprintf(buffer, sizeof(buffer), "%s", str);

    qsort(buffer, strlen(buffer), sizeof(char), cmp_ch);
    for(int i = 0;buffer[i] != '\0';++i){
        ret += powi(100,i) * buffer[i];
    }
    unsigned int hash = 0;
    for(int i = 0; buffer[i] != '\0'; ++i){
        hash = hash * 131 + buffer[i]; 
    }
    
    return hash & 0x7FFFFFFF;
    return ret;
}

int is_simi(const char *str1,const char *str2){
    return get_unique_v(str1) == get_unique_v(str2);
}

int cmp(const void *a,const void *b){
    return get_unique_v(*(char**)a) - get_unique_v(*(char**)b);
}

/**
 * 核心函数：你需要在这里实现你的算法
 * 
 * 参数说明：
 * - strs: 输入的字符串数组
 * - strsSize: 字符串数组的长度
 * - returnSize: 这是一个输出参数，你需要把它赋值为分组的总数
 * - returnColumnSizes: 这是一个输出参数，你需要为它分配内存，并填入每一组中字符串的数量
 * 
 * 返回值: 返回一个三维字符数组 (char***)，代表分组后的结果
 */
char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
    char ***ret = malloc(strsSize * 32);
    *returnColumnSizes = calloc(strsSize,sizeof(int));
    *returnSize = 0;

    qsort(strs, strsSize, sizeof(char*), cmp);
    for(int i = 0; i < strsSize; ++i){
        if(*returnSize == 0){
            ret[0] = strs;
            ++*returnSize;
            ++(*returnColumnSizes)[0];
        }else if (is_simi(strs[i], strs[i - 1])) {
            ++(*returnColumnSizes)[*returnSize - 1];
        }else {
            ++*returnSize;
            ret[*returnSize - 1] = strs + i;
            ++(*returnColumnSizes)[*returnSize - 1];
        }
    }

    return ret;
}


/**
 * 本地测试框架
 */
int main() {
    // 1. 准备测试用例 (你可以修改这里来测试不同的输入)
    char* strs[] = {"eat", "tea", "tan", "ate", "nat", "bat"};
    int strsSize = 6;
    
    int returnSize = 0;
    int* returnColumnSizes = NULL;
    
    printf("输入: [\"eat\", \"tea\", \"tan\", \"ate\", \"nat\", \"bat\"]\n");
    
    // 2. 调用你的核心函数
    char*** result = groupAnagrams(strs, strsSize, &returnSize, &returnColumnSizes);
    
    // 3. 打印输出结果 (模拟 LeetCode 的输出格式)
    printf("输出: [");
    for (int i = 0; i < returnSize; i++) {
        printf("[");
        for (int j = 0; j < returnColumnSizes[i]; j++) {
            printf("\"%s\"", result[i][j]);
            if (j < returnColumnSizes[i] - 1) {
                printf(",");
            }
        }
        printf("]");
        if (i < returnSize - 1) {
            printf(",");
        }
    }
    printf("]\n");
    
    // 4. 释放内存 (本地测试时建议加上，防止内存泄漏)
    // 注意：释放逻辑需要与你实现 groupAnagrams 时的 malloc 逻辑严格对应
    /*
    if (result != NULL) {
        for (int i = 0; i < returnSize; i++) {
            for (int j = 0; j < returnColumnSizes[i]; j++) {
                free(result[i][j]); // 如果你在函数内 malloc 了字符串
            }
            free(result[i]); // 释放每一组的字符串指针数组
        }
        free(result); // 释放最外层数组
    }
    if (returnColumnSizes != NULL) {
        free(returnColumnSizes);
    }
    */

    return 0;
}
