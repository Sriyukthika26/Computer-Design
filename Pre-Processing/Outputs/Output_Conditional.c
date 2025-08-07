#include <stdio.h>

int feature() {
    return 1;
}

int main() {
    int result = feature();
    printf("%d", result);
    return 0;
}
