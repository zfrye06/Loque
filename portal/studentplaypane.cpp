#include "studentplaypane.h"
#include "ui_studentplaypane.h"

StudentPlayPane::StudentPlayPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentPlayPane)
{
    ui->setupUi(this);
}

StudentPlayPane::~StudentPlayPane()
{
    delete ui;
}
