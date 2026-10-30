#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define  BUCKETS 8



struct Entry {
    char *key;
    struct Entry *next;
};

struct Entry *buckets[BUCKETS];     // 全局，自动全NULL

unsigned long hash(const char *s){
    unsigned long h = 5381;      // ← 初始值
    
    while (*s != '\0') {     // *s 是当前字符；到 '\0' 就停
        // 这里用 *s
        h = h * 33 + (unsigned char)*s;               // 指针前移，指向下一个字符
        s++;
    }

    return h;
}

void insert(const char *key){
    
    unsigned long idx = hash(key) % BUCKETS;
    // buckets = malloc(sizeof(struct Entry*)*BUCKETS);
    // strcpy(*key,buckets[idx]);

    struct Entry *e = malloc(sizeof(struct Entry));
    if(e == NULL) return;

    e->key = malloc(strlen(key) + 1);
    if(e->key == NULL) {
        free(e);
        return;
    }

    strcpy(e->key,key);

    e->next =  buckets[idx];

    buckets[idx] = e;

}

void print_table(void){
    for(int i = 0;i < BUCKETS; i++){
        printf("[%d]",i);

        struct Entry *p = buckets[i];

        while(p != NULL){
            printf("-> %s",p->key);
            p = p->next;
        }

        printf("\n");
    }
}

struct Entry *lookup(const char *key){
    unsigned long idx = hash(key) % BUCKETS;

    struct Entry *p = buckets[idx];

    while(p != NULL ){

        if(strcmp(p->key,key) == 0)
            return p;

        p = p->next;
    }

    return NULL;
}

void free_table(void){

    for(int i =0;i<BUCKETS;i++){
        struct Entry *p = buckets[i];
        while(p != NULL ){
            struct Entry *next = p->next;
            free(p->key);
            free(p);
            p = next;
        }
        buckets[i] = NULL;
    }
}

int main(void) {
    insert("apple");
    insert("banana");
    insert("cat");
    insert("dog");
    print_table();

    printf("\n--- lookup ---\n");
    printf("apple  : %s\n", lookup("apple")  ? "找到" : "没找到");
    printf("dog    : %s\n", lookup("dog")    ? "找到" : "没找到");
    printf("banana : %s\n", lookup("banana") ? "找到" : "没找到");
    printf("zzz    : %s\n", lookup("zzz")    ? "找到" : "没找到");
    free_table();

    return 0;
}