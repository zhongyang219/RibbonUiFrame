#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>
#include "Common.h"

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
CMainWidget::CMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMainWidget)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter_2->setStretchFactor(0, 2);

    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    //设置“导航界面”样式
    ui->widgetLeft->setProperty("nav", "left");
    ui->widgetBottom->setProperty("form", "bottom");
    ui->widgetTop->setProperty("nav", "top");

    //设置“图标”页
    InitSytemIcon();
}

CMainWidget::~CMainWidget()
{
    delete ui;
}

void CMainWidget::InitSytemIcon()
{
    QGridLayout* layout = new QGridLayout();
    ui->iconWidget->setLayout(layout);
    int iconIndex = 0;
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            enum QStyle::StandardPixmap Icon = (enum QStyle::StandardPixmap)iconIndex;
            QPushButton* pBtn = new QPushButton(QApplication::style()->standardIcon(Icon), QString::number(iconIndex));
            pBtn->setToolTip(CCommon::GetStandardIconDescription(iconIndex));
            pBtn->setCheckable(true);
            pBtn->setProperty("iconIndex", iconIndex);
            pBtn->setAutoExclusive(true);
            connect(pBtn, SIGNAL(clicked(bool)), this, SLOT(OnIconBtnClicked(bool)));
            layout->addWidget(pBtn, row, col);
            iconIndex++;
        }
    }
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

void CMainWidget::OnIconBtnClicked(bool)
{
    int iconIndex = QObject::sender()->property("iconIndex").toInt();
    ui->iconDescEdit->setText(CCommon::GetStandardIconDescription(iconIndex).replace("\r\n", "\t"));
}