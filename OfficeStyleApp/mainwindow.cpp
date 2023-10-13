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
#include "ribbonuipredefine.h"

#define CMD_FileOpen "FileOpen"
#define CMD_FileSaveAs "FileSaveAs"
#define CMD_FileSave "FileSave"
#define CMD_EditPaste "Paste"
#define CMD_EditCut "Cut"
#define CMD_EditCopy "Copy"
#define CMD_EditUndo "undo"
#define CMD_EditRedo "redo"
#define CMD_FontComboBox "FontCombobox"
#define CMD_FontPointSizeComboBox "FontSizeCombobox"
#define CMD_Bold "Bold"
#define CMD_Italic "Italic"
#define CMD_UnderLine "UnderLine"
#define CMD_Strickout "Strickout"
#define CMD_FontGroup "GroupFont"



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
    SetItemText(CMD_FontPointSizeComboBox, "9");

    connect(&m_edit, SIGNAL(cursorPositionChanged()), this, SLOT(OnCursorPositionChanged()));
    connect(&m_edit, SIGNAL(copyAvailable(bool)), this, SLOT(OnCopyAvailable(bool)));
    connect(&m_edit, SIGNAL(undoAvailable(bool)), this, SLOT(OnUndoAvailable(bool)));
    connect(&m_edit, SIGNAL(redoAvailable(bool)), this, SLOT(OnRedoAvailable(bool)));

    UpdateWindowTitle();
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
    QActionGroup* pActionGroup = new QActionGroup(pRightBar);
    auto addRightAction = [&](const QString& iconPath, const QString& text){
        QAction* pAction = pRightBar->addAction(CreateIcon(iconPath, DPI(16)), text);
        pAction->setCheckable(true);
        pActionGroup->addAction(pAction);
    };
    addRightAction("://res/Image/printlayout16x16.png", QSTR("页面视图"));
    addRightAction("://res/Image/smallfullscreen.png", QSTR("阅读版式视图"));
    addRightAction("://res/Image/smallweblayout.png", QSTR("Web 版式视图"));
    addRightAction("://res/Image/smalloutline.png", QSTR("大纲视图"));
    addRightAction("://res/Image/smalldraft.png", QSTR("草稿"));

    pRightBar->addAction("100%");
    pRightBar->addAction("-");
    QSlider* pZoomSlider = new QSlider(pRightBar);
    pZoomSlider->setOrientation(Qt::Horizontal);
    pZoomSlider->setFixedWidth(DPI(100));
    pRightBar->addWidget(pZoomSlider);
    pRightBar->addAction("+");
}

void MainWindow::ApplyFont()
{
    if (m_fontUiApplEnable)
    {
        QFont font = m_edit.currentFont();
        QFontComboBox* pFontCombo = qobject_cast<QFontComboBox*>(_GetWidget(CMD_FontComboBox));
        if (pFontCombo != nullptr)
            font.setFamily(pFontCombo->currentFont().family());
        int pointSize = QString(GetItemText(CMD_FontPointSizeComboBox)).toInt();
        QTextCharFormat format;
        format.setFontPointSize(pointSize);
        m_edit.setCurrentCharFormat(format);
        font.setBold(IsItemChecked(CMD_Bold));
        font.setItalic(IsItemChecked(CMD_Italic));
        font.setUnderline(IsItemChecked(CMD_UnderLine));
        font.setStrikeOut(IsItemChecked(CMD_Strickout));
        m_edit.setCurrentFont(font);
    }
}

void MainWindow::UpdateFontUi()
{
    m_fontUiApplEnable = false;
    QFont font = m_edit.currentFont();
    QFontComboBox* pFontCombo = qobject_cast<QFontComboBox*>(_GetWidget(CMD_FontComboBox));
    if (pFontCombo != nullptr)
        pFontCombo->setCurrentFont(font);
    SetItemText(CMD_FontPointSizeComboBox, QString::number(font.pointSize()).toUtf8().constData());
    SetItemChecked(CMD_Bold, font.bold());
    SetItemChecked(CMD_Italic, font.italic());
    SetItemChecked(CMD_UnderLine, font.underline());
    SetItemChecked(CMD_Strickout, font.strikeOut());
    m_fontUiApplEnable = true;
}

void MainWindow::UpdateWindowTitle()
{
    QString strTitle;
    if (m_edit.isWindowModified())
        strTitle += "*";
    if (m_filePath.isEmpty())
        strTitle += u8"无标题";
    else
        strTitle += m_filePath;
    strTitle += " - ";
    strTitle += qApp->applicationName();
#ifdef QT_DEBUG
    strTitle += u8" (Debug模式)";
#endif
    setWindowTitle(strTitle);
}

MainWindow::DocFormat MainWindow::GetDocFormatByFileName(const QString &fileName)
{
    DocFormat format;
    QString extension = QFileInfo(fileName).completeSuffix().toLower();
    if (extension == "md")
        format = DocFormat::Markdown;
    else if (extension.startsWith("htm"))
        format = DocFormat::Html;
    else
        format = DocFormat::Text;
    return format;
}

