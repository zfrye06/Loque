#include "adminpane.h"
#include "ui_adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(int teacherID, QWidget *parent) :
    teacherID(teacherID),
    QWidget(parent),
    ui(new Ui::AdminPane)
{
    ui->setupUi(this);
    LoqueClient client;
    std::vector<ClassStats> classStats;
    client.getAllClassStats(teacherID, classStats);
    initWidgets(classStats);
    connect(addClassButton, &QPushButton::clicked, this, [this]{
//        client.addClassroom(teacherID, )
    });
}

AdminPane::~AdminPane()
{
    delete ui;
}

void AdminPane::initWidgets(const std::vector<ClassStats>& classStats){
    tabs = new QTabWidget(this);
    mainLayout = new QHBoxLayout;

    //TODO: Add to layout somewhere
    addClassButton = new QPushButton(addClassIcon, "Add Class");
    tabs->setCornerWidget(addClassButton);


    for(auto& cstats : classStats){
        tabs->addTab(new ClassTab(cstats, teacherID), QString::fromStdString(cstats.className));
    }
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
}

void AdminPane::setUser(UserInfo user){

}

void AdminPane::updateClassStats(){

}
