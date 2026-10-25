#include <stdio.h>
#include <stdlib.h>   // malloc 和 free 在这

int main(void) {
    // 1. 申请 5 个 int
    int *p = malloc(sizeof(int)*5);
    // 2. 检查 NULL
    if(p == NULL) return -1;
    // 3. 赋值 1~5
    for(int i=0;i<5;i++)p[i]=i;
    // 4. 打印
    for(int i=0;i<5;i++) printf("%d ",p[i]);
    // 5. free
    free(p);

    return 0;
}
