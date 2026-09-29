#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 获取符号位
uint32_t get_S(uint32_t n){
    return n >> 31;
}

// 获取指数位
uint32_t get_E(uint32_t n){
    return 0x000000ff & (n >> 23);
}

// 获取尾数位
uint32_t get_M(uint32_t n){
    return 0x007fffff & n;
}

// 舍去前导零 将二进制位转化位字符串
char* to_bin_compact(uint32_t n){
    int bits = 1;
    for(int i = 31;i >= 0;i--){
        if(((n >> i) & 1) == 1){
            bits = i + 1; 
            break;
        }   
    }
    char* str = (char*)malloc(sizeof(char)*bits + 1);

    int offset = 0;
    for (int i = bits - 1; i >= 0; i--) {
        str[offset] = ((n >> i) & 1) ? '1' : '0';
        offset++;
    }
    
    str[offset] = '\0';

    return str;
}

// 根据指定位数 保留后面的位 将二进制位转化为字符串
char* to_bin(uint32_t n,int bits){
    char* str = (char*)malloc(sizeof(char)*bits + 1);

    int offset = 0;
    for (int i = bits - 1; i >= 0; i--) {
        str[offset] = ((n >> i) & 1) ? '1' : '0';
        offset++;
    }
    
    str[offset] = '\0';

    return str;
}

// 展示一个浮点数在内存中的存储结构
void print_float_inst(float x,char* name){
    printf("%s的原始值：%g\n",name,x);

    uint32_t p;
    memcpy(&p, &x, sizeof(uint32_t));

    printf("符号位S=%d\n",get_S(p));
    
    char *E = to_bin(get_E(p), 8);
    char *M = to_bin(get_M(p), 23);
    char *ALL = to_bin(p, 32);

    printf("指数位E：%s\n",E);
    printf("尾数位M：%s\n",M);
    printf("完整二进制：%s\n",ALL);

    free(E);
    free(M);
    free(ALL);
}

int main(void) {
    float x = -(float)41/64;
    print_float_inst(x,"x");
 
    // 0.1不能精确的被转换为float
    float y = 0.1;
    print_float_inst(y, "y");
    
    // 就近舍入 偶数优先 但是存在有些数不能用二进制精确表示 可能以为偶数优先但实际上是就近舍入的
}
