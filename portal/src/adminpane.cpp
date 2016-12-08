#include <iostream>
#include "adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    user(user),
    mainLayout(new QHBoxLayout),
    tabs(new QTabWidget(this)),
    addClassButton(new QPushButton(addClassIcon, "Add Class")),
    currClassDialog(nullptr)
{
    //TODO: Add to layout somewhere
    tabs->setCornerWidget(addClassButton);
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
    refreshClassTabs();
    connect(addClassButton, &QPushButton::clicked, this, [this]{
        currClassDialog.reset(new AddClassDialog);
        currClassDialog->show();
        connect(currClassDialog.get(), &AddClassDialog::nameSubmitted,
                this, &AdminPane::createClassroom);
    });
}

void AdminPane::refreshClassTabs(){
    std::vector<ClassStats> classStats;
    LoqueClient client;
    auto status = client.getAllClassStats(user.userId, classStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't get the info.
        std::cerr << "ERROR: Unable to download class stats. Client returned status" <<
                     status << std::endl;
        return;
    }
    tabs->clear();
    for(auto& cstats : classStats){
        tabs->addTab(new ClassTab(cstats, user.userId), QString::fromStdString(cstats.className));
    }
}

AdminPane::~AdminPane()
{
}

void AdminPane::createClassroom(QString name){
    LoqueClient client;
    auto status = client.createClassroom(user.userId, name.toStdString());
    if (status == Status::DB_ERR) {

    } else if (status == Status::NETWORK_ERR) {

    } else if (status == Status::OK) {
        refreshClassTabs();
    }
}
