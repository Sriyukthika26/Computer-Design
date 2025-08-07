#include <stdio.h>
#define ENABLE_FEATURE

#ifdef ENABLE_FEATURE
    #ifdef FEATURE_ENABLED
    int feature(){
        return 2;
    }
    #endif
int feature() {
    return 1;
}
#else
int feature() {
    return 0;
}
#endif

int main() {
    int result = feature();
    printf("%d", result);
    return 0;
}
