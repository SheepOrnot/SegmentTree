#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qpushbutton.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1600,900);
    ui->martixnum->setPlaceholderText(" 矩阵个数 ");
    ui->x1->setPlaceholderText("x1");
    ui->x2->setPlaceholderText("x2");
    ui->y1->setPlaceholderText("y1");
    ui->y2->setPlaceholderText("y2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

