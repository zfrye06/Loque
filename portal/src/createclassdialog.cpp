#include <iostream>
#include "createclassdialog.h"
#include "ui_addclassdialog.h"

CreateClassDialog::CreateClassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClassDialog)
{
    ui->setupUi(this);
    ui->classNameEdit->setPlaceholderText("Enter Class Name...");
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this] {
        emit createClass(ui->classNameEdit->text().toStdString());
    });
}

CreateClassDialog::~CreateClassDialog() {
    delete ui;
}


