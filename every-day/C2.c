// 【编程】C2 互斥锁（pthread_mutex）

// ■ 先讲原理：为什么要锁

// 两个线程同时改同一个变量，会「丢数」。
// 因为 counter++ 不是一步，CPU 实际分三步：

//   ① 把 counter 从内存读到寄存器
//   ② 寄存器加 1
//   ③ 写回内存

// 如果 A 线程刚做完 ①，时间片用完被挂起；
// B 线程也做了 ①（读到同样的旧值），加上 1 写回；
// 然后 A 醒来，用自己的旧值写回 —— B 那次加 1 就丢了。

// 这就叫 race condition（竞态）。几百次里偶尔丢一两次，
// 所以它特别难查：程序不报错，只是数字不对。

// ■ 锁的作用

// mutex = mutual exclusion（互斥）。
// 同一时刻只允许一个线程进入「临界区」：

//   pthread_mutex_lock(&m);     // 拿锁，拿不到就睡
//   counter++;                  // 临界区，只有我能进来
//   pthread_mutex_unlock(&m);   // 放锁，唤醒等待者

// 没抢到锁的线程会在 lock() 上阻塞（睡眠），
// 不是在原地空转 —— 这点和后面 C3 的原子操作不同。

// ■ 今天的任务（就这一小步）

// 写 C2.c：两个线程，各给全局 counter 加 100000 次。
// 要求跑两遍：

//   第 1 遍：不加锁，打印 counter，看是不是 200000
//   第 2 遍：加 pthread_mutex_t，再打印，看是不是 200000

// 需要的主干 API（只给签名，用法自己想）：

//   #include <pthread.h>
//   int pthread_create(pthread_t *tid, NULL, void *(*fn)(void*), void *arg);
//   int pthread_join(pthread_t tid, NULL);
//   int pthread_mutex_init(pthread_mutex_t *m, NULL);
//   int pthread_mutex_lock(pthread_mutex_t *m);
//   int pthread_mutex_unlock(pthread_mutex_t *m);
//   int pthread_mutex_destroy(pthread_mutex_t *m);

// 编译要点：gcc ./C2.c -o ./C2 -pthread
// （-pthread 必须加，否则链接会失败，这是新手第一坑）

// ■ 卡住时先自查三点

//   1. 线程函数必须是 void* f(void* arg) 这个签名
//   2. 两个线程要 join，不 join 主线程可能先打印、先退出
//   3. 锁一定要 init 过；unlock 一定要配 lock，漏了会死锁

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0; 

// 方案1：静态初始化（简单）
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// 方案2：动态初始化（正规，要配 destroy）
pthread_mutex_t lock;

void *worker(void *arg){
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        counter++;               // ← 这里会有竞态
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}



int main(){

    pthread_t t1,t2;
    pthread_mutex_init(&lock,NULL);

    pthread_create(&t1,NULL,worker,NULL);
    pthread_create(&t2,NULL,worker,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    pthread_mutex_destroy(&lock);

    printf("counter = %d （期望 200000）\n", counter);

    return 0;
}