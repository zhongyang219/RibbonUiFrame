#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ribbonframewindow.h"
#include <QTextEdit>

class MainWindow : public RibbonFrameWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    enum class DocFormat
    {
        Text,
        Html,
        Markdown,
        Undefined
    };

    void ApplyFont();
    void UpdateFontUi();
    void UpdateWindowTitle();
    DocFormat GetDocFormatByFileName(const QString& fileName);
    bool OpenFile(const QString& filePath, DocFormat format = DocFormat::Undefined);
    bool SaveFile(const QString& filePath, DocFormat format = DocFormat::Undefined);

private slots:
    void OnCurrentFontChanged(const QFont & font);
    void OnCursorPositionChanged();
    void OnCopyAvailable(bool yes);
    void OnUndoAvailable(bool available);
    void OnRedoAvailable(bool available);

private:
    QTextEdit m_edit;
    QString m_filePath;
    bool m_fontUiApplEnable = true;

    // RibbonFrameWindow interface
protected:
    virtual bool OnCommand(const QString &strCmd, bool checked) override;
    void OnItemChanged(const QString &strId, int index, const QString &text);
    virtual QWidget *CreateUserWidget(const QString &strId, QWidget *pParent) override;
};
#endif // MAINWINDOW_H
