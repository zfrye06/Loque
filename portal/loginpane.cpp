#include "loginpane.h"
#include "ui_loginpane.h"

LoginPane::LoginPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPane)
{
    ui->setupUi(this);
}

LoginPane::~LoginPane()
{
    delete ui;
}
