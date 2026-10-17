#include <stdio.h>

typedef struct Div Div;
struct Div{
    int count;
    int nums[256];
};

Div get_div(int n){
    Div d;
    d.count = 0;
    for(int i = 1; i < n - 1; ++i){
        if(n % i == 0){
            d.nums[d.count] = i;
            ++d.count;
        }
    }

    return d;
}

int get_div_sum(Div d){
    int sum = 0;
    for(int i = 0; i < d.count; ++i){
        sum += d.nums[i];
    }
    return sum;
}

int main(void) {
    int m;
    int n;
    scanf("%d %d",&m,&n);
    Div m_d = get_div(m);
    Div n_d = get_div(n);
    if(get_div_sum(m_d) == n && m == get_div_sum(n_d)){
        printf("yes");
    }else {
        printf("no");
    }
    printf(" %d %d",m_d.count,n_d.count);
    return 0;
}
