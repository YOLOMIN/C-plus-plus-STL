#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Task{
	int id;
	char name[32];
} task;

int main(){

	//task t = malloc(sizeof(task));
	task t;
	t.id = 1;
	strcpy(t.name,"read");
	
	task *p = &t;
	printf("%d %s %d %s",(*p).id,(*p).name,p->id,p->name);


 return 0;
}
