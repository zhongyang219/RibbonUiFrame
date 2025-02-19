#include "navigatewidget.h"
#include <QVariant>
#include <QPushButton>
#include <QToolButton>
#include "ribbonuipredefine.h"

NavigateWidget::NavigateWidget(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent),
      m_orientation(orientation)
{
    if (orientation == Qt::Horizontal)
        setProperty("nav", "top");
    else
        setProperty("nav", "left");
    setAttribute(Qt::WA_StyledBackground);

    if (orientation == Qt::Horizontal)
        m_pLayout = new QHBoxLayout;
    else
        m_pLayout = new QVBoxLayout;
    m_pLayout->setContentsMargins(0, 0, 0, 0);
    m_pLayout->setSpacing(0);
    setLayout(m_pLayout);

    QWidget* pButtonWidget = new QWidget(this);
    m_pLayout->addWidget(pButtonWidget);

    if (orientation == Qt::Horizontal)
        m_pBtnLayout = new QHBoxLayout;
    else
        m_pBtnLayout = new QVBoxLayout;
    m_pBtnLayout->setContentsMargins(0, 0, 0, 0);
    m_pBtnLayout->setSpacing(0);
    pButtonWidget->setLayout(m_pBtnLayout);
}

void NavigateWidget::AddItem(const QString &name, const QIcon &icon)
{
    QAbstractButton* pBtn = nullptr;
    if (m_orientation == Qt::Horizontal)
    {
        QToolButton* pToolBtn = new QToolButton(this);
        pBtn = pToolBtn;
        pBtn->setText(name);
        if (!icon.isNull())
            pBtn->setIcon(icon);
        pToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
    else
    {
        pBtn = new QPushButton(icon, name, this);
        QString style = QString("QPushButton { text-align: left; padding-left: %1px; }").arg(DPI(6));
        pBtn->setStyleSheet(style);
    }
    connect(pBtn, SIGNAL(clicked()), this, SLOT(OnBtnClicked()));
    pBtn->setCheckable(true);
    pBtn->setAutoExclusive(true);
    m_pBtnLayout->addWidget(pBtn);
    pBtn->setProperty("index", m_itemList.size());
    m_itemList.push_back(pBtn);
}

QString NavigateWidget::GetItemName(int index)
{
    QAbstractButton* pBtn = m_itemList.value(index);
    if (pBtn != nullptr)
        return pBtn->text();
    return QString();
}

void NavigateWidget::AddSpacing()
{
    m_pLayout->addStretch();
}

void NavigateWidget::SetCurItem(int index)
{
    QAbstractButton* pBtn = m_itemList.value(index);
    if (pBtn != nullptr)
    {
        pBtn->setChecked(true);
        emit curItemChanged(index);
        emit curItemChanged(pBtn->text());
    }
}

int NavigateWidget::GetCurItem() const
{
    int index = 0;
    Q_FOREACH(QAbstractButton* pBtn, m_itemList)
    {
        if (pBtn != nullptr && pBtn->isChecked())
            return index;
        index++;
    }
    return -1;
}

int NavigateWidget::GetItemCount() const
{
    return m_itemList.size();
}

void NavigateWidget::OnBtnClicked()
{
    int index = QObject::sender()->property("index").toInt();
    QString name = GetItemName(index);
    emit curItemChanged(index);
    emit curItemChanged(name);
}
