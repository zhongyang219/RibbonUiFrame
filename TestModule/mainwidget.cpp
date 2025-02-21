#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>
#include "Common.h"
#include "testmodule.h"

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
CMainWidget::CMainWidget(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWidget)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter_2->setStretchFactor(0, 2);

    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    //初始化“导航界面”
    ui->widgetLeft->AddItem(u8"项目一", QIcon(":/res/Template.png"));
    ui->widgetLeft->AddItem(u8"项目二");
    ui->widgetLeft->AddItem(u8"项目三");
    ui->widgetLeft->AddSpacing();
    connect(ui->widgetLeft, SIGNAL(curItemChanged(const QString&)), this, SLOT(OnNavChanged(const QString&)));

    ui->widgetTop->AddItem(u8"界面一");
    ui->widgetTop->AddItem(u8"界面二");
    ui->widgetTop->AddItem(u8"界面三");
    connect(ui->widgetTop, SIGNAL(curItemChanged(const QString&)), this, SLOT(OnNavChanged(const QString&)));

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

//点击了“导航界面”页中的按钮
void CMainWidget::OnNavChanged(const QString& text)
{
    QString strMsg = QString(u8"点击了导航栏 %1").arg(text);
    TestModule::Instance()->SetStatusBarText(strMsg, 10000);
}
