#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Buf{
    int refs;       //引用计数
    char data[64];
};

struct Buf *buf_new(const char *s);   //创建：malloc + refs=1 + 拷贝 s

void buf_retain(struct Buf *b);     // 借用：refs++

void buf_release(struct Buf *b);    // 归还：refs--；为 0 才 free


struct Buf *buf_new(const char *s){
    struct Buf * buf = malloc(sizeof(struct Buf));
    if(buf == NULL ) return NULL;
    buf->refs = 1;
    strcpy(buf->data,s);
    return buf;
}

void buf_retain(struct Buf *b){
    if(b == NULL) return;
    b->refs++;
}

void buf_release(struct Buf *b){
    if(b == NULL) return;
    
    b->refs--;
    if(b->refs == 0){
        free(b);
        printf("对象已释放\n");
    }
}


int main(void) {
    struct Buf *b = buf_new("hello");
    printf("创建后 refs=%d\n", b->refs);      // 期望 1

    buf_retain(b);
    printf("retain 后 refs=%d\n", b->refs);   // 期望 2

    buf_retain(b);
    printf("retain 后 refs=%d\n", b->refs);   // 期望 3

    buf_release(b);
    printf("release 后 refs=%d\n", b->refs);  // 期望 2

    buf_release(b);
    printf("release 后 refs=%d\n", b->refs);  // 期望 1

    buf_release(b);   // 这次应该真正释放，之后不能再访问 b
    return 0;
}