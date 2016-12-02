#include "registerpane.h"
#include "ui_registerpane.h"

RegisterPane::RegisterPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPane)
{
    ui->setupUi(this);
}

RegisterPane::~RegisterPane()
{
    delete ui;
}
