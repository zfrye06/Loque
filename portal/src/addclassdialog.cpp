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
    std::cout << cstats.className << std::endl;

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


