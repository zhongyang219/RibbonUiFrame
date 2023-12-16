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
#include <QListView>
#include "dialog/ParagraphDialog.h"
#include "ribbonuipredefine.h"
#include "resId.h"

MainWindow::MainWindow(QWidget *parent)
    : RibbonFrameWindow(parent, ":/res/MainFrame.xml", true)
{
    InitUi();
    m_edit.setStyleSheet(".QTextEdit{border:none;}");
    SetDefaultWidget(&m_edit);
    SetItemText(ID_FontSizeCombobox, "9");

    connect(&m_edit, SIGNAL(cursorPositionChanged()), this, SLOT(OnCursorPositionChanged()));
    connect(&m_edit, SIGNAL(copyAvailable(bool)), this, SLOT(OnCopyAvailable(bool)));
    connect(&m_edit, SIGNAL(undoAvailable(bool)), this, SLOT(OnUndoAvailable(bool)));
    connect(&m_edit, SIGNAL(redoAvailable(bool)), this, SLOT(OnRedoAvailable(bool)));

    QWidget* pZoomInBtn = _GetWidget("statusbar_zoomin");
    QWidget* pZoomOutBtn = _GetWidget("statusbar_zoomout");
    if (pZoomInBtn != nullptr)
        pZoomInBtn->setObjectName("zoomInBtn");
    if (pZoomOutBtn != nullptr)
        pZoomOutBtn->setObjectName("zoomOutBtn");

    QSlider* pZoomSlider = qobject_cast<QSlider*>(_GetWidget("statusbar_zoom_slider"));
    if (pZoomSlider != nullptr)
    {
        pZoomSlider->setRange(0, 200);
        pZoomSlider->setSingleStep(10);
        pZoomSlider->setSliderPosition(100);
        connect(pZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomSliderScroll(int)));
    }

    UpdateWindowTitle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::ApplyFont()
{
    if (m_fontUiApplEnable)
    {
        QFont font = m_edit.currentFont();
        QFontComboBox* pFontCombo = qobject_cast<QFontComboBox*>(_GetWidget(ID_FontCombobox));
        if (pFontCombo != nullptr)
            font.setFamily(pFontCombo->currentFont().family());
        int pointSize = QString(GetItemText(ID_FontSizeCombobox)).toInt();
        QTextCharFormat format;
        format.setFontPointSize(pointSize);
        m_edit.setCurrentCharFormat(format);
        font.setBold(IsItemChecked(ID_Bold));
        font.setItalic(IsItemChecked(ID_Italic));
        font.setUnderline(IsItemChecked(ID_UnderLine));
        font.setStrikeOut(IsItemChecked(ID_Strickout));
        m_edit.setCurrentFont(font);
    }
}

void MainWindow::UpdateFontUi()
{
    m_fontUiApplEnable = false;
    QFont font = m_edit.currentFont();
    QFontComboBox* pFontCombo = qobject_cast<QFontComboBox*>(_GetWidget(ID_FontCombobox));
    if (pFontCombo != nullptr)
        pFontCombo->setCurrentFont(font);
    SetItemText(ID_FontSizeCombobox, QString::number(font.pointSize()).toUtf8().constData());
    SetItemChecked(ID_Bold, font.bold());
    SetItemChecked(ID_Italic, font.italic());
    SetItemChecked(ID_UnderLine, font.underline());
    SetItemChecked(ID_Strickout, font.strikeOut());
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
    SetItemEnable(ID_Copy, yes);
    SetItemEnable(ID_Cut, yes);
}

void MainWindow::OnUndoAvailable(bool available)
{
    SetItemEnable(ID_undo, available);
}

void MainWindow::OnRedoAvailable(bool available)
{
    SetItemEnable(ID_redo, available);
}

void MainWindow::zoomSliderScroll(int value)
{
    QString str;
    str += QString("%1%").arg(value);
    SetItemText("statusbar_zoom", str.toUtf8().constData());
}

