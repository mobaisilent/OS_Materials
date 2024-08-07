#include <iostream>
#include <iomanip>
#include <cmath> // 使用标准数学库
using namespace std;

const int MaxNumber = 100;
int TrackOrder[MaxNumber];   // 磁道号
int MoveDistance[MaxNumber]; // 移动距离
int FindOrder[MaxNumber];    // 寻找序列
double AverageDistance;      // 平均寻道长度
bool direction;              // 方向 true时为向外，false为向里
int BeginNum;                // 开始磁道号
int M = 500;                 // 磁道数
int N;                       // 提出磁盘I/O申请的进程数
int SortOrder[MaxNumber];    // 排序后的序列
bool Finished[MaxNumber];

void Inith()
{
    cout << "请输入提出磁盘I/O申请的进程数: ";
    cin >> N;
    cout << "请依次输入要访问的磁道号： ";
    for (int i = 0; i < N; i++)
        cin >> TrackOrder[i];
    for (int j = 0; j < N; j++)
        MoveDistance[j] = 0;
    cout << "请输入开始磁道号： ";
    cin >> BeginNum;
    for (int k = 0; k < N; k++)
        Finished[k] = false;
    for (int i = 0; i < N; i++)
        SortOrder[i] = TrackOrder[i];
}

//======排序函数，将各进程申请的磁道按从小到大排列======
void Sort()
{
    int temp;
    for (int i = N - 1; i >= 0; i--)
        for (int j = 0; j < i; j++)
        {
            if (SortOrder[j] > SortOrder[j + 1])
            {
                temp = SortOrder[j];
                SortOrder[j] = SortOrder[j + 1];
                SortOrder[j + 1] = temp;
            }
        }
}

//=====FCFS,先来先服务========
void FCFS()
{
    int temp;
    temp = BeginNum; // 将BeginNum赋给temp作为寻道时的当前所在磁道号
    for (int i = 0; i < N; i++)
    {
        MoveDistance[i] = abs(TrackOrder[i] - temp); // 计算移动磁道数
        temp = TrackOrder[i];                      // 寻到后，将此道作为当前所在磁道号，赋给temp
        FindOrder[i] = TrackOrder[i];              // 寻好的赋给寻好序列
    }
}

//====SSTF,最短寻道时间优先算法==========
void SSTF()
{
    int temp, n;
    int A = M;
    temp = BeginNum;            // 将BeginNum赋给temp作为寻道时的当前所在磁道号
    for (int i = 0; i < N; i++) // 寻找最短的寻道长度
    {
        for (int j = 0; j < N; j++)
        {
            if (abs(TrackOrder[j] - temp) < A && Finished[j] == false)
            {
                A = abs(TrackOrder[j] - temp);
                n = j;
            }
            else
                continue;
        }

        Finished[n] = true; // 将已经寻找到的Finished赋值为true

        MoveDistance[i] = A;          // 寻道长度
        temp = TrackOrder[n];         // 当前寻道号
        A = M;                        // 重置A值
        FindOrder[i] = TrackOrder[n]; //
    }
}

//=======SCAN,扫描算法============
void SCAN()
{
    int m, n, temp;
    temp = BeginNum;
    Sort();                                  // 排序
    cout << "请选择开始方向1-向外，0-向里:"; // 选择扫描方向
    cin >> m;
    if (m == 1)
        direction = true;
    else if (m == 0)
        direction = false;
    else
        cout << "输入错误!";
    for (int i = 0; i < N; i++)
    {
        if (SortOrder[i] < BeginNum)
            continue;
        else
        {
            n = i;
            break;
        }
    }
    if (direction == true) // 选择向外
    {
        for (int i = n; i < N; i++)
        {
            MoveDistance[i - n] = abs(SortOrder[i] - temp);
            temp = SortOrder[i];
            FindOrder[i - n] = SortOrder[i];
        }
        for (int j = n - 1; j >= 0; j--)
        {
            MoveDistance[N - 1 - j] = abs(SortOrder[j] - temp);
            temp = SortOrder[j];
            FindOrder[N - 1 - j] = SortOrder[j];
        }
    }
    else // 选择向里
    {
        for (int i = n - 1; i >= 0; i--)
        {
            MoveDistance[n - 1 - i] = abs(SortOrder[i] - temp);
            temp = SortOrder[i];
            FindOrder[n - 1 - i] = SortOrder[i];
        }
        for (int j = n; j < N; j++)
        {
            MoveDistance[j] = abs(SortOrder[j] - temp);
            temp = SortOrder[j];
            FindOrder[j] = SortOrder[j];
        }
    }
}

//========CSCAN,循环扫描算法=========
void CSCAN()
{
    int m, n, temp;
    temp = BeginNum;
    Sort();
    cout << "请选择开始方向：1--向外;0---向里: ";
    cin >> m;
    if (m == 1)
        direction = true;
    else if (m == 0)
        direction = false;
    else
        cout << "输入错误!";
    for (int i = 0; i < N; i++)
    {
        if (SortOrder[i] < BeginNum)
            continue;
        else
        {
            n = i;
            break;
        }
    }
    if (direction == true)
    {
        for (int i = n; i < N; i++)
        {
            MoveDistance[i - n] = abs(SortOrder[i] - temp);
            temp = SortOrder[i];
            FindOrder[i - n] = SortOrder[i];
        }
        for (int j = 0; j < n; j++)
        {
            MoveDistance[N - n + j] = abs(SortOrder[j] - temp);
            temp = SortOrder[j];
            FindOrder[N - n + j] = SortOrder[j];
        }
    }
    else
    {
        for (int i = n - 1; i >= 0; i--)
        {
            MoveDistance[n - 1 - i] = abs(SortOrder[i] - temp);
            temp = SortOrder[i];
            FindOrder[n - 1 - i] = SortOrder[i];
        }
        for (int j = N - 1; j >= n; j--)
        {
            MoveDistance[N - j + n - 1] = abs(SortOrder[j] - temp);
            temp = SortOrder[j];
            FindOrder[N - j + n - 1] = SortOrder[j];
        }
    }
}

//========计算平均寻道时间==============
void Count()
{
    int Total = 0;
    for (int i = 0; i < N; i++)
    {
        Total += MoveDistance[i];
    }
    AverageDistance = ((double)Total) / ((double)N);
}

void Show()
{
    cout << setw(20) << "被访问的下一个磁道号" << setw(20) << "移动距离（磁道数)" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << setw(15) << FindOrder[i] << setw(15) << MoveDistance[i] << endl;
    }

    cout << setw(20) << "平均寻道长度：" << AverageDistance << endl;
    cout << endl;
}

int main()
{
    int y = 1;
    int s;
    Inith();
    while (y)
    {
        cout << "请选择寻道方式：1-FCFS，2-SSTF，3-SCAN，4-CSCAN:";
        cin >> s;
        switch (s)
        {
        case 1:
            FCFS();
            Count();
            Show();
            break;
        case 2:
            SSTF();
            Count();
            Show();
            break;
        case 3:
            SCAN();
            Count();
            Show();
            break;
        case 4:
            CSCAN();
            Count();
            Show();
            break;
        }
        cout << "是否继续选择寻道算法？1--是，0--否 :";
        int p;
        cin >> p;
        y = p;
        for (int i = 0; i < MaxNumber; i++)
        {
            Finished[i] = false;
        }
    }
    return 0;
}
