#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qpushbutton.h>


drawnode::drawnode(Node *node, drawnode *_fa)
{
    this->treenode = node;
    this->x = this->y = 0;
    this->fa = _fa;
    this->fontColor = Qt::black;
    this->lineColor = Qt::black;
    this->vectorColor = Qt::white;
}

//比例尺 50:1
static double maprate = 100.0;
static double X1[20];
static double Y1[20];
static double X2[20];
static double Y2[20];
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(1600,900);
    //ui->martixnum->setPlaceholderText(" 矩阵个数 ");
    ui->x1->setPlaceholderText("x1");
    ui->x2->setPlaceholderText("x2");
    ui->y1->setPlaceholderText("y1");
    ui->y2->setPlaceholderText("y2");

    Rscene = new QGraphicsScene(0, 0, ui->RgraphicsView->width()-10, ui->RgraphicsView->height()-10);
    Tscene = new QGraphicsScene(0, 0, ui->TgraphicsView->width()-10, ui->TgraphicsView->height()-10);
    ui->RgraphicsView->setScene(Rscene);
    ui->TgraphicsView->setScene(Tscene);
    RsceneWidth  = Rscene->width();
    RsceneHeight = Rscene->height();
    TsceneWidth  = Tscene->width();
    TsceneHeight = Tscene->height();

    segtree = new SegmentTree;

    //test();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::node2graph(drawnode* node)
{
    QGraphicsRectItem *item = new QGraphicsRectItem(node->x,node->y, (segtree->X[node->treenode->r+1] - segtree->X[node->treenode->l]) * maprate, vectorHeight);
    qDebug() << "noderect: [" << segtree->X[node->treenode->l] << "," << segtree->X[node->treenode->r+1] << "] len: " << (segtree->X[node->treenode->r+1] - segtree->X[node->treenode->l]) * maprate;
    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setZValue(2);
    item->setBrush(QBrush(node->vectorColor));
    Tscene->addItem(item);

    //QGraphicsTextItem *textitem = new QGraphicsTextItem(node->treenode->s, item);
    char buffer[255];
    sprintf(buffer, "[%.2lf,%.2lf]", segtree->X[node->treenode->l], segtree->X[node->treenode->r+1]);
    QGraphicsTextItem *textitem = new QGraphicsTextItem(buffer, item);
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
}

void MainWindow::drawNode()
{
    Tscene->clear();
    genDrawNode();

    qDebug() << " 开始绘制 ";
    for(int dep = maxDep; dep >= 1; dep --)
    {
        for(int i = 1; i <= layer[dep]; i ++ )
        {
            drawnode *current = drawNodeData[dep][i];
            node2graph(current);
        }
    }

    //for node in drawNodeData[1~allDep-1][1~layer[dep]]
        //node2graph(node)
        //画椭圆 node.x, node.y
        //写文字 node.x, node.y, node.treenode.str
        //连线   node.fa.x, node.fa.y -> node.x, node.y
}

void MainWindow::dfs(int x, int l, int r, drawnode *fa, int dep)
{
    //遍历用的深度优先搜索，此处为前序遍历，搜索出本层所有节点，写入drawNodeData数组

    //load
    drawnode* current = drawNodeData[dep][++ layer[dep]] = new drawnode(&segtree->tree[x], fa);
    //if(fa) qDebug() << "pos: dep" << dep << "  data: (" << segtree->tree[x].l << ", " << segtree->tree[x].r << ")  cnt:"<< cnt << "  fa:" << fa->treenode->website;

    //next
    if(segtree->tree[x].l==segtree->tree[x].r)
    {
        if(maxDep < dep) maxDep = dep;
        return;
    }
    int mid = (l+r)>>1;
    dfs(x<<1, l, mid, current, dep+1);
    dfs(x<<1|1, mid+1, r, current, dep+1);
}