bool MainWindow::OnCommand(const QString &strCmd, bool checked)
{
    Q_UNUSED(checked)
    if (strCmd == ID_FileOpen)
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
    else if (strCmd == ID_FileSave)
    {
        if (!SaveFile(m_filePath))
        {
            QMessageBox::warning(this, QString(), u8"文件保存失败！");
        }
    }
    else if (strCmd == ID_FileSaveAs)
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
    else if (strCmd == ID_undo)
    {
        m_edit.undo();
    }
    else if (strCmd == ID_redo)
    {
        m_edit.redo();
    }
    else if (strCmd == ID_Copy)
    {
        m_edit.copy();
    }
    else if (strCmd == ID_Cut)
    {
        m_edit.cut();
    }
    else if (strCmd == ID_Paste)
    {
        m_edit.paste();
    }
    else if (strCmd == ID_Bold || strCmd == ID_Italic || strCmd == ID_UnderLine || strCmd == ID_Strickout)
    {
        ApplyFont();
    }
    else if (strCmd == ID_GroupFont)
    {
        QFontDialog dlg(m_edit.currentFont(), this);
        if (dlg.exec() == QDialog::Accepted)
            m_edit.setCurrentFont(dlg.font());
    }
    else if (strCmd == ID_AppAbout)
    {
        QMessageBox::about(this, QSTR("关于 %1").arg(qApp->applicationName()), QSTR("%1 %2\r\n这是一个Office风格的示例程序。").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    }
    else if (strCmd == ID_GroupClipBoard)
    {
        QMessageBox::information(this, QString(), QSTR("剪贴板对话框。"));
    }
    else if (strCmd == ID_colorOther)
    {
        QColorDialog dlg(this);
        dlg.exec();
    }
    else if (strCmd == ID_GroupParagraph)
    {
        ParagraphDialog dlg(this);
        dlg.exec();
    }
    else if (strCmd == ID_GroupPageSetup)
    {
        QMessageBox::information(this, QString(), QSTR("页面设置对话框。"));
    }
    else if (strCmd == ID_statusbar_zoomin)
    {
        QSlider* pZoomSlider = qobject_cast<QSlider*>(_GetWidget(ID_statusbar_zoom_slider));
        if (pZoomSlider != nullptr)
        {
            pZoomSlider->setValue((pZoomSlider->value() + 10) / 10 * 10);
        }
    }
    else if (strCmd == ID_statusbar_zoomout)
    {
        QSlider* pZoomSlider = qobject_cast<QSlider*>(_GetWidget(ID_statusbar_zoom_slider));
        if (pZoomSlider != nullptr)
        {
            pZoomSlider->setValue((pZoomSlider->value() - 10) / 10 * 10);
        }
    }
    else if (strCmd == ID_chekcboxStatusbar)
    {
        QStatusBar* pStatusbar = statusBar();
        if (pStatusbar != nullptr)
            pStatusbar->setVisible(checked);
    }

    return RibbonFrameWindow::OnCommand(strCmd, checked);
}

void MainWindow::OnItemChanged(const QString &strId, int index, const QString &text)
{
    if (strId == ID_FontSizeCombobox)
    {
        ApplyFont();
    }
}


QWidget *MainWindow::CreateUserWidget(const QString &strId, QWidget *pParent)
{
    if (strId == ID_FontCombobox)
    {
        QFontComboBox* pFontCombo = new QFontComboBox(pParent);
        pFontCombo->setEditable(false);
        return pFontCombo;
    }
    else if (strId == ID_IndentLeftSpin || strId == ID_IndentRightSpin)
    {
        QDoubleSpinBox* pSpin = new QDoubleSpinBox(pParent);
        pSpin->setSuffix(QSTR(" 字符"));
        pSpin->setSingleStep(0.5);
        pSpin->setDecimals(1);
        pSpin->setRange(-99, 99);
        return pSpin;
    }
    else if (strId == ID_SpacingBeforeSpin || strId == ID_SpacingAfterSpin)
    {
        QDoubleSpinBox* pSpin = new QDoubleSpinBox(pParent);
        pSpin->setSuffix(QSTR(" 行"));
        pSpin->setSingleStep(0.5);
        pSpin->setDecimals(1);
        return pSpin;
    }
    else if (strId == ID_statusbar_zoom_slider)
    {
        QSlider* pZoomSlider = new QSlider(pParent);
        pZoomSlider->setOrientation(Qt::Horizontal);
        pZoomSlider->setFixedWidth(DPI(100));
        return pZoomSlider;
    }
    return nullptr;
}
