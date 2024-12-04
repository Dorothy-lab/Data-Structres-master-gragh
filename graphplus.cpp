// 5-3-2.cpp �������ǵ���ϵͳ
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

#define INFINITY INT_MAX  // ���ֵ��
#define MAX_VERTEX_NUM 20 // ��󶥵����
typedef enum
{
    UDG, // ����ͼ��UnDirected Graph��
    DG,  // ����ͼ��Directed Graph��
    UDN, // ��������UnDirected Network��
    DN   // ��������Directed Network��
} GraphKind;
typedef char *VertexType; // VertexType�Ƕ������ͣ�Ϊ�˱�ʾ��������Ϣ����������ַ���
typedef int VRType;       // VRType�Ƕ����ϵ����
// ��ͼ����1��0��ʾ�Ƿ�����
// ��������ΪȨֵ����

typedef struct
{                                                // ͼ�Ķ���
    VertexType vexs[MAX_VERTEX_NUM];             // ������Ϣ
    VRType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // �ڽ���
    int vexnum, arcnum;                          // �������ͱ���
    GraphKind kind;                              // ͼ�������־
} MGraph;

VertexType vexs[20] = {"����", "����", "����", "�ɶ�", "����", "���", "����", "֣��", "�人", "����", "�ϲ�", "����", "�Ϻ�", "����", "����", "����", "����", "����", "����"};
VertexType arcs[23][2] = {
    {"����", "����"},
    {"����", "����"},
    {"����", "֣��"},
    {"֣��", "����"},
    {"����", "���"},
    {"���", "����"},
    {"֣��", "����"},
    {"����", "�ɶ�"},
    {"֣��", "�人"},
    {"����", "�Ϻ�"},
    {"�ɶ�", "����"},
    {"�ɶ�", "����"},
    {"�人", "����"},
    {"�Ϻ�", "�ϲ�"},
    {"����", "����"},
    {"����", "����"},
    {"����", "�ϲ�"},
    {"�ϲ�", "����"},
    {"����", "����"},
    {"����", "����"},
    {"����", "����"},
    {"����", "����"},
    {"����", "����"}
};
int d[23] = {216, 676, 511, 695, 137, 674, 349, 842, 534, 651, 1100, 967, 409, 825, 639, 902, 367, 622, 607, 672, 675, 255, 140};

VertexType GetVex(MGraph G, int v)
// ��ȡָ������ֵ�Ĳ�����v����֪ͼG��ĳ������ţ����ض���v��ֵ
{
    if (v < 0 || v >= G.vexnum)
        exit(0);
    return G.vexs[v];
}

int LocateVex(MGraph G, VertexType u)
// ���㶨λ����������֪ͼG�в���ָ���Ķ���u����G�д��ڶ���u���򷵻ظö�����ͼ��λ�ã����򷵻ء��ա�
{
    int i;
    for (i = 0; i < G.vexnum; i++)
        if (strcmp(G.vexs[i], u) == 0)
            return i;
    return -1;
}

int FirstAdjVex(MGraph G, int v)
// ���׸��ڽӵ�Ĳ�����v����֪ͼG��ĳ�����㣬����v���׸��ڽӵ㣬��������G��û���ڽӵ㣬�򷵻ء��ա�
{
    if (v < 0 || v >= G.vexnum)
        return -1;
    for (int j = 0; j < G.vexnum; j++) // �����ڽ����v��
        if (G.arcs[v][j] != 0 && G.arcs[v][j] < INFINITY)
            return j;
    return -1;
}

int NextAdjVex(MGraph G, int v, int w)
// ���¸��ڽӵ�Ĳ�����v����֪ͼG��ĳ�����㣬w��v���ڽӵ㣬����v�ģ������w�ģ��¸��ڽӵ㡣��w��v�����һ���ڽӵ㣬�򷵻ء��ա�
{
    if (v < 0 || v >= G.vexnum)
        return -1;
    for (int j = w + 1; j < G.vexnum; j++) // �����ڽ����v��
        if (G.arcs[v][j] != 0 && G.arcs[v][j] < INFINITY)
            return j;
    return -1;
}

