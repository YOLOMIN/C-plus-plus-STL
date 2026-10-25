#include <stdio.h>

int main(){
	int a[4] ={10,20,30,40};
	int *p;
	p= a;
	printf("a[0]=%d,*p=%d,p=%p,&a[0]=%p\n",a[0],*p,p,&a[0]);
	for(int i=0;i<4;i++){
		printf("a[%d]=%d\n",i,*(p+i));
	}
	return 0;
}
