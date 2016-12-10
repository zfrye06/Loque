#include "deleteclassconfirmation.h"
#include "ui_deleteclassconfirmation.h"

DeleteClassConfirmation::DeleteClassConfirmation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteClassConfirmation)
{
    ui->setupUi(this);
}

DeleteClassConfirmation::~DeleteClassConfirmation()
{
    delete ui;
}
