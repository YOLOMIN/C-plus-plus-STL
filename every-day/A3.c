#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 64
#define BLOCK_COUNT 4

struct Pool {
    char memory[BLOCK_COUNT * BLOCK_SIZE];  // 一整块连续内存
    void *free_list;                         // 链表头
};

// ① 初始化：把 memory 切成 4 块，串成空闲链表
void pool_init(struct Pool *p) {
    // 你的代码：
    // 循环 i 从 0 到 BLOCK_COUNT-1：
    //   第 i 块地址 = (void*)(p->memory + i * BLOCK_SIZE)
    //   把这块开头 8 字节写成「下一块地址」；最后一块写 NULL
    // 最后 p->free_list = 第 0 块地a址
    for(int i = 0;i<BLOCK_COUNT;i++){
	void *block = (void*)(p->memory + i * BLOCK_SIZE);
	void *next = (i == BLOCK_COUNT -1)?NULL:(void *)(p->memory + (i+1)*BLOCK_SIZE);
	*(void **)block = next;
    }

    p->free_list = (void*)p->memory;
}

// ② 分配：从链表头取一块
void *pool_alloc(struct Pool *p) {
    // 你的代码：
    // 如果 free_list == NULL，返回 NULL
    // 记下当前头 → 头后移一位 → 返回记下的块
    if(p->free_list == NULL)
	    return NULL;
    void *block = p->free_list;
    p->free_list = *(void **)block;
    return block;
}

// ③ 释放：把块插回链表头
void pool_free(struct Pool *p, void *block) {
    // 你的代码：
    // *(void**)block = p->free_list;   // 归还的块指向当前头
    // p->free_list = block;             // 头指向归还的块
    *(void**)block = p->free_list;
    p->free_list = block;
}

int main(){
	struct Pool p;
	pool_init(&p);

	void *a = pool_alloc(&p);
	void *b = pool_alloc(&p);
	void *c = pool_alloc(&p);
	void *d = pool_alloc(&p);
	void *e = pool_alloc(&p);

	printf("a=%p b=%p c=%p d=%p e=%p\n", a, b, c, d, e);
	
	pool_free(&p,a);
	void *f = pool_alloc(&p);

	printf("a==f ? %s\n", (a == f) ? "yes" : "no");
	
	return 0;
}
