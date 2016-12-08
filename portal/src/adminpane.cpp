#include "adminpane.h"
#include "classtab.h"
#include "addclassdialog.h"

AdminPane::AdminPane(int teacherID, QWidget *parent) :
    teacherID(teacherID),
    QWidget(parent)
{
    LoqueClient client;
    std::vector<ClassStats> classStats;
    client.getAllClassStats(teacherID, classStats);
    initWidgets(classStats);
    connect(addClassButton, &QPushButton::clicked, this, [this]{
        AddClassDialog *d = new AddClassDialog;
        d->show();
        connect(d, &AddClassDialog::nameSubmitted, this, &AdminPane::createClassroom);
    });
}

AdminPane::~AdminPane()
{
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

void AdminPane::createClassroom(QString name){
    LoqueClient client;
    ClassStats classStats;
    client.createClassroom(teacherID, name.toStdString(), classStats);
    tabs->addTab(new ClassTab(classStats, teacherID), QString::fromStdString(classStats.className));
    emit classCreated(classStats.classId);
}

void AdminPane::setUser(UserInfo user){

}

void AdminPane::updateClassStats(){

}
