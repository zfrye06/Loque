#include "adminpane.h"
#include "ui_adminpane.h"

AdminPane::AdminPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPane)
{
    ui->setupUi(this);
}

AdminPane::~AdminPane()
{
    delete ui;
}
