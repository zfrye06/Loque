#include <iostream>
#include "createclassdialog.h"
#include "ui_addclassdialog.h"

CreateClassDialog::CreateClassDialog(int userId, QWidget *parent) :
    userId(userId), QDialog(parent),
    ui(new Ui::AddClassDialog)
{
    ui->setupUi(this);
    ui->classNameEdit->setPlaceholderText("Enter Class Name...");
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]{
        createClassroom(ui->classNameEdit->text());
    });
}

CreateClassDialog::~CreateClassDialog()
{
    delete ui;
}

void CreateClassDialog::createClassroom(QString name){
    LoqueClient client;
    ClassStats cstats;
    auto status = client.createClassroom(userId, name.toStdString(), cstats);

    if (status == Status::DB_ERR) {
        QMessageBox message;
        message.setText("Class does not exist.");
        message.setWindowTitle("Error");
        message.show();
    } else if (status == Status::NETWORK_ERR) {
        QMessageBox message;
        message.setText("Cannot connect to the server.");
        message.setWindowTitle("Error");
        message.show();
    } else if (status == Status::OK) {
        emit classCreated(cstats);
    }
}


