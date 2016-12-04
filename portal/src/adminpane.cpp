#include "adminpane.h"
#include "ui_adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(int teacherID, QWidget *parent) :
    teacherID(teacherID),
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
    tabs->addTab(new ClassTab(2, teacherID), tr("Class 1"));
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
}

void AdminPane::initConnections(){

}

void AdminPane::setUser(UserInfo user) {
    
}

void AdminPane::updateClassStats() {
    
}
