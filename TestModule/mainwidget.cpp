#include "mainwidget.h"
#include "ui_mainwidget.h"

CMainWidget::CMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMainWidget)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter_2->setStretchFactor(0, 2);

    //设置“导航界面”样式
    ui->widgetLeft->setProperty("nav", "left");
    ui->widgetBottom->setProperty("form", "bottom");
    ui->widgetTop->setProperty("nav", "top");
}

CMainWidget::~CMainWidget()
{
    delete ui;
}
