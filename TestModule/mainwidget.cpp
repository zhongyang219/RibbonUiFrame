#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>

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

void CMainWidget::on_btnInfo_clicked()
{
    QMessageBox::information(this, QString(), u8"信息框测试。");
}


void CMainWidget::on_btnQuestion_clicked()
{
    QMessageBox::question(this, QString(), u8"提示框测试。");
}


void CMainWidget::on_btnError_clicked()
{
    QMessageBox::critical(this, QString(), u8"错误框测试。");
}


void CMainWidget::on_btnInput_clicked()
{
    QInputDialog dlg(this);
    dlg.exec();
}


void CMainWidget::on_btnFont_clicked()
{
    QFontDialog dlg(this);
    dlg.exec();
}


void CMainWidget::on_btnColor_clicked()
{
    QColorDialog dlg(this);
    dlg.exec();
}

