#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint32_t spawn_count = 0;
    uint32_t sque[1000] = {0};
    uint32_t offset = 0;

    scanf("%d%d%d", sque, sque + 1, &spawn_count);

    for (uint32_t i = 2; i < spawn_count;) {
        uint32_t next = sque[offset] * sque[offset + 1];
        if(next >= 9){
            sque[i] = (next - (next % 10)) / 10;
            ++i;
            if(i == spawn_count) break;
            sque[i] = next % 10;
            ++i;
        }else {
            sque[i] = next;
            ++i;
        }
        ++offset;
    }

    for (int i = 0; i < spawn_count; ++i) {
        printf("%d ", sque[i]);
    }

    return 0;
}
