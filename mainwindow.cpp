#include "mainwindow.h"
#include "ui_mainwindow.h"
<<<<<<< Updated upstream
#include<qpushbutton.h>
=======


drawnode::drawnode(Node *node, drawnode *_fa)
{
    this->treenode = node;
    this->x = this->y = 0;
    this->fa = _fa;
    this->fontColor = Qt::black;
    this->lineColor = Qt::black;
    this->vectorColor = Qt::white;
}


//比例尺 10:1
>>>>>>> Stashed changes
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
<<<<<<< Updated upstream
    this->resize(1600,900);
    ui->martixnum->setPlaceholderText(" 矩阵个数 ");
    ui->x1->setPlaceholderText("x1");
    ui->x2->setPlaceholderText("x2");
    ui->y1->setPlaceholderText("y1");
    ui->y2->setPlaceholderText("y2");
=======

    Rscene = new QGraphicsScene(0, 0, ui->RgraphicsView->width()-10, ui->RgraphicsView->height()-10);
    Tscene = new QGraphicsScene(0, 0, ui->TgraphicsView->width()-10, ui->TgraphicsView->height()-10);
    ui->RgraphicsView->setScene(Rscene);
    ui->TgraphicsView->setScene(Tscene);
    RsceneWidth  = Rscene->width();
    RsceneHeight = Rscene->height();
    TsceneWidth  = Tscene->width();
    TsceneHeight = Tscene->height();

    segtree = new SegmentTree;

    test();
>>>>>>> Stashed changes
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::node2graph(drawnode* node)
{
    QGraphicsRectItem *item = new QGraphicsRectItem(node->x,node->y, vectorWidth, vectorHeight);
    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setZValue(2);
    item->setBrush(QBrush(node->vectorColor));
    Tscene->addItem(item);

    //QGraphicsTextItem *textitem = new QGraphicsTextItem(node->treenode->s, item);
    QGraphicsTextItem *textitem = new QGraphicsTextItem("test", item);
    textitem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    textitem->setX(node->x);
    textitem->setY(node->y);
    textitem->setZValue(3);
    textitem->setDefaultTextColor(node->fontColor);
    Tscene->addItem(textitem);


    if(node->fa == NULL) return;
    else
    {
        QGraphicsLineItem *lineitem = new QGraphicsLineItem(node->x + vectorWidth / 2, node->y + vectorHeight / 2, node->fa->x + vectorWidth / 2, node->fa->y + vectorHeight / 2);
        lineitem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        lineitem->setZValue(1);
        lineitem->setPen(QPen(node->lineColor));
        Tscene->addItem(lineitem);
    }

    Tscene->clearSelection();
    //item->setSelected(true);
    //qDebug() << " 成功插入节点 " << node->treenode->s << " 于( " << node->x << " , " << node->y << ")";
}

