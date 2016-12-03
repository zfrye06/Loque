#include <iostream>
#include "classtab.h"
#include "ui_classtab.h"

ClassTab::ClassTab(int classID, int teacherID, QWidget *parent) :
    classID(classID), teacherID(teacherID), QWidget(parent), ui(new Ui::ClassTab)
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
    LoqueClient client;
    ClassStats stats;
    client.getClassStats(teacherID, classID, stats);

    QStringList headers;
    headers.append("Student");
    headers.append("Total Score");
    headers.append("Time Played");
    headers.append("Level 1");
    headers.append("Level 2");
    headers.append("Level 3");

    userStatsTable->setRowCount(stats.studentStats.size());
    userStatsTable->setColumnCount(6);
    userStatsTable->setHorizontalHeaderLabels(headers);
    userStatsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for(UserStats user : stats.studentStats){
        std::cout << user.totalScore << std::endl;
        QTableWidgetItem *nameCell = new QTableWidgetItem(QString::fromStdString(user.username));
        QTableWidgetItem *scoreCell = new QTableWidgetItem(QString::number(user.totalScore));
        QTableWidgetItem *timeCell = new QTableWidgetItem(QString::number(user.totalSecPlayed));
        QTableWidgetItem *lvl1Cell = new QTableWidgetItem();
        QTableWidgetItem *lvl2Cell = new QTableWidgetItem();
        QTableWidgetItem *lvl3Cell = new QTableWidgetItem();
        lvl1Cell->setBackgroundColor(getLevelColor(user, 1));
        lvl2Cell->setBackgroundColor(getLevelColor(user, 2));
        lvl3Cell->setBackgroundColor(getLevelColor(user, 3));
        userStatsTable->setItem(row, 0, nameCell);
        userStatsTable->setItem(row, 1, scoreCell);
        userStatsTable->setItem(row, 2, timeCell);
        userStatsTable->setItem(row, 3, lvl1Cell);
        userStatsTable->setItem(row, 4, lvl2Cell);
        userStatsTable->setItem(row++, 5, lvl3Cell);
    }

    for(int row = 0; row < userStatsTable->rowCount(); row++){
        for(int col = 0; col < userStatsTable->columnCount(); col++){
//            userStatsTable->itemAt(row, col)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

QColor ClassTab::getLevelColor(UserStats user, int levelID){
    QColor color;
    if(user.highScores.find(levelID) == user.highScores.end() ||user.highScores.at(levelID) == 0){
        color.setRgb(215, 0, 0, 100);
        color.setAlpha(100);
    } else {
        color.setRgb(0, 186, 6);
        color.setAlpha(100);
    }
    return color;
}

void ClassTab::setMapTable(){

}
