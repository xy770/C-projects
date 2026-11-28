#include <stdio.h>

int main() {
    int n;
    // 读取数字个数
    if (scanf("%d", &n) != 1) return 0;

    // 开3个数组，分别存储：原始数字、偶数个数、偶数和
    long long nums[100]; 
    int counts[100];
    int sums[100];

    // 第一步：读取数据并计算每个数字的“偶数个数”和“偶数和”
    for (int i = 0; i < n; i++) {
        scanf("%lld", &nums[i]);
        
        long long temp = nums[i];
        int c = 0; // 偶数个数
        int s = 0; // 偶数和

        // 用 do-while 可以完美处理输入本身就是 0 的情况
        do {
            int digit = temp % 10; // 取出最后一位
            if (digit % 2 == 0) {
                c++;
                s += digit;
            }
            temp = temp / 10;      // 去掉最后一位
        } while (temp > 0);

        // 把算好的结果存入对应的数组
        counts[i] = c;
        sums[i] = s;
    }

    // 第二步：找出所有数字中，最大的偶数个数 和 最大的偶数和
    int best_count = counts[0];
    int best_sum = sums[0];
    
    for (int i = 1; i < n; i++) {
        if (counts[i] > best_count) {
            best_count = counts[i];
        }
        if (sums[i] > best_sum) {
            best_sum = sums[i];
        }
    }

    // 第三步：特殊拦截（全是奇数的情况）
    // 如果最大偶数个数都是0，说明没有符合条件的数字，直接输出No
    if (best_count == 0) {
        printf("No\n");
        return 0;
    }

    // 第四步：找交集
    // 遍历数组，看有没有哪个数字，它的个数刚好等于 best_count，且和也等于 best_sum
    for (int i = 0; i < n; i++) {
        if (counts[i] == best_count && sums[i] == best_sum) {
            // 找到了，输出并结束程序
            printf("Yes %lld %d %d\n", nums[i], counts[i], sums[i]);
            return 0;
        }
    }

    // 如果循环结束都没找到，说明不满足条件
    printf("No\n");
    return 0;
}