Status CreateUDG(MGraph &G)
{ // �������飨�ڽ��󣩱�ʾ������������ͼG
    int i, j, k;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    printf("���붥����G.vexnum: ");
    scanf("%d", &G.vexnum);
    printf("�������G.arcnum: ");
    scanf("%d", &G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        printf("���붥��G.vexs[%d]: ", i);
        G.vexs[i] = (char *)malloc(20);
        scanf("%s", G.vexs[i]);
    } // ���춥������
    for (i = 0; i < G.vexnum; i++) // ��ʼ���ڽӾ���
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = 0;
    for (k = 0; k < G.arcnum; k++)
    { // �����ڽӾ���
        printf("�����%d����(vi vj)���ÿո������: ", k + 1);
        scanf("%s %s", v1, v2); // ����һ���������Ķ���
        i = LocateVex(G, v1);
        j = LocateVex(G, v2);        // ȷ��v1��v2��G��λ��
        G.arcs[i][j] = 1;            // ��(v1,v2)
        G.arcs[j][i] = G.arcs[i][j]; // ��(v1,v2)�ĶԳƱ�(v2,v1)
    }
    return OK;
}

Status CreateDG(MGraph &G)
{ // �������飨�ڽ��󣩱�ʾ������������ͼG
    int i, j, k;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    printf("���붥����G.vexnum: ");
    scanf("%d", &G.vexnum);
    printf("�������G.arcnum: ");
    scanf("%d", &G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        printf("���붥��G.vexs[%d]: ", i);
        G.vexs[i] = (char *)malloc(20);
        scanf("%s", G.vexs[i]);
    } // ���춥������
    for (i = 0; i < G.vexnum; i++) // ��ʼ���ڽӾ���
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = 0;
    for (k = 0; k < G.arcnum; k++)
    { // �����ڽӾ���
        printf("�����%d���ߣ��ÿո������: ", k + 1);
        scanf("%s %s", v1, v2); // ����һ���������Ķ���
        i = LocateVex(G, v1);
        j = LocateVex(G, v2); // ȷ��v1��v2��G��λ��
        G.arcs[i][j] = 1;     // ��
    }
    return OK;
} // CreateDG

// Status CreateUDN(MGraph &G)
// { // �������飨�ڽ��󣩱�ʾ��������������G
//     int i, j, k, w;
//     VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
//     printf("���붥����G.vexnum: ");
//     scanf("%d", &G.vexnum);
//     printf("�������G.arcnum: ");
//     scanf("%d", &G.arcnum);
//     for (i = 0; i < G.vexnum; i++)
//     {
//         printf("���붥��G.vexs[%d]: ", i);
//         G.vexs[i] = (char *)malloc(20);
//         scanf("%s", G.vexs[i]);
//     } // ���춥������
//     for (i = 0; i < G.vexnum; i++) // ��ʼ���ڽӾ���
//         for (j = 0; j < G.vexnum; j++)
//             G.arcs[i][j] = INFINITY;
//     for (k = 0; k < G.arcnum; k++)
//     { // �����ڽӾ���
//         printf("�����%d����vi��vj��Ȩֵw ���ÿո������: ", k + 1);
//         scanf("%s %s %d", v1, v2, &w); // ����һ���������Ķ��㼰Ȩֵ
//         i = LocateVex(G, v1);
//         j = LocateVex(G, v2);        // ȷ��v1��v2��G��λ��
//         G.arcs[i][j] = w;            // ��(v1,v2)��Ȩֵ
//         G.arcs[j][i] = G.arcs[i][j]; // ��(v1,v2)�ĶԳƱ�(v2,v1)
//     }
//     return OK;
// } // CreateUDN

Status CreateUDN(MGraph &G)
{ // �������飨�ڽ��󣩱�ʾ��������������G���ģ�
    int i, j, k, w;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    G.vexnum = 19;
    G.arcnum = 23;
    for (i = 0; i < G.vexnum; i++)
    {
        G.vexs[i] = (char *)malloc(20);
        G.vexs[i] = vexs[i];
    }
    for (i = 0; i < G.vexnum; i++) // ��ʼ���ڽӾ���
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = INFINITY;
    for (k = 0; k < G.arcnum; k++)
    { // �����ڽӾ���
        v1=arcs[k][0];
        v2=arcs[k][1];
        w=d[k];
        i = LocateVex(G, v1);
        j = LocateVex(G, v2);        // ȷ��v1��v2��G��λ��
        G.arcs[i][j] = w;            // ��(v1,v2)��Ȩֵ
        G.arcs[j][i] = G.arcs[i][j]; // ��(v1,v2)�ĶԳƱ�(v2,v1)
    }
    return OK;
} // CreateUDN

