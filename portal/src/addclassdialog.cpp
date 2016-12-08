#include <iostream>
#include "addclassdialog.h"
#include "ui_addclassdialog.h"

AddClassDialog::AddClassDialog(int userId, QWidget *parent) :
    userId(userId), QDialog(parent),
    ui(new Ui::AddClassDialog)
{
    ui->setupUi(this);
    ui->classNameEdit->setPlaceholderText("Enter Class Name...");
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]{
        createClassroom(ui->classNameEdit->text());
    });
}

AddClassDialog::~AddClassDialog()
{
    delete ui;
}

void AddClassDialog::createClassroom(QString name){
    LoqueClient client;
    ClassStats cstats;
    auto status = client.createClassroom(userId, name.toStdString(), cstats);
    emit classCreated(cstats.classId);
    if (status == Status::DB_ERR) {

    } else if (status == Status::NETWORK_ERR) {

    } else if (status == Status::OK) {

    }
}