void MainWindow::drawNode()
{
    Tscene->clear();
    //if(!alreadyGen) genDrawNode();

    qDebug() << " 开始绘制 ";
    for(int dep = maxDep; dep >= 1; dep --)
    {
        for(int i = 1; i <= layer[dep]; i ++ )
        {
            drawnode *current = drawNodeData[dep][i];
            //if(istarget[current->treenode->id])
            if(0)
            {
                current->fontColor = Qt::white;
                current->lineColor = Qt::blue;
                current->vectorColor = Qt::red;
            }
            else
            {
                current->fontColor = Qt::black;
                current->lineColor = Qt::black;
                current->vectorColor = Qt::white;
            }
            node2graph(current);
        }
    }

    //for node in drawNodeData[1~allDep-1][1~layer[dep]]
        //node2graph(node)
        //画椭圆 node.x, node.y
        //写文字 node.x, node.y, node.treenode.str
        //连线   node.fa.x, node.fa.y -> node.x, node.y
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

void MainWindow::dfs(int x, int l, int r, drawnode *fa, int dep)
{
    //遍历用的深度优先搜索，此处为前序遍历，搜索出本层所有节点，写入drawNodeData数组

    //load
    drawNodeData[dep][++cnt] = new drawnode(&segtree->tree[x], fa);
    if(fa) qDebug() << "pos: dep" << dep << "  data: (" << segtree->tree[x].l << ", " << segtree->tree[x].r << ")  cnt:"<< cnt << "  fa:" << fa->treenode->website;

    //next
    if(segtree->tree[x].l==segtree->tree[x].r){return;}
    int mid = (l+r)>>1;
    dfs(x<<1, l, mid, fa, dep);
    dfs(x<<1|1, mid+1, r, fa, dep);
}

void MainWindow::genDrawNode()
{
    memset(drawNodeData, 0, sizeof(drawNodeData));
    memset(layer, 0, sizeof(layer));

    layer[1] = 1;
    drawNodeData[1][1] = new drawnode(&segtree->tree[1], NULL);

    //更改遍历方式，采用递归的形式遍历，本质上还是双指针法，此处优先处理根节点
    int dep = 2;
    cnt = 0;
    //drawnode *current = new drawnode(T, drawNodeData[1][1]);
    //dfs(current->treenode, current->fa, dep);
    //layer[dep] = cnt;
    //dep++;
    //cnt = 0;

    while(1)
    {
        if(drawNodeData[dep-1][1] == NULL)
        {
            maxDep = dep-2;
            break;
        }
        for(int i = 1; i <= layer[dep-1]; i ++ )
        {
            //current->treenode->root即搜索本指针的下一层，并收集结果到drawNodeData数组
            drawnode *current = drawNodeData[dep-1][i];
            dfs(current->treenode->root, current, dep);
        }
        layer[dep] = cnt;
        dep ++;
        cnt = 0;
    }

    for(int dep = 1; dep <= maxDep; dep ++)
    {
        for(int i = 1; i <= layer[dep]; i ++ )
        {
            drawnode *current = drawNodeData[dep][i];
            current->x = sceneWidth  / (layer[dep]+1) * i;
            current->y = sceneHeight / (maxDep + 1) * dep;
        }
        qDebug() << "layer: " << dep << ": " << layer[dep];
    }
    qDebug() << "dep: " << maxDep;
}



void MainWindow::drawRect()
{
    for(int i = 0; i < rect_data.size(); i ++ )
    {
        //QGraphicsRectItem *item = new QGraphicsRectItem(rect_data[i]->x1,rect_data[i]->y1, rect_data[i]->x2 - rect_data[i]->x1, rect_data[i]->y2 - rect_data[i]->y1);
        myrect *item = new myrect(rect_data[i], rect_data[i]->x1,rect_data[i]->y1, rect_data[i]->x2 - rect_data[i]->x1, rect_data[i]->y2 - rect_data[i]->y1);
        item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setZValue(1);
        item->setBrush(QBrush(Qt::white));
        Rscene->addItem(item);
    }
}

void MainWindow::drawRectLine()
{
    for(int i = 0; i < rect_data.size(); i ++ )
    {
        QGraphicsLineItem *lineitem1 = new QGraphicsLineItem(rect_data[i]->x1, rect_data[i]->y1, rect_data[i]->x2, rect_data[i]->y1);

        lineitem1->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        lineitem1->setZValue(2);
        lineitem1->setPen(QPen(Qt::blue));
        Rscene->addItem(lineitem1);

        QGraphicsLineItem *lineitem2 = new QGraphicsLineItem(rect_data[i]->x1, rect_data[i]->y2, rect_data[i]->x2, rect_data[i]->y2);

        lineitem2->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        lineitem2->setZValue(2);
        lineitem2->setPen(QPen(Qt::red));
        Rscene->addItem(lineitem2);
    }
}

//inline void setRect(int x, int y, int w, int h) Q_DECL_NOTHROW;
void myrect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    this->rect.setRect(this->x, this->y, this->w, this->h);
    QPen mPen;
    QBrush mBrush;
    mPen.setWidth(2);
    mPen.setColor(this->rect_data->bgcolor);
    mBrush.setColor(this->rect_data->bgcolor);
    painter->setPen(mPen);
    painter->setBrush(mBrush);

    QPainterPath path,pathOthers;
    path.addRect(this->rect);
    painter->drawPath(path);

    //有重叠的情况
    if(!this->scene()->collidingItems(this).isEmpty())
    {
       QList<QGraphicsItem *> lstcolliItems = this->scene()->collidingItems(this);
       int nColliNum = lstcolliItems.count();
       for(int i = 0;i<nColliNum;i++)
       {
           QGraphicsItem* pTempItem = this->scene()->collidingItems(this)[i];
           QPainterPath tempPath = pTempItem->shape();
           tempPath.translate(pTempItem->pos());//转换到view中的坐标
           pathOthers += tempPath;//记录与本item重叠的item的路径
       }
       path.translate(this->pos());//转换到view中的坐标
       path &= pathOthers;//计算重叠部分的路径path
       path.translate(-this->pos().x(),-this->pos().y());//转换回本Item中的坐标
       QBrush brush(Qt::cyan);
       mPen.setColor(Qt::blue);
       painter->setPen(mPen);
       painter->setBrush(brush);
       painter->drawPath(path);//绘制重叠区域
    }
}



void MainWindow::test()
{
    double X1[]={0,0,1};
    double X2[]={0,2,3};
    double Y1[]={0,0,1};
    double Y2[]={0,2,3};
    rectdata* a = new rectdata(0,0,100,100);
    rectdata* b = new rectdata(200,200,300,300);

    rectdata* c = new rectdata(0,0,200,200);
    rectdata* d = new rectdata(150,150,300,300);
    rect_data.push_back(c);
    rect_data.push_back(d);

    mmax = 3;
    drawRect();
    //drawRectLine();
}
