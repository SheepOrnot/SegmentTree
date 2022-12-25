#include "segmenttree.h"

SegmentTree::SegmentTree()
{

}

void SegmentTree::push_up(int x)
// 更新tag
{
    if(tree[x].cov)
    {
        tree[x].len = X[tree[x].r+1]-X[tree[x].l];
        tree[x].lc = tree[x].rc = 1;
        tree[x].cnt = 1;
    }
    else
    {
        tree[x].len = tree[x<<1].len+tree[x<<1|1].len;
        tree[x].lc = tree[x<<1].lc;
        tree[x].rc = tree[x<<1|1].rc;
        tree[x].cnt = tree[x<<1].cnt+tree[x<<1|1].cnt;
        tree[x].cnt -= (tree[x<<1].rc&tree[x<<1|1].lc);
    }
}

void SegmentTree::build(int x,int l,int r)
// 建树加初始化
{
    tree[x].l = l, tree[x].r = r;
    tree[x].cnt = tree[x].len = tree[x].cov = 0; tree[x].lc = tree[x].rc = 0;
    if(l==r) return;
    int mid = (l+r)>>1;
    build(x<<1,l,mid);
    build(x<<1|1,mid+1,r);
    push_up(x);
}

void SegmentTree::update(int x,double L,double R,int flag)
// 遍历时调用的更新函数
{
    if(X[tree[x].r+1]<=L||X[tree[x].l]>=R) return;
    if(X[tree[x].l]>=L&&X[tree[x].r+1]<=R)
    {
        tree[x].cov += flag;
        push_up(x); return;
    }
    update(x<<1,L,R,flag);
    update(x<<1|1,L,R,flag);
    push_up(x);
}

std::pair<double,double> SegmentTree::Calc(int InputN,double X1[],double Y1[],double X2[],double Y2[])
{
    n = InputN;
    for(int i = 1; i <= n; ++i)
    {
        if(X1[i]>X2[i]) std::swap(X1[i],X2[i]);
        if(Y1[i]>Y2[i]) std::swap(Y1[i],Y2[i]);
        line[(i<<1)] = Line(X1[i],X2[i],Y1[i],1);
        line[(i<<1)-1] = Line(X1[i],X2[i],Y2[i],-1);
        X[(i<<1)] = X1[i], X[(i<<1)-1] = X2[i];
    }
    n <<= 1;
    std::sort(X+1,X+n+1);
    std::sort(line+1,line+n+1);
    m = std::unique(X+1,X+n+1)-X-1;
    // 离散化横坐标，便于建树和计算
    build(1,1,m-1);
    // 注意是1到m-1，每个节点表示范围为l~r+1，这样l=r的叶子节点也能表示区间，并且解决了上下限的问题
    double Area = 0,Perimeter = 0,pre = 0;
    for(int i = 1; i < n; ++i)
    {
        update(1,line[i].l,line[i].r,line[i].flag);
        Area += (tree[1].len*(line[i+1].h-line[i].h));
        // 面积为扫描线投影*统计区间宽
        Perimeter += std::max(tree[1].len,pre)-std::min(tree[1].len,pre);
        // 横边（相对于扫描线）长度求和，新增量为投影新增或新减量
        pre = tree[1].len;
        Perimeter += 2*(tree[1].cnt*(line[i+1].h-line[i].h));
        // 竖边（相对于扫描线）长度求和，新增量为统计区间宽*投影连续段数*2
    }
    Perimeter += (line[n].r-line[n].l);
    return std::pair<double,double>(Area,Perimeter);
}

void SegmentTree::ergodic(int x,int l,int r)
{
    //返回你想要的值
    //线段左端点X[tree[x].l]，右端点X[tree[x].r+1]
    if(tree[x].l==tree[x].r){return;}
    int mid = (l+r)>>1;
    ergodic(x<<1,l,mid);
    ergodic(x<<1|1,mid+1,r);
}
