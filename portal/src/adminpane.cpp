#include <iostream>
#include "adminpane.h"
#include "classtab.h"

AdminPane::AdminPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    user(user),
    allClassStats(new std::vector<ClassStats>()),
    activeClassIdx(-1),
    ui(new Ui::AdminPane)
{
    ui->setupUi(this);
    ui->classList->setAttribute(Qt::WA_MacShowFocusRect, 0);

    connect(ui->classList, &QListWidget::currentRowChanged,
            this, &AdminPane::classClicked);

    connect(ui->clcreateClassButton, &QPushButton::clicked,
            this, &AdminPane::showCreateClassDialog);

    connect(ui->ncCreateClassButton, &QPushButton::clicked,
            this, &AdminPane::showCreateClassDialog);

    connect(ui->exportHtmlButton, &QPushButton::clicked,
            this, [this] {

    });

    connect(ui->levelSettingsButton, &QPushButton::clicked,
            this, [this] {

    });

    connect(ui->deleteClassButton, &QPushButton::clicked,
            this, [this] {

    });

    refreshClassTabs();
}

AdminPane::~AdminPane() {
    delete ui;
}

void AdminPane::refreshClassTabs() {
    std::unique_ptr<std::vector<ClassStats> > newClassStats(new std::vector<ClassStats>);
    LoqueClient client;
    auto status = client.getAllClassStats(user.userId, *newClassStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't get the info.
        std::cerr << "ERROR: Unable to download class stats. " <<
            "Client returned status" << status << std::endl;
        return;
    }
    allClassStats.swap(newClassStats);
    ui->classList->clear();
    ui->studentsList->clear();
    if (allClassStats->size() == 0) {
        activeClassIdx = -1;
        ui->stackedWidget->setCurrentWidget(ui->noClassesPage);
        return;
    }
    for (auto& classStats : *allClassStats) {
        ui->classList->addItem(QString::fromStdString(classStats.className));
    }
    classClicked(0);
}

void AdminPane::classClicked(int row) {
    if (row == -1) return;
    activeClassIdx = row;
    const ClassStats& currStats = allClassStats->at(activeClassIdx);
    ui->studentsList->clear();
    for (auto& ustats : currStats.studentStats) {
        ui->studentsList->addItem(QString::fromStdString(ustats.username));
    }
    ui->currClassLabel->setText(QString::fromStdString(currStats.className));
}

void AdminPane::showCreateClassDialog() {

}
