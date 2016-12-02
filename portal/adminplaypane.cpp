#include "adminplaypane.h"
#include "ui_adminplaypane.h"

AdminPlayPane::AdminPlayPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPlayPane)
{
    ui->setupUi(this);
}

AdminPlayPane::~AdminPlayPane()
{
    delete ui;
}
