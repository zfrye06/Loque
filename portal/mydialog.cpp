#include "mydialog.h"
#include "ui_mydialog.h"
#include <iostream>
MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    std::cout << "MyDialog instantiated" << std::endl;
}

MyDialog::~MyDialog()
{
    delete ui;
}
