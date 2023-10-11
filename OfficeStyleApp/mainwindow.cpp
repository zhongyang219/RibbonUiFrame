#include "mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QFontDialog>
#include <QFontComboBox>
#include <QDoubleSpinBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QSlider>
#include "dialog/ParagraphDialog.h"

#define QSTR(str) QString::fromWCharArray(L ## str)
#define CMD_FileOpen "FileOpen"
#define CMD_FileSaveAs "FileSaveAs"

//将一个像素值根据当前屏幕DPI设置进行等比放大
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
static int DPI(int x)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    return QGuiApplication::primaryScreen()->logicalDotsPerInch() * x / 96;
#else
        return QApplication::desktop()->logicalDpiX() * x / 96;
#endif
}

//从路径创建一个指定大小的图标
static QIcon CreateIcon(QString strPath, int size)
{
    if (!strPath.isEmpty())
    {
        QPixmap pixmap(strPath);
        if (!pixmap.isNull())
            return QIcon(pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    return QIcon();
}

MainWindow::MainWindow(QWidget *parent)
    : RibbonFrameWindow(parent, ":/res/MainFrame.xml", true)
{
    InitUi();
    m_edit.setStyleSheet(".QTextEdit{border:none;}");
    SetDefaultWidget(&m_edit);
    InitStatusBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::InitStatusBar()
{
    QStatusBar* pStatusbar = statusBar();
    if (pStatusbar == nullptr)
    {
        pStatusbar = new QStatusBar(this);
        setStatusBar(pStatusbar);
    }
    pStatusbar->setFixedHeight(DPI(22));
    QToolBar* pLeftBar = new QToolBar(pStatusbar);
    pStatusbar->addWidget(pLeftBar);
    pLeftBar->addAction(QSTR("页面：1/1"));
    pLeftBar->addSeparator();
    pLeftBar->addAction(QSTR("字数：0"));
    pLeftBar->addSeparator();
    pLeftBar->addAction(QSTR("中文 (中国)"));
    pLeftBar->addSeparator();
    pLeftBar->addAction(QSTR("插入"));

    QToolBar* pRightBar = new QToolBar(pStatusbar);
    pRightBar->setIconSize(QSize(DPI(16), DPI(16)));
    pRightBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    pStatusbar->addPermanentWidget(pRightBar);
    pRightBar->addAction(CreateIcon("://res/Image/printlayout16x16.png", DPI(16)), QSTR("页面视图"));
    pRightBar->addAction(CreateIcon("://res/Image/smallfullscreen.png", DPI(16)), QSTR("阅读版式视图"));
    pRightBar->addAction(CreateIcon("://res/Image/smallweblayout.png", DPI(16)), QSTR("Web 版式视图"));
    pRightBar->addAction(CreateIcon("://res/Image/smalloutline.png", DPI(16)), QSTR("大纲视图"));
    pRightBar->addAction(CreateIcon("://res/Image/smalldraft.png", DPI(16)), QSTR("草稿"));

    pRightBar->addAction("100%");
    pRightBar->addAction("-");
    QSlider* pZoomSlider = new QSlider(pRightBar);
    pZoomSlider->setOrientation(Qt::Horizontal);
    pZoomSlider->setFixedWidth(DPI(100));
    pRightBar->addWidget(pZoomSlider);
    pRightBar->addAction("+");
}


bool MainWindow::OnCommand(const QString &strCmd, bool checked)
{
    Q_UNUSED(checked)
    if (strCmd == CMD_FileOpen)
    {
        QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), QSTR("文本文件 (*.txt);;HTML文件 (*.html *.htm)"));
        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly))
            {
                QString strContents = QString::fromUtf8(file.readAll());
                QString fileExtension = QFileInfo(fileName).completeSuffix();
                if (fileExtension.startsWith("htm"))
                    m_edit.setHtml(strContents);
                else
                    m_edit.setText(strContents);
                file.close();
            }
        }
    }
    else if (strCmd == CMD_FileSaveAs)
    {
        QString fileName = QFileDialog::getSaveFileName(this, QString(), QString(), QSTR("文本文件 (*.txt);;HTML文件 (*.html)"));
        if (!fileName.isEmpty())
        {
            //
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly))
            {
                QString fileExtension = QFileInfo(fileName).completeSuffix();
                if (fileExtension.startsWith("htm"))
                    file.write(m_edit.toHtml().toUtf8());
                else
                    file.write(m_edit.toPlainText().toUtf8());
                file.close();
            }
        }
    }
    else if (strCmd == "AppAbout")
    {
        QMessageBox::about(this, QSTR("关于 %1").arg(qApp->applicationName()), QSTR("%1 %2\r\n这是一个Office风格的示例程序。").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    }
    else if (strCmd == "GroupClipBoard")
    {
        QMessageBox::information(this, QString(), QSTR("剪贴板对话框。"));
    }
    else if (strCmd == "GroupFont")
    {
        QFontDialog dlg(this);
        dlg.exec();
    }
    else if (strCmd == "colorOther")
    {
        QColorDialog dlg(this);
        dlg.exec();
    }
    else if (strCmd == "GroupParagraph")
    {
        ParagraphDialog dlg(this);
        dlg.exec();
    }
    else if (strCmd == "GroupPageSetup")
    {
        QMessageBox::information(this, QString(), QSTR("页面设置对话框。"));
    }

    return RibbonFrameWindow::OnCommand(strCmd, checked);
}


QWidget *MainWindow::CreateUserWidget(const QString &strId, QWidget *pParent)
{
    if (strId == "FontCombobox")
    {
        QFontComboBox* pFontCombo = new QFontComboBox(pParent);
        return pFontCombo;
    }
    else if (strId == "IndentLeftSpin" || strId == "IndentRightSpin")
    {
        QDoubleSpinBox* pSpin = new QDoubleSpinBox(pParent);
        pSpin->setSuffix(QSTR(" 字符"));
        pSpin->setSingleStep(0.5);
        pSpin->setDecimals(1);
        pSpin->setRange(-99, 99);
        return pSpin;
    }
    else if (strId == "SpacingBeforeSpin" || strId == "SpacingAfterSpin")
    {
        QDoubleSpinBox* pSpin = new QDoubleSpinBox(pParent);
        pSpin->setSuffix(QSTR(" 行"));
        pSpin->setSingleStep(0.5);
        pSpin->setDecimals(1);
        return pSpin;

    }
    return nullptr;
}
