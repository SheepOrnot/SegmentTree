#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
#define _USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<iostream>
#include<algorithm>
const int MAXN = 1e3+10;

struct Node
{
    int l,r,cov,cnt;
    // 记录左右范围，被覆盖了几次，投影连续段数
    double len;
    // 记录被覆盖长度
    bool lc,rc;
    // 记录左端点是否被覆盖
};

struct Line
{
    int flag;
    // 记录该边待加还是待删
    double l,r,h;
    Line(){l=r=h=flag=0;}
    Line(double l,double r,double h,int flag){this->l=l,this->r=r,this->h=h,this->flag = flag;}
    bool operator < (const Line& a) const
    {
        return h==a.h ? flag>a.flag : h<a.h;
    }
    //让线段总体按y升序，局部先加后待删
};

class SegmentTree
{
public:
    int n,m;
    SegmentTree();
    std::pair<double,double> Calc(int InputN,double X1[],double Y1[],double X2[],double Y2[]);
    void ergodic(int x,int l,int r);
//private:
    double X[MAXN<<1];
    Node tree[MAXN<<2];
    Line line[MAXN<<1];
protected:
    void push_up(int x);
    void build(int x,int l,int r);
    void update(int x,double L,double R,int flag);
};

#endif // SEGMENTTREE_H