Status CreateDN(MGraph &G)
{ // �������飨�ڽ��󣩱�ʾ��������������G
    int i, j, k, w;
    VertexType v1 = (char *)malloc(20), v2 = (char *)malloc(20);
    printf("���붥����G.vexnum: ");
    scanf("%d", &G.vexnum);
    printf("�������G.arcnum: ");
    scanf("%d", &G.arcnum);
    for (i = 0; i < G.vexnum; i++)
    {
        printf("���붥��G.vexs[%d]: ", i);
        G.vexs[i] = (char *)malloc(20);
        scanf("%s", G.vexs[i]);
    } // ���춥������
    for (i = 0; i < G.vexnum; i++) // ��ʼ���ڽӾ���
        for (j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = INFINITY;
    for (k = 0; k < G.arcnum; k++)
    { // �����ڽӾ���
        printf("�����%d����vi��vj��Ȩֵw���ÿո������: ", k + 1);
        scanf("%s %s %d", v1, v2, &w); // ����һ���������Ķ��㼰Ȩֵ
        i = LocateVex(G, v1);
        j = LocateVex(G, v2); // ȷ��v1��v2��G��λ��
        G.arcs[i][j] = w;     // ����Ȩֵ
    }
    return OK;
} // CreateDN

Status CreateGraph(MGraph &G)
// ��������(�ڽӾ���)��ʾ��������ͼG
{
    printf("������ͼ������:0��ʾ����ͼUDG��1��ʾ����ͼDG��2��ʾ������UDN��3��ʾ������DN ");
    scanf("%d", &G.kind); // �Զ������뺯��������һ�����ֵ
    switch (G.kind)
    {
    case UDG:
        return CreateUDG(G); // ��������ͼG
    case DG:
        return CreateDG(G); // ��������ͼG
    case UDN:
        return CreateUDN(G); // ����������G
    case DN:
        return CreateDN(G); // ����������G
    default:
        return ERROR;
    }
} // CreateGraph

void ShortestPath_DIJ(MGraph G, int start, int end, int *prev, int *dist)
// prevΪǰ���������飬��prev[i]��ֵ�ǡ�����vs����������i�������·����������ȫ��������
// λ��"����i"֮ǰ���Ǹ����㡣dist -- �������顣����dist[i]��"����vs"��"����i"�����·���ĳ���
{
    int i, j, k, min, tmp;
    // flag[i]=1��ʾ"����v"��"����i"�����·���ѳɹ���ȡ��
    bool flag[MAX_VERTEX_NUM];
    // ��ʼ��
    for (i = 0; i < G.vexnum; i++)
    {
        flag[i] = FALSE;            // ����i�����·����û��ȡ��
        prev[i] = start;            // ����i��ǰ������Ϊ0
        dist[i] = G.arcs[start][i]; // ����i�����·��Ϊ������v����������i����Ȩ
    }
    // ��"����v"������г�ʼ��
    flag[start] = 1;
    dist[start] = 0;
    // ����G.vexnum-1�Σ�ÿ���ҳ�һ����������·��
    for (i = 1; i < G.vexnum; i++)
    {
        // Ѱ�ҵ�ǰ��С��·��
        // ������δ��ȡ���·���Ķ����У��ҵ���vs����Ķ���(k)
        min = INFINITY;
        for (j = 0; j < G.vexnum; j++)
        {
            if (!flag[j] && dist[j] < min)
            {
                min = dist[j];
                k = j;
            }
        }
        // ��ǡ�����k��Ϊ�Ѿ���ȡ�����·��
        flag[k] = TRUE;
        // ������ǰ���·����ǰ������
        // ���Ѿ�������k�����·����֮�󣬸��¡�δ��ȡ���·���Ķ�������·����ǰ�����㡱
        for (j = 0; j < G.vexnum; j++)
        { // ��ֹ���
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
{ // ��ӡdijkstra���·���Ľ��
    int cnt = 0, i = end;
    int next[MAX_VERTEX_NUM];
    while (i != start)
    {
        next[cnt++] = i;
        i = prev[i];
    }
    next[cnt++] = start;
    printf("���·���ǣ�");
    for (i = cnt - 1; i > 0; i--)
        printf("%s->", G.vexs[next[i]]);
    printf("%s ", G.vexs[next[0]]);
    printf("���·�������ǣ�%d", dist[end]);
}


int main()
{
    MGraph G;
    char sstart[20], send[20];
    int start, end;
    CreateUDN(G);
    int prev[MAX_VERTEX_NUM], dist[MAX_VERTEX_NUM];
    printf("�����������յ㣺");
    scanf("%s %s", sstart, send);
    start = LocateVex(G, sstart);
    end = LocateVex(G, send); // ȷ��v1��v2��G��λ��
    ShortestPath_DIJ(G, start, end, prev, dist);
    DisplayShortestPath(G, start, end, prev, dist);
    system("pause");
    return 0;
}