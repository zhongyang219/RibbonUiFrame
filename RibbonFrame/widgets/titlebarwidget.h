#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
class RibbonFrameWindow;

class TitleBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBarWidget(QWidget *parent = nullptr);

    /**
     * @brief       设置快速启动栏，必须在Init函数之前调用
     * @param[in]	pQuickAccessBar 快速启动栏
     */
    void AddQuickAccessBar(QWidget* pQuickAccessBar);

signals:

public slots:
    void OnWindowIconChanged(const QIcon &icon);
    void OnWindowTitleChanged(const QString &title);

private slots:
    //更新最大化/还原按钮的状态
    void UpdateMaximize();

private:
    QFrame* CreateSpacingLine();

private:
    RibbonFrameWindow* m_pMainWindow;
    QHBoxLayout* m_pLayout;
    QToolButton* m_pMinBtn;
    QToolButton* m_pMaxBtn;
    QToolButton* m_pRestoreBtn;
    QToolButton* m_pCloseBtn;
    QLabel* m_pIconLabel;
    QLabel* m_pTitleLabel;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // TITLEBARWIDGET_H
