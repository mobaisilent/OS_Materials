#include <stdio.h>
#include <stdlib.h>

#define maxPCB 6 //最大进程数
#define maxPart 6 //最大空闲分区数
#define size 10 //不在切割剩余分区的大小

typedef struct PCB_type {
    char name; //进程名
    int address; //进程所占分区首地址
    int len; //进程所占分区的长度
    int valid; //PCB标识符（有效，无效）
} PCB;

typedef struct seqlist {
    PCB PCBelem[maxPCB]; //maxPCB为系统中允许的最多进程数
    int total; //系统中实际的进程数
} PCBseql;

typedef struct Partition {
    int address; //分区起址
    int len; //分区的长度
    int valid; //有效标识符（有效，无效）
} Part;

typedef struct Partlist {
    Part Partelem[maxPart]; //maxPart 为系统中可能的最多空闲分区数
    int sum; //系统中实际的分区数
} Partseql;

//全局变量
PCBseql *pcbl; //进程队列指针
Partseql *partl; //空闲队列指针

void initpcb() {
    int i;
    pcbl->PCBelem[0].address = 0;
    pcbl->PCBelem[0].len = 40;
    pcbl->PCBelem[0].name = 's';
    pcbl->PCBelem[0].valid = 1;
    pcbl->total = 1; //初始化有一个有效进程
    for (i = 1; i < maxPCB; i++) {
        pcbl->PCBelem[i].name = '\0';
        pcbl->PCBelem[i].address = 0;
        pcbl->PCBelem[i].len = 0;
        pcbl->PCBelem[i].valid = 0;
    }
}

void initpart() {
    int i;
    partl->Partelem[0].address = 40;
    partl->Partelem[0].len = 600;
    partl->Partelem[0].valid = 1;
    for (i = 1; i < maxPart; i++) {
        partl->Partelem[i].address = 0;
        partl->Partelem[i].len = 0;
        partl->Partelem[i].valid = 0;
    }
    partl->sum = 1;
}

void request(char name, int len) {
    int i, j, k;
    int address;
    for (i = 0; i < partl->sum; i++) {
        if (partl->Partelem[i].len >= len) {
            address = partl->Partelem[i].address;
            if (partl->Partelem[i].len - len >= size) {
                partl->Partelem[i].address += len;
                partl->Partelem[i].len -= len;
                partl->Partelem[i].valid = 1;
            } else {
                for (j = i; j < maxPart - 1; j++) {
                    partl->Partelem[j] = partl->Partelem[j + 1];
                }
                partl->Partelem[j].valid = 0;
                partl->Partelem[j].address = 0;
                partl->Partelem[j].len = 0;
                partl->sum--;
            }
            for (k = 0; k < maxPCB; k++) {
                if (pcbl->PCBelem[k].valid == 0) {
                    pcbl->PCBelem[k].address = address;
                    pcbl->PCBelem[k].len = len;
                    pcbl->PCBelem[k].name = name;
                    pcbl->PCBelem[k].valid = 1;
                    pcbl->total++;
                    break;
                }
            }
            break;
        }
    }
    if (i == partl->sum) {
        printf("内存紧张，暂时不予分配，请等待！\n");
    }
}

void release(char name) {
    int i;
    for (i = 0; i < maxPCB; i++) {
        if (pcbl->PCBelem[i].name == name) {
            if (pcbl->PCBelem[i].valid == 0) {
                printf("%c进程非运行进程，无法结束！\n", name);
            } else {
                pcbl->PCBelem[i].valid = 0;
                pcbl->total--;
                partl->Partelem[partl->sum].address = pcbl->PCBelem[i].address;
                partl->Partelem[partl->sum].len = pcbl->PCBelem[i].len;
                partl->Partelem[partl->sum].valid = 1;
                partl->sum++;
            }
        }
    }
}

void print() {
    int i;
    printf("当前的进程有：\n");
    printf("name address length\n");
    for (i = 0; i < maxPCB; i++) {
        if (pcbl->PCBelem[i].valid == 1) {
            printf("%c %d %d\n", pcbl->PCBelem[i].name, pcbl->PCBelem[i].address, pcbl->PCBelem[i].len);
        }
    }
    printf("当前的空闲分区有：\n");
    printf("address length\n");
    for (i = 0; i < maxPart; i++) {
        if (partl->Partelem[i].valid == 1) {
            printf("%d %d\n", partl->Partelem[i].address, partl->Partelem[i].len);
        }
    }
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char* argv[]) {
    char ch;
    char pcbname;
    int pcblen;
    PCBseql pcb;
    Partseql part;
    pcbl = &pcb;
    partl = &part;
    initpcb();
    initpart();
    printf("\t****************************MENU****************************\n");
    printf("\t************** Enter: r 请求分配内存 **************\n");
    printf("\t************** Enter: s 结束进程 **************\n");
    printf("\t************** Enter: p 打印分配情况 **************\n");
    printf("\t************** Enter: e 退出 **************\n");
    while (1) {
        ch = getchar();
        clear_input_buffer();
        switch (ch) {
            case 'r':
                printf("请输入请求进程的name，len: ");
                scanf("%c %d", &pcbname, &pcblen);
                clear_input_buffer();
                request(pcbname, pcblen);
                break;
            case 's':
                printf("请输入要结束进程的name：");
                scanf("%c", &pcbname);
                clear_input_buffer();
                release(pcbname);
                break;
            case 'p':
                print();
                break;
            case 'e':
                exit(0);
            default:
                printf("无效的输入，请重新输入\n");
        }
    }
    return 0;
}
