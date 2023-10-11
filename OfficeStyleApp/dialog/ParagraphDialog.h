#ifndef PARAGRAPHDIALOG_H
#define PARAGRAPHDIALOG_H

#include <QDialog>

namespace Ui {
class ParagraphDialog;
}

class ParagraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParagraphDialog(QWidget *parent = nullptr);
    ~ParagraphDialog();

private:
    Ui::ParagraphDialog *ui;
};

#endif // PARAGRAPHDIALOG_H
