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
#include <QClipboard>
#include "../mainwindow.h"

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
    int format = settings.value("format").toInt();
    if (format == 1)
        ui->qstringRadioBtn->setChecked(true);
    else if (format == 2)
        ui->structRadioBtn->setChecked(true);
    else
        ui->defineRadioBtn->setChecked(true);
}

void Widget::SaveConfig() const
{
    //保存设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    settings.setValue("prefix", ui->prefixEdit->text());
    int format = 0;
    if (ui->qstringRadioBtn->isChecked())
        format = 1;
    else if (ui->structRadioBtn->isChecked())
        format = 2;
    settings.setValue("format", format);
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

struct IdItem
{
    QString name;
    QString id;
    bool operator==(const IdItem& item) const
    {
        return id == item.id;
    }
};

void Widget::on_generateBtn_clicked()
{
    if (ui->pathEdit->text().isEmpty())
    {
        QMessageBox::warning(this, QString(), QSTR("请先打开 xml 文件！"));
        return;
    }
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

    //保存所有的id
    QList<IdItem> idList;
    IterateXmlElement(root, [&](const QDomElement& element){
        QString strId = element.attribute("id");
        if (!strId.isEmpty())
        {
            IdItem item;
            item.name = QString("%1%2").arg(ui->prefixEdit->text()).arg(strId);
            item.id = strId;
            if (!idList.contains(item))
                idList.push_back(item);
        }
    });

    QString contents;
    //宏定义格式
    if (ui->defineRadioBtn->isChecked())
    {
        Q_FOREACH(const auto& idItem, idList)
        {
            QString line = QString("#define %1 \"%3\"\r\n").arg(idItem.name).arg(idItem.id);
            contents += line;

        }
    }

    //const char*格式
    else if (ui->qstringRadioBtn->isChecked())
    {
        Q_FOREACH(const auto& idItem, idList)
        {
            QString line = QString("static const char* %1 = \"%3\";\r\n").arg(idItem.name).arg(idItem.id);
            contents += line;
        }
    }

    //结构体格式
    else if (ui->structRadioBtn->isChecked())
    {
        contents += "static struct _id\r\n{\r\n";
        contents += "    _id() :\r\n";
        Q_FOREACH(const auto& idItem, idList)
        {
            QString line = QString("        %1(\"%2\"),\r\n").arg(idItem.name).arg(idItem.id);
            contents += line;
        }
        //去掉最后一个逗号
        contents.chop(3);
        contents += "\r\n    {}\r\n";

        Q_FOREACH(const auto& idItem, idList)
        {
            QString line = QString("    const char* %1;\r\n").arg(idItem.name);
            contents += line;
        }
        contents += "}id;\r\n";
    }

    ui->plainTextEdit->setPlainText(contents);
}

void Widget::on_copyBtn_clicked()
{
    QApplication::clipboard()->setText(ui->plainTextEdit->toPlainText());
    MainWindow::Instance()->SetStatusBarText(QSTR("代码已复制到剪贴板").toUtf8().constData(), 10000);
}

}
