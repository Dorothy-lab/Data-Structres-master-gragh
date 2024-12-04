// 5-3-2.cpp 北斗卫星导航系统
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR 0
#define OK 1
#define OVERFLOW -2
#define FALSE 0
#define TRUE 1
typedef int Status;

#define INFINITY INT_MAX  // 最大值∞
#define MAX_VERTEX_NUM 20 // 最大顶点个数
typedef enum
{
    UDG, // 无向图（UnDirected Graph）
    DG,  // 有向图（Directed Graph）
    UDN, // 无向网（UnDirected Network）
    DN   // 有向网（Directed Network）
} GraphKind;
typedef char *VertexType; // VertexType是顶点类型，为了表示地名等信息，这里采用字符串
typedef int VRType;       // VRType是顶点关系类型
// 对图，用1或0表示是否相邻
// 对网，则为权值类型

typedef struct
{                                                // 图的定义
    VertexType vexs[MAX_VERTEX_NUM];             // 顶点信息
    VRType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 邻接阵
    int vexnum, arcnum;                          // 顶点数和边数
    GraphKind kind;                              // 图的种类标志
} MGraph;

VertexType vexs[20] = {"西宁", "兰州", "西安", "成都", "北京", "天津", "徐州", "郑州", "武汉", "株洲", "南昌", "福州", "上海", "广州", "深圳", "昆明", "贵阳", "柳州", "南宁"};
VertexType arcs[23][2] = {
    {"西宁", "兰州"},
    {"兰州", "西安"},
    {"西安", "郑州"},
    {"郑州", "北京"},
    {"北京", "天津"},
    {"天津", "徐州"},
    {"郑州", "徐州"},
    {"西安", "成都"},
    {"郑州", "武汉"},
    {"徐州", "上海"},
    {"成都", "昆明"},
    {"成都", "贵阳"},
    {"武汉", "株洲"},
    {"上海", "南昌"},
    {"昆明", "贵阳"},
    {"贵阳", "株洲"},
    {"株洲", "南昌"},
    {"南昌", "福州"},
    {"贵阳", "柳州"},
    {"株洲", "柳州"},
    {"株洲", "广州"},
    {"南宁", "柳州"},
    {"广州", "深圳"}
};
int d[23] = {216, 676, 511, 695, 137, 674, 349, 842, 534, 651, 1100, 967, 409, 825, 639, 902, 367, 622, 607, 672, 675, 255, 140};

VertexType GetVex(MGraph G, int v)
// 获取指定顶点值的操作，v是已知图G的某个顶点号，返回顶点v的值
{
    if (v < 0 || v >= G.vexnum)
        exit(0);
    return G.vexs[v];
}

int LocateVex(MGraph G, VertexType u)
// 顶点定位操作，在已知图G中查找指定的顶点u，若G中存在顶点u，则返回该顶点在图中位置；否则返回“空”
{
    int i;
    for (i = 0; i < G.vexnum; i++)
        if (strcmp(G.vexs[i], u) == 0)
            return i;
    return -1;
}

int FirstAdjVex(MGraph G, int v)
// 求首个邻接点的操作，v是已知图G的某个顶点，返回v的首个邻接点，若顶点在G中没有邻接点，则返回“空”
{
    if (v < 0 || v >= G.vexnum)
        return -1;
    for (int j = 0; j < G.vexnum; j++) // 遍历邻接阵第v行
        if (G.arcs[v][j] != 0 && G.arcs[v][j] < INFINITY)
            return j;
    return -1;
}

int NextAdjVex(MGraph G, int v, int w)
// 求下个邻接点的操作，v是已知图G的某个顶点，w是v的邻接点，返回v的（相对于w的）下个邻接点。若w是v的最后一个邻接点，则返回“空”
{
    if (v < 0 || v >= G.vexnum)
        return -1;
    for (int j = w + 1; j < G.vexnum; j++) // 遍历邻接阵第v行
        if (G.arcs[v][j] != 0 && G.arcs[v][j] < INFINITY)
            return j;
    return -1;
}

