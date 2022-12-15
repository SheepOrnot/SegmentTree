#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
#define _USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<iostream>
#include<algorithm>
const int MAXN = 1e6+10;

struct Node
{
    int l,r,cov;
    double len,cnt;
    bool lc,rc;
};

struct Line
{
    int flag;
    double l,r,h;
    Line(){l=r=h=flag=0;}
    Line(double l,double r,double h,int flag):l(l),r(r),h(h),flag(flag){}
    bool operator < (const Line& a) const
    {
        return h==a.h ? flag>a.flag : h<a.h;
    }
};

class SegmentTree
{
public:
    SegmentTree();
    std::pair<double,double> Calc(int InputN);
private:
    int n;
    double X[MAXN<<1];
    Node tree[MAXN<<2];
    Line line[MAXN<<1];
protected:
    void push_up(int x);
    void build(int x,int l,int r);
    void update(int x,int L,int R,int flag);
};

#endif // SEGMENTTREE_H