void MainWindow::genDrawNode()
{
    memset(drawNodeData, 0, sizeof(drawNodeData));
    memset(layer, 0, sizeof(layer));

    dfs(1, 1, mmax, NULL, 1);

    for(int dep = 1; dep <= maxDep; dep ++)
    {
        int acc = 0;
        for(int i = 1; i <= layer[dep]; i ++ )
        {
            drawnode *current = drawNodeData[dep][i];
            //current->x = TsceneWidth  / (layer[dep]+1) * i;
            current->x = acc;
            current->y = TsceneHeight / (maxDep + 1) * (maxDep - dep + 1);

            acc += ( segtree->X[current->treenode->r+1] - segtree->X[current->treenode->l]) * maprate;
        }
        qDebug() << "layer: " << dep << ": " << layer[dep];
    }
    qDebug() << "dep: " << maxDep;
}

void MainWindow::drawRect()
{
    this->Rscene->clear();
    for(int i = 0; i < rect_data.size(); i ++ )
    {
        //QGraphicsRectItem *item = new QGraphicsRectItem(rect_data[i]->x1,rect_data[i]->y1, rect_data[i]->x2 - rect_data[i]->x1, rect_data[i]->y2 - rect_data[i]->y1);
        myrect *item = new myrect(rect_data[i], rect_data[i]->x1,rect_data[i]->y1, rect_data[i]->x2 - rect_data[i]->x1, rect_data[i]->y2 - rect_data[i]->y1);
        item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setZValue(1);
        item->setBrush(QBrush(Qt::white));
        item->setPen(QPen(rect_data[i]->bgcolor));
        Rscene->addItem(item);
        rect_pointer.push_back(item);
    }
}

void myrect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    this->rect.setRect(this->x, this->y, this->w, this->h);
    QPen mPen;
    QBrush mBrush;
    mPen.setWidth(2);
    mPen.setColor(this->rect_data->bgcolor);
    mBrush.setColor(this->rect_data->bgcolor);


    QPainterPath pathn,pathOthersn, path, pathOthers;

    pathOthersn = this->shape();
    pathOthersn.translate(this->pos());
    pathn += pathOthersn;

    pathn.translate(-this->pos().x(),-this->pos().y());//转换回本Item中的坐标
    painter->setPen(mPen);
    painter->setBrush(mBrush);
    painter->drawPath(pathn);
}

void MainWindow::test()
{
    double X1[]={0,0,2,3};
    double X2[]={0,4,6,6};
    double Y1[]={0,0,2,3};
    double Y2[]={0,4,6,6};
    rectdata* a = new rectdata(0*maprate,0*maprate,1*maprate,1*maprate);
    rectdata* b = new rectdata(2*maprate,2*maprate,3*maprate,3*maprate);

    rectdata* c = new rectdata(0*maprate,0*maprate,1*maprate,1*maprate);
    rectdata* d = new rectdata(2*maprate,2*maprate,3*maprate,3*maprate);

    rectdata* f = new rectdata(0*maprate,0*maprate,4*maprate,4*maprate);
    rectdata* g = new rectdata(2*maprate,2*maprate,6*maprate,6*maprate);
    rectdata* h = new rectdata(3*maprate,3*maprate,6*maprate,6*maprate);

    rect_data.push_back(f);
    rect_data.push_back(g);
    rect_data.push_back(h);

    mmax = 6;
    drawRect();
    std::pair<double,double> res =  segtree->Calc(3, X1, Y1, X2, Y2);
    qDebug() << res.first << "  " << res.second;
    drawNode();

}

void MainWindow::on_xyok_clicked()
{
   double inx1 = ui->x1->text().toDouble();
   double iny1 = ui->y1->text().toDouble();
   double inx2 = ui->x2->text().toDouble();
   double iny2 = ui->y2->text().toDouble();
   qDebug() << "read input: (" << inx1 << "," << iny1 << ")  " << "(" << inx2 << "," << iny2 << ")";

   inputnum ++;
   X1[inputnum] = inx1;
   Y1[inputnum] = iny1;
   X2[inputnum] = inx2;
   Y2[inputnum] = iny2;

   mmax = fmax(mmax, inx1);
   mmax = fmax(mmax, iny1);
   mmax = fmax(mmax, inx2);
   mmax = fmax(mmax, iny2);

   rectdata* newrect = new rectdata(inx1*maprate,iny1*maprate,inx2*maprate,iny2*maprate);
   rect_data.push_back(newrect);

   std::pair<double,double> res =  segtree->Calc(inputnum, X1, Y1, X2, Y2);
   qDebug() << res.first << "  " << res.second;

   ui->ans_s->setText(QString::number(res.first, 'f', 2));
   ui->ans_c->setText(QString::number(res.second, 'f', 2));

   drawRect();
   drawNode();
}

