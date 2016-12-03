#include "adminpane.h"
#include "ui_adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPane)
{
    ui->setupUi(this);
    initWidgets();
}

AdminPane::~AdminPane()
{
    delete ui;
}

void AdminPane::initWidgets(){
    tabs = new QTabWidget(this);
    mainLayout = new QHBoxLayout;
    addClassButton = new QPushButton(addClassIcon, "Add Class");
    tabs->addTab(new ClassTab(1), tr("Class 1"));
    tabs->addTab(new ClassTab(2), tr("Class 2"));
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
}

void AdminPane::initConnections(){

}