Status CreateUDG(MGraph &G)
{ // 采用数组（邻接阵）表示法，构造无向图G
    int i, j, k;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    printf("输入顶点数G.vexnum: ");
    scanf("%d", &G.vexnum);
    printf("输入边数G.arcnum: ");
    scanf("%d", &G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        printf("输入顶点G.vexs[%d]: ", i);
        G.vexs[i] = (char *)malloc(20);
        scanf("%s", G.vexs[i]);
    } // 构造顶点向量
    for (i = 0; i < G.vexnum; i++) // 初始化邻接矩阵
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = 0;
    for (k = 0; k < G.arcnum; k++)
    { // 构造邻接矩阵
        printf("输入第%d条边(vi vj)（用空格隔开）: ", k + 1);
        scanf("%s %s", v1, v2); // 输入一条边依附的顶点
        i = LocateVex(G, v1);
        j = LocateVex(G, v2);        // 确定v1和v2在G中位置
        G.arcs[i][j] = 1;            // 边(v1,v2)
        G.arcs[j][i] = G.arcs[i][j]; // 置(v1,v2)的对称边(v2,v1)
    }
    return OK;
}

Status CreateDG(MGraph &G)
{ // 采用数组（邻接阵）表示法，构造有向图G
    int i, j, k;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    printf("输入顶点数G.vexnum: ");
    scanf("%d", &G.vexnum);
    printf("输入边数G.arcnum: ");
    scanf("%d", &G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        printf("输入顶点G.vexs[%d]: ", i);
        G.vexs[i] = (char *)malloc(20);
        scanf("%s", G.vexs[i]);
    } // 构造顶点向量
    for (i = 0; i < G.vexnum; i++) // 初始化邻接矩阵
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = 0;
    for (k = 0; k < G.arcnum; k++)
    { // 构造邻接矩阵
        printf("输入第%d条边（用空格隔开）: ", k + 1);
        scanf("%s %s", v1, v2); // 输入一条边依附的顶点
        i = LocateVex(G, v1);
        j = LocateVex(G, v2); // 确定v1和v2在G中位置
        G.arcs[i][j] = 1;     // 弧
    }
    return OK;
} // CreateDG

// Status CreateUDN(MGraph &G)
// { // 采用数组（邻接阵）表示法，构造无向网G
//     int i, j, k, w;
//     VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
//     printf("输入顶点数G.vexnum: ");
//     scanf("%d", &G.vexnum);
//     printf("输入边数G.arcnum: ");
//     scanf("%d", &G.arcnum);
//     for (i = 0; i < G.vexnum; i++)
//     {
//         printf("输入顶点G.vexs[%d]: ", i);
//         G.vexs[i] = (char *)malloc(20);
//         scanf("%s", G.vexs[i]);
//     } // 构造顶点向量
//     for (i = 0; i < G.vexnum; i++) // 初始化邻接矩阵
//         for (j = 0; j < G.vexnum; j++)
//             G.arcs[i][j] = INFINITY;
//     for (k = 0; k < G.arcnum; k++)
//     { // 构造邻接矩阵
//         printf("输入第%d条边vi、vj和权值w （用空格隔开）: ", k + 1);
//         scanf("%s %s %d", v1, v2, &w); // 输入一条边依附的顶点及权值
//         i = LocateVex(G, v1);
//         j = LocateVex(G, v2);        // 确定v1和v2在G中位置
//         G.arcs[i][j] = w;            // 边(v1,v2)的权值
//         G.arcs[j][i] = G.arcs[i][j]; // 置(v1,v2)的对称边(v2,v1)
//     }
//     return OK;
// } // CreateUDN

Status CreateUDN(MGraph &G)
{ // 采用数组（邻接阵）表示法，构造无向网G（改）
    int i, j, k, w;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    G.vexnum = 19;
    G.arcnum = 23;
    for (i = 0; i < G.vexnum; i++)
    {
        G.vexs[i] = (char *)malloc(20);
        G.vexs[i] = vexs[i];
    }
    for (i = 0; i < G.vexnum; i++) // 初始化邻接矩阵
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = INFINITY;
    for (k = 0; k < G.arcnum; k++)
    { // 构造邻接矩阵
        v1=arcs[k][0];
        v2=arcs[k][1];
        w=d[k];
        i = LocateVex(G, v1);
        j = LocateVex(G, v2);        // 确定v1和v2在G中位置
        G.arcs[i][j] = w;            // 边(v1,v2)的权值
        G.arcs[j][i] = G.arcs[i][j]; // 置(v1,v2)的对称边(v2,v1)
    }
    return OK;
} // CreateUDN

