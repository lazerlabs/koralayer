#include <kora/syscalls.h>
#include <stdio.h>

int main() {
    printf("Testing sys_putc: ");
    
    int result = sys_putc('A');
    if (result != KORA_SUCCESS) {
        printf("FAILED\n");
        return 1;
    }
    
    result = sys_putc('\n');
    if (result != KORA_SUCCESS) {
        printf("FAILED\n");
        return 1;
    }
    
    printf("SUCCESS\n");
    return 0;
} 