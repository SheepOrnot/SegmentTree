#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QStringListModel>
#include <QVector>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include "segmenttree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class drawnode
{
public:
    drawnode(Node *node, drawnode* _fa);

    drawnode *fa;
    Node *treenode;
    int x, y;
    QColor fontColor;
    QColor lineColor;
    QColor vectorColor;
};

class rectdata
{
public:
    rectdata() {}
    rectdata(int x1, int y1, int x2, int y2):x1(x1), x2(x2), y1(y1), y2(y2) {}
    int x1, x2, y1, y2;

    QColor bgcolor = QColor(rand()%256,rand()%256,rand()%256);
    int r, g, b;
};


class myrect : public QGraphicsRectItem
{
public:
    myrect(rectdata *rect_data, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR) :
                QGraphicsRectItem(x, y, w, h, parent = Q_NULLPTR),
                rect_data(rect_data)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int x, y, w, h;
    rectdata *rect_data;
    QRect rect = QRect();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SegmentTree *segtree;

    int maxDep;
    int layer[25];
    int alreadyGen = 0;
    int cnt = 0;
    drawnode *drawNodeData[25][1010];

    QGraphicsScene *Rscene;
    QGraphicsScene *Tscene;
    int RsceneWidth, RsceneHeight;
    int vectorWidth = 50;
    int vectorHeight = 50;
    int gviewHeight = 700;
    int gviewWidth  = 700;

    int TsceneWidth, TsceneHeight;

    QVector<rectdata*> rect_data;

    void drawNode();
    //void genDrawNode();
    void node2graph(drawnode* node);
    void drawRectLine();
    void drawRect();
    void test();
    void genDrawNode();
    void dfs(int x, int l, int r, drawnode *fa, int dep);

    int mmax = -1;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
