#include "classtab.h"
#include "ui_classtab.h"
#include "../../shared/loqueclient.h"

ClassTab::ClassTab(int classID, QWidget *parent) :
    classID(classID), QWidget(parent), ui(new Ui::ClassTab)
{
    ui->setupUi(this);
    initWidgets();
    setSummaryBox();
    setUserTable();
    setMapTable();
}

ClassTab::~ClassTab()
{
    delete ui;
}

void ClassTab::initWidgets(){
    classSummaryLabel = new QLabel("Class Summary");
    classNameLabel = new QLabel("CS 3505");
    classPointsLabel = new QLabel("Total Points: ");
    classTimeLabel = new QLabel("Total Time Played: ");
    enabledLevelsLabel = new QLabel("Enabled Levels: ");
    userStatsLabel = new QLabel("Student Stats");
    userLabel = new QLabel("Student");
    totalScoreLabel = new QLabel("Total Score");
    totalTimeLabel = new QLabel("Total Time Played");
    lvl1Label = new QLabel("Level 1");
    lvl2Label = new QLabel("Level 2");
    lvl3Label = new QLabel("Level 3");
    mapStatsLabel = new QLabel("Map Stats");
    mapUserLabel = new QLabel("Student");
    levelLabel = new QLabel("Level");
    levelNameLabel = new QLabel("Level Name");
    levelScoreLabel = new QLabel("High Score");
    completionTimeLabel = new QLabel("Best Time");

    summaryBox = new QGroupBox;
    summaryLayout = new QVBoxLayout;
    userStatsTable = new QTableWidget;
    levelStatsTable = new QTableWidget;
    mainLayout = new QVBoxLayout(this);

    summaryLayout->addWidget(classNameLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classPointsLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classTimeLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(enabledLevelsLabel, 0, Qt::AlignCenter);
    summaryBox->setLayout(summaryLayout);
    summaryBox->setFixedSize(300, 100);

    mainLayout->addWidget(summaryBox, 0, Qt::AlignCenter);
    mainLayout->addWidget(userStatsLabel);
    mainLayout->addWidget(userStatsTable);
    mainLayout->addWidget(mapStatsLabel);
    mainLayout->addWidget(levelStatsTable);
    setLayout(mainLayout);
}

void ClassTab::initConnections(){

}

void ClassTab::setSummaryBox(){

}

void ClassTab::setUserTable(){

}

void ClassTab::setMapTable(){

}
