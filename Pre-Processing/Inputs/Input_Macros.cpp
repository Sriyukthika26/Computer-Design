#include <stdio.h>
#define MAX 100
#define SQUARE(x) ((x) * (x))

int main(){
    int arr[MAX];
    for(int i = 0; i < MAX; i++) {
        arr[i] = SQUARE(i);
    }
    
    for(int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    
    return 0;
}