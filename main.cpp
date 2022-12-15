#include "mainwindow.h"
#include "segmenttree.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    double X1[]={0,0,1};
    double X2[]={0,2,3};
    double Y1[]={0,0,1};
    double Y2[]={0,2,3};
    SegmentTree T;
    std::pair<double,double> PP = T.Calc(2,X1,Y1,X2,Y2);
    qDebug()<<PP.first<<" "<<PP.second;
    w.show();
    return a.exec();
}
