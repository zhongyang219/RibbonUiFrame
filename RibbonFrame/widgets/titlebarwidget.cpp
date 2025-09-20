#include "titlebarwidget.h"
#include "ribbonuipredefine.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QTimer>
#include "ribbonframewindow.h"
#include <QDebug>
#include <QMouseEvent>

static QToolButton* CreateTitleBarButton(const QString& objName, const QString& text, const QString& toolTip)
{
    QToolButton* pBtn = new QToolButton();
    pBtn->setObjectName(objName);
    pBtn->setText(text);
    pBtn->setToolTip(toolTip);
    return pBtn;
}


//////////////////////////////////////////////////////////////////////////////////////////////
TitleBarWidget::TitleBarWidget(QWidget *parent) : QWidget(parent)
{
    setObjectName("RibbonTitleBar");
    setAttribute(Qt::WA_StyledBackground);
    m_pMainWindow = dynamic_cast<RibbonFrameWindow*>(parent);

    setFixedHeight(DPI(30));
    m_pLayout = new QHBoxLayout();
    m_pLayout->setContentsMargins(DPI(6), 0, 0, 0);
    m_pLayout->setSpacing(DPI(4));
    setLayout(m_pLayout);

    //添加图标
    m_pIconLabel = new QLabel(this);
    m_pLayout->addWidget(m_pIconLabel);

    //添加分隔线
    QFrame* line = CreateSpacingLine();
    m_pLayout->addWidget(line);

    //添加标题
    m_pLayout->addWidget(m_pTitleLabel = new QLabel(qApp->applicationName()), 1);

    //添加右上角按钮
    QVBoxLayout* pTopRightLayout = new QVBoxLayout();
    pTopRightLayout->setContentsMargins(0, 0, 0, 0);
    pTopRightLayout->setSpacing(0);
    m_pLayout->addLayout(pTopRightLayout);

    QHBoxLayout* pButtonLayout = new QHBoxLayout();
    pButtonLayout->setContentsMargins(0, 0, 0, 0);
    pButtonLayout->setSpacing(0);
    pButtonLayout->addWidget(m_pMinBtn = CreateTitleBarButton("btnMenu_Min", QSTR("–"), QSTR("最小化")));
    pButtonLayout->addWidget(m_pMaxBtn = CreateTitleBarButton("btnMenu_Max", QSTR("□"), QSTR("最大化")));
    pButtonLayout->addWidget(m_pRestoreBtn = CreateTitleBarButton("btnMenu_Restore", QSTR("▽"), QSTR("还原")));
    pButtonLayout->addWidget(m_pCloseBtn = CreateTitleBarButton("btnMenu_Close", QSTR("╳"), QSTR("关闭")));
    pTopRightLayout->addLayout(pButtonLayout);
    pTopRightLayout->addStretch();

    //连接主窗口信号槽
    if (m_pMainWindow != nullptr)
    {
        connect(m_pMinBtn, SIGNAL(clicked()), m_pMainWindow, SLOT(showMinimized()));
        connect(m_pMaxBtn, SIGNAL(clicked()), m_pMainWindow, SLOT(showMaximized()));
        connect(m_pRestoreBtn, SIGNAL(clicked()), m_pMainWindow, SLOT(showNormal()));
        connect(m_pCloseBtn, SIGNAL(clicked()), m_pMainWindow, SLOT(close()));
    }

    UpdateMaximize();
}

void TitleBarWidget::AddQuickAccessBar(QWidget *pQuickAccessBar)
{
    //添加快速启动栏
    m_pLayout->insertWidget(1, pQuickAccessBar);
    //添加分隔线
    QFrame* line = CreateSpacingLine();
    m_pLayout->insertWidget(1, line);

}

void TitleBarWidget::OnWindowIconChanged(const QIcon &icon)
{
    const int iconSize = DPI(16);
    QPixmap iconPixmap(icon.pixmap(iconSize, iconSize));
    m_pIconLabel->setPixmap(iconPixmap.scaled(iconSize, iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

}

void TitleBarWidget::OnWindowTitleChanged(const QString &title)
{
    m_pTitleLabel->setText(title);
}

void TitleBarWidget::UpdateMaximize()
{
    if (m_pMainWindow != nullptr)
    {
        if (m_pMainWindow->isMaximized())
        {
            m_pRestoreBtn->show();
            m_pMaxBtn->hide();
        }
        else
        {
            m_pRestoreBtn->hide();
            m_pMaxBtn->show();
        }
    }
}

QFrame *TitleBarWidget::CreateSpacingLine()
{
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Plain);
    line->setFixedSize(DPI(1), DPI(16));
    return line;
}


void TitleBarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressedPos = event->globalPos();
        m_isPressed = true;
    }
}

void TitleBarWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = false;
    }
}

void TitleBarWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)      //双击时最大化/还原窗口
    {
        QWidget *pWindow = this->window();
        pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
    }
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed && m_pMainWindow != nullptr)
    {
        bool maximized = m_pMainWindow->isMaximized();
        QPoint curPos = event->globalPos();
//#ifdef Q_OS_WIN
//        if (maximized)
//        {
//            m_pMainWindow->showNormal();
//        }
//        m_pMainWindow->move(m_pMainWindow->pos() + curPos - m_pressedPos);
//        m_pressedPos = curPos;
//#else
        if (!maximized)
        {
            m_pMainWindow->move(m_pMainWindow->pos() + curPos - m_pressedPos);
            m_pressedPos = curPos;
        }
//#endif
    }
}


bool TitleBarWidget::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *pWidget = qobject_cast<QWidget *>(watched);
    if (pWidget == m_pMainWindow)
    {
        if (event->type() == QEvent::WindowStateChange)
        {
//            UpdateMaximize();
            QTimer::singleShot(50, this, SLOT(UpdateMaximize()));
        }
        else if (event->type() == QEvent::WindowTitleChange)
        {
            OnWindowTitleChanged(m_pMainWindow->windowTitle());
        }
        else if (event->type() == QEvent::WindowIconChange)
        {
            OnWindowIconChanged(m_pMainWindow->windowIcon());
        }

    }
    return QWidget::eventFilter(watched, event);
}
