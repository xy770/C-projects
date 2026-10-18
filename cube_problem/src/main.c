#include <assert.h>
#include <stdio.h>

int main(void) {
    int input = 0;
    scanf("%d", &input);

    int start = input * input - input + 1;
    int sum = 0;
    for(int i = 1; i <= input; ++i){
        int result = start + (i - 1) * 2;
        sum += result;
        if(i != input) printf("%d ",result);
        if(i == input) printf("%d\n",result);
    }

    assert(sum == input * input * input);

    return 0;
}
