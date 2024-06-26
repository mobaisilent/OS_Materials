#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// 线程函数，生成一个新的线程
void* th_fn(void* arg) {
    cout << "new thread" << endl;
    return (void*)10;
}

int main() {
    pthread_t ptid;
    void* tret;

    // 创建线程
    pthread_create(&ptid, NULL, th_fn, NULL);

    // 监测线程退出的状态
    pthread_join(ptid, &tret);
    cout << "code 2 exit id = " << (int)tret << endl;

    return 0;
}
