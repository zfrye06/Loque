#include <iostream>
#include "adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPane),
    user(user)
    // mainLayout(new QHBoxLayout(this)),
    // tabs(new QTabWidget(this)),
    // sidebar(new AdminSidebar(user, this))
{
    ui->setupUi(this);
    // mainLayout->addWidget(sidebar);
    // mainLayout->addWidget(tabs);
    // setLayout(mainLayout);

    // refreshClassTabs();
    ui->classList->addItem("CS 3505");
    ui->classList->addItem("MR. ROMERO");
    ui->currClassLabel->setText("CS 3505");
    ui->studentsLabel->setText("Students");
    ui->studentsList->addItem("John Romero");
    ui->studentsList->addItem("Lisa Stultz");
    ui->studentsList->addItem("Bernie (Sanderz)");
    ui->studentsList->addItem("Alex Dalton Christian");

    ui->classList->setAttribute(Qt::WA_MacShowFocusRect, 0);
}

AdminPane::~AdminPane() {
    delete ui;
}

void AdminPane::refreshClassTabs() {
    std::vector<ClassStats> classStats;
    LoqueClient client;
    auto status = client.getAllClassStats(user.userId, classStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't get the info.
        std::cerr << "ERROR: Unable to download class stats. " <<
            "Client returned status" << status << std::endl;
        return;
    }
    tabs->clear();
    for(auto& cstats : classStats){
        ClassTab *c = new ClassTab(user.userId, cstats);
        connect(c, &ClassTab::classCreated, this, &AdminPane::addClassTab);
        tabs->addTab(c, QString::fromStdString(cstats.className));
    }
}

void AdminPane::addClassTab(const ClassStats& cstats) {
    ClassTab *c = new ClassTab(user.userId, cstats);
    connect(c, &ClassTab::classCreated, this, &AdminPane::addClassTab);
    tabs->addTab(c, QString::fromStdString(cstats.className));
    tabs->setCurrentWidget(c);
}
