#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QSettings>
#include <QPainter>
#include <QMouseEvent>
#include <QListView>
#include <QColorDialog>
#include "ribbonuipredefine.h"
#include "RibbonFrameHelper.h"


/////////////////////////////////////////////////////////////////////////////////////
void SettingsDialog::Data::Save() const
{
    //保存设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    settings.setValue("ribbonHideEnable", static_cast<int>(ribbonHideEnable));
    settings.setValue("ribbonPin", static_cast<int>(ribbonPin));
    settings.setValue("ribbonDoubleClickEnable", static_cast<int>(ribbonDoubleClickEnable));
    settings.setValue("showWhenTabClicked", static_cast<int>(showWhenTabClicked));
    settings.setValue("showLeftNaviBar", static_cast<int>(showLeftNaviBar));
    settings.setValue("showStatusBar", static_cast<int>(showStatusBar));
}

void SettingsDialog::Data::Load()
{
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    ribbonHideEnable = settings.value("ribbonHideEnable", false).toBool();
    if (!ribbonHideEnable)
        ribbonPin = true;
    else
        ribbonPin = settings.value("ribbonPin", true).toBool();
    ribbonDoubleClickEnable = settings.value("ribbonDoubleClickEnable", false).toBool();
    showWhenTabClicked = static_cast<ShowWhenTabClicked>(settings.value("showWhenTabClicked", 0).toInt());
    showLeftNaviBar = settings.value("showLeftNaviBar", false).toBool();
//#ifdef QT_DEBUG
//    showLeftNaviBar = true;
//#endif
    showStatusBar = settings.value("showStatusBar", true).toBool();
}

