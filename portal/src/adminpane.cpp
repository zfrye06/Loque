#include <iostream>
#include "adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    user(user),
    mainLayout(new QHBoxLayout(this)),
    tabs(new QTabWidget(this)),
    sidebar(new AdminSidebar(user, this))
{
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);

    refreshClassTabs();
}

void AdminPane::refreshClassTabs()
{
    std::vector<ClassStats> classStats;
    LoqueClient client;
    auto status = client.getAllClassStats(user.userId, classStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't get the info.
        std::cerr << "ERROR: Unable to download class stats. Client returned status" << status << std::endl;
        return;
    }
    tabs->clear();
    connect(sidebar, &AdminSidebar::classCreated, this, &AdminPane::addClassTab);
    connect(sidebar, &AdminSidebar::classTabChanged, this, &AdminPane::changeClassTab);
    for(auto& cstats : classStats){
        ClassTab *c = new ClassTab(cstats, user.userId);
        connect(c, &ClassTab::classCreated, this, &AdminPane::addClassTab);
        tabs->addTab(c, QString::fromStdString(cstats.className));
    }
}

AdminPane::~AdminPane()
{
}

void AdminPane::addClassTab(ClassStats cstats)
{
    ClassTab *c = new ClassTab(cstats, user.userId);
    connect(c, &ClassTab::classCreated, this, &AdminPane::addClassTab);
    tabs->addTab(c, QString::fromStdString(cstats.className));
    tabs->setCurrentWidget(c);
}

void AdminPane::changeClassTab(int index)
{
    tabs->setCurrentIndex(index);
}
