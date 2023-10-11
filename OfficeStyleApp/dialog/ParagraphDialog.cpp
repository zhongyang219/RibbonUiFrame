#include "ParagraphDialog.h"
#include "ui_ParagraphDialog.h"

ParagraphDialog::ParagraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParagraphDialog)
{
    ui->setupUi(this);
}

ParagraphDialog::~ParagraphDialog()
{
    delete ui;
}