/////////////////////////////////////////////////////////////////////////////////
SettingsDialog::SettingsDialog(IRibbonStyle* ribbonStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_ribbonStyle(ribbonStyle)
{
    ui->setupUi(this);
    resize(DPI(380), DPI(280));

    //禁用ComboBox鼠标滚轮
    WheelEventFilter* filter = new WheelEventFilter(this);
    ui->showWhenTabClickedComboBox->installEventFilter(filter);
    ui->selectStyleCombo->installEventFilter(filter);

    ui->showWhenTabClickedComboBox->setView(new QListView);
    ui->showWhenTabClickedComboBox->addItem(u8"无");
    ui->showWhenTabClickedComboBox->addItem(u8"显示功能区");
    ui->showWhenTabClickedComboBox->addItem(u8"显示菜单");

    if (m_ribbonStyle != nullptr)
    {
        //初始化主题下拉列表
        ui->selectStyleCombo->setView(new QListView);
        QStringList styleNameList;
        m_ribbonStyle->GetAllStyleNames(styleNameList);
        ui->selectStyleCombo->addItem(u8"默认主题");        //添加默认主题
        for (const auto& styleName : styleNameList)
        {
            ui->selectStyleCombo->addItem(styleName, styleName);
        }
        ui->selectStyleCombo->setCurrentText(m_ribbonStyle->GetCurrentStyle());

        ui->followSystemColorModeCheck->setChecked(m_ribbonStyle->IsFollowingSystemColorMode());

        //添加当前颜色控件
        m_curColor = new ColorIndicatorWidget(m_ribbonStyle->GetThemeColor(), u8"当前的主题颜色", this);
        m_curColor->setFixedSize(QSize(DPI(28), DPI(22)));
        ui->themeColorHLayout->insertWidget(0, m_curColor);

        //添加预置颜色控件
        ui->colorPresetGroupbox->layout()->setSpacing(DPI(10));
        ColorIndicatorWidget* colorWord = new ColorIndicatorWidget(QColor("#2b579a"), "Micorsoft Word", this);
        ColorIndicatorWidget* colorExcel = new ColorIndicatorWidget(QColor("#2a724b"), "Micorsoft Excel", this);
        ColorIndicatorWidget* colorPowerPoint = new ColorIndicatorWidget(QColor("#b7472a"), "Micorsoft PowerPoint", this);
        ColorIndicatorWidget* colorOneNote = new ColorIndicatorWidget(QColor("#80397b"), "Micorsoft OneNote", this);
        connect(colorWord, SIGNAL(colorChanged(const QColor&)), m_curColor, SLOT(SetColor(const QColor&)));
        connect(colorExcel, SIGNAL(colorChanged(const QColor&)), m_curColor, SLOT(SetColor(const QColor&)));
        connect(colorPowerPoint, SIGNAL(colorChanged(const QColor&)), m_curColor, SLOT(SetColor(const QColor&)));
        connect(colorOneNote, SIGNAL(colorChanged(const QColor&)), m_curColor, SLOT(SetColor(const QColor&)));
        ui->colorPresetGroupbox->layout()->addWidget(colorWord);
        ui->colorPresetGroupbox->layout()->addWidget(colorExcel);
        ui->colorPresetGroupbox->layout()->addWidget(colorPowerPoint);
        ui->colorPresetGroupbox->layout()->addWidget(colorOneNote);

        connect(ui->btnCustomColor, SIGNAL(clicked()), this, SLOT(OnBorwseThemeColor()));
        ui->followSystemThemeColorCheck->setChecked(m_ribbonStyle->IsFollowingSystemThemeColor());
    }
    else
    {
        //没有获取到主题接口，禁用主题设置相关控件
        ui->label_4->setEnabled(false);
        ui->selectStyleCombo->setEnabled(false);
        ui->followSystemColorModeCheck->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->colorPresetGroupbox->setEnabled(false);
        ui->btnCustomColor->setEnabled(false);
        ui->followSystemThemeColorCheck->setEnabled(false);
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::SetData(Data data)
{
    ui->enableHideRibbonCheck->setChecked(data.ribbonHideEnable);
    ui->ribbonPinCheck->setChecked(data.ribbonPin);
    ui->doubleClickShowHideRibbonCheck->setChecked(data.ribbonDoubleClickEnable);
    ui->showWhenTabClickedComboBox->setCurrentIndex(static_cast<int>(data.showWhenTabClicked));
    ui->showLeftNaviBarCheck->setChecked(data.showLeftNaviBar);
    ui->showStatusbarCheck->setChecked(data.showStatusBar);
    EnableControl();
}

SettingsDialog::Data SettingsDialog::GetData() const
{
    Data data;
    data.ribbonHideEnable = ui->enableHideRibbonCheck->isChecked();
    data.ribbonPin = ui->ribbonPinCheck->isChecked();
    data.ribbonDoubleClickEnable = ui->doubleClickShowHideRibbonCheck->isChecked();
    data.showWhenTabClicked = static_cast<Data::ShowWhenTabClicked>(ui->showWhenTabClickedComboBox->currentIndex());
    if (!data.ribbonHideEnable)
        data.ribbonPin = true;
    data.showLeftNaviBar = ui->showLeftNaviBarCheck->isChecked();
    data.showStatusBar = ui->showStatusbarCheck->isChecked();

    return data;
}

void SettingsDialog::EnableControl()
{
    bool enable = ui->enableHideRibbonCheck->isChecked();
    ui->ribbonPinCheck->setEnabled(enable);
    ui->doubleClickShowHideRibbonCheck->setEnabled(enable);
    ui->showWhenTabClickedComboBox->setEnabled(enable);
}

void SettingsDialog::accept()
{
    //点击确定时保存主题设置
    if (m_ribbonStyle != nullptr)
    {
        m_ribbonStyle->SetCurrentStyle(ui->selectStyleCombo->currentData().toString());
        m_ribbonStyle->SetFollowingSystemColorMode(ui->followSystemColorModeCheck->isChecked());
        m_ribbonStyle->SetThemeColor(m_curColor->GetColor());
        m_ribbonStyle->SetFollowingSystemThemeColor(ui->followSystemThemeColorCheck->isChecked());
    }

    QDialog::accept();
}

void SettingsDialog::on_enableHideRibbonCheck_stateChanged(int)
{
    EnableControl();
}

void SettingsDialog::OnBorwseThemeColor()
{
    if (m_ribbonStyle != nullptr)
    {
        QColorDialog dlg(m_ribbonStyle->GetThemeColor());
        if (dlg.exec() == QDialog::Accepted)
        {
            m_curColor->SetColor(dlg.currentColor());
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
ColorIndicatorWidget::ColorIndicatorWidget(const QColor& color, const QString& toolTip, QWidget* parent)
    : QWidget(parent), m_color(color)
{
    setFixedSize(QSize(DPI(20), DPI(20)));
    setToolTip(toolTip);
}

QColor ColorIndicatorWidget::GetColor() const
{
    return m_color;
}

void ColorIndicatorWidget::SetColor(const QColor& color)
{
    m_color = color;
    update();
}

void ColorIndicatorWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), m_color);
}

void ColorIndicatorWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit colorChanged(m_color);
    }

}
