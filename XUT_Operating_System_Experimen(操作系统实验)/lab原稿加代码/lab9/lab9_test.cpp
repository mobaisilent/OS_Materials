#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#define TOTAL_INSTRUCTION 200  // 指令流长
#define M 16                   // 实际页数
#define N 4                    // 可用页面数

struct Pro {
  int num, time;
};

int a[TOTAL_INSTRUCTION];
int page[N];

void Input(Pro p[TOTAL_INSTRUCTION]) {
  int m, i, m1, m2;

  srand((unsigned int)time(NULL));
  m = rand() % 160;  // 随机生成初始指令地址

  for (i = 0; i < TOTAL_INSTRUCTION;) {  // 产生指令队列
    if (m < 0 || m > 159) {
      printf("When i==%d, Error, m==%d\n", i, m);
      exit(0);
    }
    a[i] = m;  // 任选一指令访问点m
    a[i + 1] = a[i] + 1;
    a[i + 2] = a[i] + 2;  // 顺序执行两条指令

    m1 = rand() % m;  // 执行前地址指令m1
    a[i + 3] = m1;
    a[i + 4] = m1 + 1;
    a[i + 5] = m1 + 2;  // 顺序执行两条指令

    m2 = rand() % (157 - m1) + m1 + 3;
    a[i + 6] = m2;
    if ((m2 + 2) > 159) {
      a[i + 7] = m2 + 1;
      i += 8;
    } else {
      a[i + 7] = m2 + 1;
      a[i + 8] = m2 + 2;
      i += 9;
    }
    m = rand() % m2;
  }

  for (i = 0; i < TOTAL_INSTRUCTION; i++) {  // 将指令序列变换成页地址流
    p[i].num = a[i] / 10;
    p[i].time = 0;
  }
}

void print(Pro *page1) {  // 打印当前的页面
  for (int i = 0; i < N; i++) {
    std::cout << page1[i].num << " ";
  }
  std::cout << std::endl;
}

int Search(int e, Pro *page1) {
  for (int i = 0; i < N; i++) {
    if (e == page1[i].num) return i;
  }
  return -1;
}

int Max(Pro *page1) {
  int e = page1[0].time, i = 0;
  for (int j = 1; j < N; j++) {  // 找出离现在时间最长的页面
    if (e < page1[j].time) e = page1[j].time;
  }
  for (i = 0; i < N; i++) {
    if (e == page1[i].time) return i;
  }
  return -1;
}

int Compfu(Pro *page1, int i, int t, Pro p[M]) {
  int count = 0;
  for (int j = i; j < M; j++) {
    if (page1[t].num == p[j].num)
      break;
    else
      count++;
  }
  return count;
}

int main() {
  Pro p[TOTAL_INSTRUCTION];
  Pro *page = new Pro[N];
  char c;
  int t = 0;
  float n = 0;

  Input(p);

  do {
    for (int i = 0; i < N; i++) {  // 初试化页面基本情况
      page[i].num = 0;
      page[i].time = 2 - i;
    }
    int i = 0;
    std::cout << "f: FIFO 页面置换" << std::endl;
    std::cout << "l: LRU 页面置换" << std::endl;
    std::cout << "o: OPT 页面置换" << std::endl;
    std::cout << "按其它键结束" << std::endl;
    std::cin >> c;

    if (c == 'f') {  // FIFO 页面置换
      n = 0;
      std::cout << "页面置换情况: " << std::endl;
      while (i < TOTAL_INSTRUCTION) {
        if (Search(p[i].num, page) >= 0)
          i++;  // 找到相同的页面
        else {
          if (t == N)
            t = 0;
          else {
            n++;
            page[t].num = p[i].num;
            print(page);
            t++;
          }
        }
      }
      std::cout << "缺页次数：" << n << " 缺页率：" << n / TOTAL_INSTRUCTION
                << std::endl;
    }

    if (c == 'l') {  // LRU 页面置换
      n = 0;
      std::cout << "页面置换情况: " << std::endl;
      while (i < TOTAL_INSTRUCTION) {
        int k;
        k = t = Search(p[i].num, page);
        if (t >= 0)
          page[t].time = 0;
        else {
          n++;
          t = Max(page);
          page[t].num = p[i].num;
          page[t].time = 0;
        }
        if (t == 0) {
          page[t + 1].time++;
          page[t + 2].time++;
        }
        if (t == 1) {
          page[2].time++;
          page[0].time++;
        }
        if (t == 2) {
          page[1].time++;
          page[0].time++;
        }
        if (k == -1) print(page);
        i++;
      }
      std::cout << "缺页次数：" << n << " 缺页率：" << n / TOTAL_INSTRUCTION
                << std::endl;
    }

    if (c == 'o') {  // OPT 页面置换
      n = 0;
      while (i < TOTAL_INSTRUCTION) {
        if (Search(p[i].num, page) >= 0)
          i++;
        else {
          int temp = 0, cn;
          for (t = 0; t < N; t++) {
            if (temp < Compfu(page, i, t, p)) {
              temp = Compfu(page, i, t, p);
              cn = t;
            }
          }
          page[cn] = p[i];
          n++;
          print(page);
          i++;
        }
      }
      std::cout << "缺页次数：" << n << " 缺页率：" << n / TOTAL_INSTRUCTION
                << std::endl;
    }
  } while (c == 'f' || c == 'l' || c == 'o');

  delete[] page;
  return 0;
}
