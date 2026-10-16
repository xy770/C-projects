#include <stdio.h>

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)
#define ABS(a,b) (MAX(a,b) - MIN(a,b))

double target_num = 0;

double sqrt_iter(double input){
    double forward = (input + target_num/input) / 2;
    if(ABS(input,forward) < 0.00001) return forward;
    return sqrt_iter(forward);
}

int main(void) {
    scanf("%lf",&target_num);
    printf("%.5lf",sqrt_iter(30));

   return 0;
}
