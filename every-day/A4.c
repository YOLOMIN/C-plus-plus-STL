#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

struct Block {
    size_t size;      // 整个块的大小（含块头）
    struct Block *next;
};

// 从空闲链表里分配 n 字节，返回用户可用的地址（跳过块头）
void *alloc_block(struct Block **head, size_t n)
{
    struct Block *p = *head;

    // 1. 遍历链表，找第一块 size >= n + sizeof(Block) 的块
    while (p && p->size < n + sizeof(struct Block)) {
        p = p->next;
    }
    if (!p) return NULL;   // 没找到够大的

    // 2. 算剩余块的信息
    char *user = (char *)p + sizeof(struct Block);         // 用户地址
    size_t remain_size = p->size - (n + sizeof(struct Block)); // 剩余大小

    // 3. 如果剩余部分够放一个块头，切两块；否则整块给出去
    if (remain_size >= sizeof(struct Block)) {
        // 切：在 (char*)p + n + sizeof(Block) 处放一个新块
        struct Block *remain = (struct Block *)((char *)p + n + sizeof(struct Block));
        remain->size = remain_size;
        remain->next = p->next;
        *head = remain;   // 剩余块取代原块位置
    } else {
        *head = p->next;  // 剩余太小，整块给出去
    }

    // 4. 记录用户块大小，返回用户地址
    p->size = n + sizeof(struct Block);
    return user;
}

void my_free(struct Block **head,void *ptr){
	
	struct Block *block = (struct Block*) ptr - 1;
	
	block->next = *head;
	*head = block;

	struct Block *prev = NULL;
	struct Block *q = *head;
	while(q != NULL){
		if(q != block && (char*)block + block->size == (char*)q){
		
			block->size += q->size;
			if(prev == NULL)
				*head = q->next;
			else
				prev->next = q->next;
			break;
		}

		prev = q;
		q = q->next;
	}

}
int main(){
	
	struct Block* test = malloc(512);
	test->size = 512;        // ← 告诉链表：这块 512 字节
	test->next = NULL;       // ← 这是唯一一块，next 指向空	
	struct Block *test1 = alloc_block(&test,200);
	printf("原块地址: %p, 大小: %zu\n", (void*)test, test->size);
	printf("分配后用户地址: %p\n", test1);
	printf("剩余块地址: %p, 剩余大小: %zu\n", (void*)test, test->size);
	
	void *u1 = alloc_block(&test,200);
	printf("分配得到: %p\n", u1);
	
	// 释放它
	my_free(&test, u1);
	printf("释放后 free_list 指向: %p\n", (void*)test);  // 应该回到块头地址

	// 再分配，应该拿到同一块
	void *u2 = alloc_block(&test, 200);
	printf("再分配得到: %p\n", u2);
	printf("两次地址相同? %s\n", (u1 == u2) ? "yes" : "no");
	

	char *buf = malloc(512);
	
	struct Block *A = (struct Block *)buf;          // A 在 buf
	struct Block *B = (struct Block *)(buf + 128);  // B 紧跟 A
	A->size = 128; 
	B->size = 128;
	B->next = NULL;

	struct Block *head = B;
    	printf("释放前: head=%p, size=%zu\n", (void*)head, head->size);
	
       // 用户归还 A → my_free 应把 A 插回链表，并和相邻的 B 合并
       my_free(&head, (char *)A + sizeof(struct Block));   // 传 A 的数据区地址
       printf("释放后: head=%p, size=%zu, next=%p\n",
           (void*)head, head->size, (void*)head->next);	
	    return  0;
}
