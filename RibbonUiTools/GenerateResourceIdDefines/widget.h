#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

namespace GenerateResourceIdDefines {
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void LoadConfig();
    void SaveConfig() const;

private slots:
    void on_browseBtn_clicked();

    void on_generateBtn_clicked();

    void on_copyBtn_clicked();

private:
    Ui::Widget *ui;
};

}

#endif // WIDGET_H
