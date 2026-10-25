
#include <stdio.h>

void set_ptr(int **pp, int *target) {
// 主人填这一行：通过 pp 修改指针的指向
*pp = &(*target);
}

int main(void) {
int a = 10, b = 20;
int *p = &a;
printf("before: *p = %d\n", *p);
set_ptr(&p, &b);
printf("after: *p = %d\n", *p);
return 0;
}