void MainWindow::on_cancel_clicked()
{
    if(inputnum == 0) return;
    inputnum --;

    rectdata* last = rect_data.last();
    free(last);
    rect_data.pop_back();

    QGraphicsRectItem* pRect = rect_pointer.last();
    free(pRect);
    rect_pointer.pop_back();

    if(inputnum == 0)
    {
        Rscene->clear();
        Tscene->clear();
        return;
    }
    mmax = 0;
    for(int i = 1; i <= inputnum; i ++)
    {
        mmax = fmax(mmax, X1[inputnum]);
        mmax = fmax(mmax, Y1[inputnum]);
        mmax = fmax(mmax, X2[inputnum]);
        mmax = fmax(mmax, Y2[inputnum]);
    }

    std::pair<double,double> res =  segtree->Calc(inputnum, X1, Y1, X2, Y2);
    qDebug() << res.first << "  " << res.second;

    ui->ans_s->setText(QString::number(res.first, 'f', 2));
    ui->ans_c->setText(QString::number(res.second, 'f', 2));

    drawRect();
    drawNode();
}

void MainWindow::insertLine(QString line)
{
    double inx1, inx2, iny1, iny2;
    sscanf(line.toStdString().c_str(), "%lf %lf %lf %lf", &inx1, &iny1, &inx2, &iny2);
    qDebug() << "read input: (" << inx1 << "," << iny1 << ")  " << "(" << inx2 << "," << iny2 << ")";

    inputnum ++;
    X1[inputnum] = inx1;
    Y1[inputnum] = iny1;
    X2[inputnum] = inx2;
    Y2[inputnum] = iny2;

    mmax = fmax(mmax, inx1);
    mmax = fmax(mmax, iny1);
    mmax = fmax(mmax, inx2);
    mmax = fmax(mmax, iny2);

    rectdata* newrect = new rectdata(inx1*maprate,iny1*maprate,inx2*maprate,iny2*maprate);
    rect_data.push_back(newrect);
}

void MainWindow::on_save_clicked()
{
    //创建一个file文件
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,tr("Open File"),"/data",tr("Text File(*.txt)"));
    if(fileName == "")
    {
        return;
    }
    QFile file(fileName);//可以自己选择路径来保存文件名
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
        return;
    }
    else
    {
        QTextStream textStream(&file);

        textStream << inputnum << '\n';

        for(int i = 1; i <= inputnum; i ++ )
        {
            QString line;
            line.sprintf("%.2f %.2f %.2f %.2f\n", X1[i], Y1[i], X2[i], Y2[i]);
            textStream << line;
        }
        //QString str = ui->textEdit->toPlainText();//从textEdit里面回去内容，然后再直接拿来用
        //textStream<<str;

        QMessageBox::warning(this,tr("提示"),tr("保存文件成功"));
        file.close();
    }

}

void MainWindow::on_fileimport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,QStringLiteral("文件对话框！"),"F:",QStringLiteral("TextFile(*txt)"));
    qDebug() << "得到文件名:" << fileName;
    QFile file(fileName);
    QByteArray bytes;
    if(!file.exists()) //文件不存在则退出
    {
        qDebug()<<"file not exist";
        return;
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line;
        QTextStream in(&file);  //用文件构造流

        line = in.readLine();//读取一行放到字符串里 第一行为数量
        inputnum = line.toInt();
        for(int i = 1; i <= inputnum; i ++ )
        {
            line=in.readLine();//循环读取下行
            if(line.isNull()) break;
            insertLine(line);
        }
    }

    std::pair<double,double> res =  segtree->Calc(inputnum, X1, Y1, X2, Y2);
    qDebug() << res.first << "  " << res.second;

    ui->ans_s->setText(QString::number(res.first, 'f', 2));
    ui->ans_c->setText(QString::number(res.second, 'f', 2));

    drawRect();
    drawNode();

    qDebug() << "导入成功";
}