Status CreateDN(MGraph &G)
{ // 采用数组（邻接阵）表示法，构造有向网G
    int i, j, k, w;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    printf("输入顶点数G.vexnum: ");
    scanf("%d", &G.vexnum);
    printf("输入边数G.arcnum: ");
    scanf("%d", &G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        printf("输入顶点G.vexs[%d]: ", i);
        G.vexs[i] = (char *)malloc(20);
        scanf("%s", G.vexs[i]);
    } // 构造顶点向量
    for (i = 0; i < G.vexnum; i++) // 初始化邻接矩阵
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = INFINITY;
    for (k = 0; k < G.arcnum; k++)
    { // 构造邻接矩阵
        printf("输入第%d条边vi、vj和权值w（用空格隔开）: ", k + 1);
        scanf("%s %s %d", v1, v2, &w); // 输入一条边依附的顶点及权值
        i = LocateVex(G, v1);
        j = LocateVex(G, v2); // 确定v1和v2在G中位置
        G.arcs[i][j] = w;     // 弧的权值
    }
    return OK;
} // CreateDN

Status CreateGraph(MGraph &G)
// 采用数组(邻接矩阵)表示法，构造图G
{
    printf("请输入图的种类:0表示无向图UDG，1表示有向图DG，2表示无向网UDN，3表示有向网DN ");
    scanf("%d", &G.kind); // 自定义输入函数，读入一个随机值
    switch (G.kind)
    {
    case UDG:
        return CreateUDG(G); // 构造无向图G
    case DG:
        return CreateDG(G); // 构造有向图G
    case UDN:
        return CreateUDN(G); // 构造无向网G
    case DN:
        return CreateDN(G); // 构造有向网G
    default:
        return ERROR;
    }
} // CreateGraph

void ShortestPath_DIJ(MGraph G, int start, int end, int *prev, int *dist)
// prev为前驱顶点数组，即prev[i]的值是“顶点vs”到“顶点i”的最短路径所经历的全部顶点中
// 位于"顶点i"之前的那个顶点。dist -- 长度数组。即，dist[i]是"顶点vs"到"顶点i"的最短路径的长度
{
    int i, j, k, min, tmp;
    // flag[i]=1表示"顶点v"到"顶点i"的最短路径已成功获取。
    bool flag[MAX_VERTEX_NUM];
    // 初始化
    for (i = 0; i < G.vexnum; i++)
    {
        flag[i] = FALSE;            // 顶点i的最短路径还没获取到
        prev[i] = start;            // 顶点i的前驱顶点为0
        dist[i] = G.arcs[start][i]; // 顶点i的最短路径为“顶点v”到“顶点i”的权
    }
    // 对"顶点v"自身进行初始化
    flag[start] = 1;
    dist[start] = 0;
    // 遍历G.vexnum-1次；每次找出一个顶点的最短路径
    for (i = 1; i < G.vexnum; i++)
    {
        // 寻找当前最小的路径
        // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)
        min = INFINITY;
        for (j = 0; j < G.vexnum; j++)
        {
            if (!flag[j] && dist[j] < min)
            {
                min = dist[j];
                k = j;
            }
        }
        // 标记“顶点k”为已经获取到最短路径
        flag[k] = TRUE;
        // 修正当前最短路径和前驱顶点
        // 当已经“顶点k的最短路径”之后，更新“未获取最短路径的顶点的最短路径和前驱顶点”
        for (j = 0; j < G.vexnum; j++)
        { // 防止溢出
            tmp = (G.arcs[k][j] == INFINITY ? INFINITY : (min + G.arcs[k][j]));
            if (!flag[j] && (tmp < dist[j]))
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }
}

void DisplayShortestPath(MGraph G, int start, int end, int *prev, int *dist)
{ // 打印dijkstra最短路径的结果
    int cnt = 0, i = end;
    int next[MAX_VERTEX_NUM];
    while (i != start)
    {
        next[cnt++] = i;
        i = prev[i];
    }
    next[cnt++] = start;
    printf("最短路径是：");
    for (i = cnt - 1; i > 0; i--)
        printf("%s->", G.vexs[next[i]]);
    printf("%s ", G.vexs[next[0]]);
    printf("最短路径长度是：%d", dist[end]);
}


int main()
{
    MGraph G;
    char sstart[20], send[20];
    int start, end;
    CreateUDN(G);
    int prev[MAX_VERTEX_NUM], dist[MAX_VERTEX_NUM];
    printf("请输入起点和终点：");
    scanf("%s %s", sstart, send);
    start = LocateVex(G, sstart);
    end = LocateVex(G, send); // 确定v1和v2在G中位置
    ShortestPath_DIJ(G, start, end, prev, dist);
    DisplayShortestPath(G, start, end, prev, dist);
    system("pause");
    return 0;
}