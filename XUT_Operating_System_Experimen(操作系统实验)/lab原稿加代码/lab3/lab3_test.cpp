#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();  // 创建子进程

    if (pid < 0) {  // fork 失败
        printf("fork fail!\n");
        exit(1);
    }

    if (pid == 0) {  // 子进程
        printf("Is son:\n");
        execl("/bin/ls", "ls", "-1", NULL);  // 执行 ls 命令
        printf("exec fail!\n");  // 如果 exec 失败，打印错误信息
        exit(1);
    } else {  // 父进程
        printf("ls parent:\n");
        while(1) sleep(1);  // 无限循环
        exit(0);
    }

    return 0;
}
