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
    double len;
    bool lc,rc;
};

struct Line
{
    int flag;
    double l,r,h;
    Line(){l=r=h=flag=0;}
    Line(double l,double r,double h,int flag){this->l=l,this->r=r,this->h=h,this->flag = flag;}
    bool operator < (const Line& a) const
    {
        return h==a.h ? flag>a.flag : h<a.h;
    }
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
