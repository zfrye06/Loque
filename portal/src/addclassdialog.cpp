#include "addclassdialog.h"
#include "ui_addclassdialog.h"

AddClassDialog::AddClassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClassDialog)
{
    ui->setupUi(this);
    ui->classNameEdit->setPlaceholderText("Enter Class Name...");
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]{
        emit nameSubmitted(ui->classNameEdit->text());
    });
}

AddClassDialog::~AddClassDialog()
{
    delete ui;
}


