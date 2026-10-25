#include <stdio.h>

int main(){

	int x =42;
	int *p = &x;
	printf("x=%d,&x=%p,p=%p,*p=%d\n",x,&x,p,*p);
	*p =100;
	printf("x=%d,&x=%p,p=%p,*p=%d\n",x,&x,p,*p);
}
