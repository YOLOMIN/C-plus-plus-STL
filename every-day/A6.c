#include <stdio.h>
#include <stdlib.h>


#define HEAP_SIZE 4096

struct Block
{
    size_t size;    //本快总大小
    int is_free;
    struct Block *next;
};

static char heap[HEAP_SIZE];
static struct Block *head;

void heap_init(void){
    head = (struct Block *)heap;
    head->size = HEAP_SIZE;
    head->is_free = 1;
    head->next = NULL;
}

void  my_free(void *ptr){
    if(ptr == NULL)
        return;
    struct Block *blk = (struct Block *)ptr -1;
    blk->is_free = 1;

    struct Block *q = blk->next;
    if(q && q->is_free && ((char *)blk +blk->size == (char*)q)){
        
        blk->size += q->size;
        blk->next = q->next;

    }

}
char* my_malloc(size_t size){
    struct Block *p = head;

    while(p != NULL){
        if(p->is_free == 1 && p->size >= size + sizeof(struct Block)){
            size_t need = size + sizeof(struct Block);
            if(p->size>= need+sizeof(struct Block)){
                struct Block * rest = (struct Block*)((char *)p + need);
                rest->size = p->size - need;
                rest->is_free = 1;
                rest->next = p->next;
                p->next = rest;
                p->size = need;

            }
            
            p->is_free = 0;
            return (char*)p + sizeof(struct Block);
        }
        p = p->next;
    }

    return NULL;
}

int main(){

    heap_init();

    // void *a = my_malloc(16);
    // my_free(a);
    // void *b = my_malloc(16);

    // printf("a=%p\nb=%p\n", a, b);
    // printf("b == a ? %s\n", (a == b) ? "yes" : "no");

    // my_free(a);
    // my_free(NULL);

    // heap_init();

void *a = my_malloc(16);      // A 占用，后面剩一大块空闲
printf("分配前 head->size = %zu\n", head->size);   // 40

my_free(a);                    // 释放 A → 应和后面那块合并

printf("合并后 head->size = %zu\n", head->size);   // 期望 4096
printf("合并后 head->next = %p\n", (void*)head->next); // 期望 (nil)

}