bool MainWindow::OpenFile(const QString &filePath, DocFormat format)
{
    m_filePath = filePath;
    UpdateWindowTitle();
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QString strContents = QString::fromUtf8(file.readAll());
        if (format == DocFormat::Undefined)
        {
            format = GetDocFormatByFileName(filePath);
        }
        if (format == DocFormat::Markdown)
            m_edit.setMarkdown(strContents);
        else if (format == DocFormat::Html)
            m_edit.setHtml(strContents);
        else
            m_edit.setPlainText(strContents);

        file.close();
        return true;
    }
    return false;
}

bool MainWindow::SaveFile(const QString &filePath, DocFormat format)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        if (format == DocFormat::Undefined)
        {
            format = GetDocFormatByFileName(filePath);
        }
        if (format == DocFormat::Markdown)
            file.write(m_edit.toMarkdown().toUtf8());
        else if (format == DocFormat::Html)
            file.write(m_edit.toHtml().toUtf8());
        else
            file.write(m_edit.toPlainText().toUtf8());
        file.close();
        return true;
    }
    return false;
}

void MainWindow::OnCurrentFontChanged(const QFont &font)
{
    if (m_fontUiApplEnable)
    {
        QFont currentFont = m_edit.currentFont();
        currentFont.setFamily(font.family());
        m_edit.setCurrentFont(currentFont);
    }
}

void MainWindow::OnCursorPositionChanged()
{
    UpdateFontUi();
    UpdateWindowTitle();
}

void MainWindow::OnCopyAvailable(bool yes)
{
    SetItemEnable(CMD_EditCopy, yes);
    SetItemEnable(CMD_EditCut, yes);
}

void MainWindow::OnUndoAvailable(bool available)
{
    SetItemEnable(CMD_EditUndo, available);
}

void MainWindow::OnRedoAvailable(bool available)
{
    SetItemEnable(CMD_EditRedo, available);
}


bool MainWindow::OnCommand(const QString &strCmd, bool checked)
{
    Q_UNUSED(checked)
    if (strCmd == CMD_FileOpen)
    {
        QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), u8"所有支持的文件 (*.txt *.md *.html *.htm);;HTML文件 (*.html *.htm);;Markdown文件 (*.md);;文件文件 (*.txt)");
        if (!fileName.isEmpty())
        {
            if (!OpenFile(fileName))
            {
                QMessageBox::warning(this, QString(), u8"文件打开失败！");
            }
        }
    }
    else if (strCmd == CMD_FileSave)
    {
        if (!SaveFile(m_filePath))
        {
            QMessageBox::warning(this, QString(), u8"文件保存失败！");
        }
    }
    else if (strCmd == CMD_FileSaveAs)
    {
        QString fileName = QFileDialog::getSaveFileName(this, QString(), m_filePath, u8"HTML文件 (*.html);;Markdown文件 (*.md);;文件文件 (*.txt)");
        if (!fileName.isEmpty())
        {
            if (!SaveFile(fileName))
            {
                QMessageBox::warning(this, QString(), u8"文件保存失败！");
            }
        }
    }
    else if (strCmd == CMD_EditUndo)
    {
        m_edit.undo();
    }
    else if (strCmd == CMD_EditRedo)
    {
        m_edit.redo();
    }
    else if (strCmd == CMD_EditCopy)
    {
        m_edit.copy();
    }
    else if (strCmd == CMD_EditCut)
    {
        m_edit.cut();
    }
    else if (strCmd == CMD_EditPaste)
    {
        m_edit.paste();
    }
    else if (strCmd == CMD_Bold || strCmd == CMD_Italic || strCmd == CMD_UnderLine || strCmd == CMD_Strickout)
    {
        ApplyFont();
    }
    else if (strCmd == CMD_FontGroup)
    {
        QFontDialog dlg(m_edit.currentFont(), this);
        if (dlg.exec() == QDialog::Accepted)
            m_edit.setCurrentFont(dlg.font());
    }
    else if (strCmd == "AppAbout")
    {
        QMessageBox::about(this, QSTR("关于 %1").arg(qApp->applicationName()), QSTR("%1 %2\r\n这是一个Office风格的示例程序。").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    }
    else if (strCmd == "GroupClipBoard")
    {
        QMessageBox::information(this, QString(), QSTR("剪贴板对话框。"));
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

void MainWindow::OnItemChanged(const QString &strId, int index, const QString &text)
{
    if (strId == CMD_FontPointSizeComboBox)
    {
        ApplyFont();
    }
}


QWidget *MainWindow::CreateUserWidget(const QString &strId, QWidget *pParent)
{
    if (strId == CMD_FontComboBox)
    {
        QFontComboBox* pFontCombo = new QFontComboBox(pParent);
        pFontCombo->setEditable(false);
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
