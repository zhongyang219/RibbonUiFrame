#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <functional>
#include <QSettings>
#include "ribbonuipredefine.h"

namespace GenerateResourceIdDefines {

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::LoadConfig()
{
    //载入设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    ui->prefixEdit->setText(settings.value("prefix", "CMD_").toString());
}

void Widget::SaveConfig() const
{
    //保存设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    settings.setValue("prefix", ui->prefixEdit->text());
}


void Widget::on_browseBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), QSTR("xml文件 (*.xml);;文件文件 (*.txt)"));
    if (!fileName.isEmpty())
        ui->pathEdit->setText(fileName);
}

template <class Func>
static void IterateXmlElement(QDomElement& root, Func func)
{
    func(root);
    QDomNodeList childNode = root.childNodes();
    for(int i = 0; i < childNode.count(); i++)
    {
        QDomElement nodeInfo = childNode.at(i).toElement();
        IterateXmlElement(nodeInfo, func);
    }
}

void Widget::on_generateBtn_clicked()
{
    if (ui->pathEdit->text().isEmpty())
    {
        QMessageBox::warning(this, QString(), QSTR("请先打开 xml 文件！"));
        return;
    }
    QString contents;
    QFile file(ui->pathEdit->text());
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString strInfo = QSTR("打开 xml 文件失败！");
        QMessageBox::critical(this, QString(), strInfo);
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file, false))
        return;

    QDomElement root = doc.documentElement();
    if (root.isNull())
    {
        QString strInfo = QSTR("MainFrame.xml文件找不到节点！");
        QMessageBox::critical(this, QString(), strInfo);
        return;
    }

    IterateXmlElement(root, [&](const QDomElement& element){
        QString strId = element.attribute("id");
        if (!strId.isEmpty())
        {
            QString defLine = QString("#define %1%2 \"%3\"\r\n").arg(ui->prefixEdit->text()).arg(strId).arg(strId);
            contents += defLine;
        }
    });
    ui->plainTextEdit->setPlainText(contents);
}

}
