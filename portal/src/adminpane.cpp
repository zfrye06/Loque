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
    LoqueClient client;
    std::vector<ClassStats> classStats;
    auto status = client.getAllClassStats(user.userId, classStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't download the userinfo.
        std::cerr << "ERROR: Unable to download class stats. Client returned status " <<
            status << std::endl;
        return;
    }
    for (auto& cstats : classStats){
        tabs->addTab(new ClassTab(cstats, user.userId), QString::fromStdString(cstats.className));
    }
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
    client.getAllClassStats(user.userId, classStats);
    tabs->clear();
    for(auto& cstats : classStats){
        tabs->addTab(new ClassTab(cstats, user.userId), QString::fromStdString(cstats.className));
    }
AdminPane::~AdminPane()
{
}

void AdminPane::createClassroom(QString name){
    LoqueClient client;
    auto status = client.createClassroom(user.userId, name.toStdString());
    if (status == Status::DB_ERR) {

    }else if (status == Status::NETWORK_ERR) {

    }else if (status == Status::OK) {
        refreshClassTabs();
    }
}
