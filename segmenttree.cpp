#include "segmenttree.h"

SegmentTree::SegmentTree()
{

}

void SegmentTree::push_up(int x)
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
{
    tree[x].l = l, tree[x].r = r;
    if(l==r){tree[x].cnt = tree[x].len = tree[x].cov = 0; tree[x].lc = tree[x].rc = 0;return;}
    int mid = (l+r)>>1;
    build(x<<1,l,mid);
    build(x<<1|1,mid+1,r);
    push_up(x);
}

void SegmentTree::update(int x,int L,int R,int flag)
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
