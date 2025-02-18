#ifndef NAVIGATEWIDGET_H
#define NAVIGATEWIDGET_H

#include <QIcon>
#include <QWidget>
#include <QBoxLayout>

class QAbstractButton;

class NavigateWidget : public QWidget
{
    Q_OBJECT
public:
    NavigateWidget(Qt::Orientation orientation, QWidget* parent = nullptr);
    void AddItem(const QString& name, const QIcon& icon = QIcon());
    QString GetItemName(int index);
    void AddSpacing();
    void SetCurItem(int index);
    int GetCurItem() const;
    int GetItemCount() const;

signals:
    void curItemChanged(int index);
    void curItemChanged(const QString& text);

private slots:
    void OnBtnClicked();

private:
    QBoxLayout* m_pLayout;
    QBoxLayout* m_pBtnLayout;
    QList<QAbstractButton*> m_itemList;
    Qt::Orientation m_orientation;
};

class HorizontalNavigateWidget : public NavigateWidget
{
public:
    HorizontalNavigateWidget(QWidget* parent = nullptr)
        : NavigateWidget(Qt::Horizontal, parent)
    {}
};

class VerticalNavigateWidget : public NavigateWidget
{
public:
    VerticalNavigateWidget(QWidget* parent = nullptr)
        : NavigateWidget(Qt::Vertical, parent)
    {}
};

#endif // NAVIGATEWIDGET_H
