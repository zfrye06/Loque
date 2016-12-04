#include <iostream>
#include "classtab.h"
#include "ui_classtab.h"

ClassTab::ClassTab(ClassStats classStats, QWidget *parent) :
    QWidget(parent), ui(new Ui::ClassTab)
{
    ui->setupUi(this);
    initWidgets();
    setSummaryBox(classStats);
    setUserTable(classStats);
    setMapTable(classStats);
}

ClassTab::~ClassTab()
{
    delete ui;
}

void ClassTab::initWidgets(){
    classSummaryLabel = new QLabel("Class Summary");
    classNameLabel = new QLabel;
    classPointsLabel = new QLabel;
    classTimeLabel = new QLabel;
    enabledLevelsLabel = new QLabel("Enabled Levels: ");
    userStatsLabel = new QLabel("Student Stats");
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

void ClassTab::setSummaryBox(ClassStats classStats){
    int totalPoints = 0;
    int totalTime = 0;
    for(UserStats user : classStats.studentStats){
        totalPoints += user.totalScore;
        totalTime += user.totalSecPlayed;
    }
    std::cout << classStats.className << std::endl;
    classNameLabel->setText("Class Name: " + QString::fromStdString(classStats.className));
    classPointsLabel->setText("Total Points: " + QString::number(totalPoints));
    classTimeLabel->setText("Total Time Played: " + getFormattedTime(totalTime));
}

QString ClassTab::getFormattedTime(int seconds){
    QString hourString;
    int hours = seconds / 3600;
    if(hours < 10){
        hourString = QString::number(0) + QString::number(seconds / 3600);
    } else{
        hourString = QString::number(seconds / 3600);
    }
    QString minuteString;
    int minutes = (seconds % 3600) / 60;
    if(minutes < 10){
        minuteString = QString::number(0) + QString::number((seconds % 3600) / 60);
    } else{
        minuteString = QString::number((seconds % 3600) / 60);
    }

    QString secString;
    int secs = (seconds % 3600) % 60;
    if(secs < 10){
        secString = QString::number(0) + QString::number((seconds % 3600) % 60);
    } else{
        secString = QString::number((seconds % 3600) % 60);
    }
    return  hourString + QString::fromStdString(":") + minuteString + QString::fromStdString(":") + secString;
}

void ClassTab::setUserTable(ClassStats classStats){
    QStringList headers;
    headers.append("Student");
    headers.append("Total Score");
    headers.append("Time Played");
    headers.append("Level 1 Completed");
    headers.append("Level 2 Completed");
    headers.append("Level 3 Completed");

    userStatsTable->setRowCount(classStats.studentStats.size());
    userStatsTable->setColumnCount(6);
    userStatsTable->setHorizontalHeaderLabels(headers);
    userStatsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for(UserStats user : classStats.studentStats){
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
    userStatsTable->sortByColumn(0, Qt::AscendingOrder);
    userStatsTable->setSelectionMode(QAbstractItemView::NoSelection);
}

QColor ClassTab::getLevelColor(UserStats user, int levelID){
    QColor color;
    if(user.highScores.find(levelID) == user.highScores.end() ||user.highScores.at(levelID) == 0){
        color.setRgb(235, 16, 16);
    } else {
        color.setRgb(0, 186, 6);
    }
    return color;
}

void ClassTab::setMapTable(ClassStats classStats){
    QStringList headers;
    headers.append("Student");
    headers.append("Level ID");
    headers.append("Level Name");
    headers.append("Level Score");
    headers.append("Completion Time");
    levelStatsTable->setColumnCount(5);
    levelStatsTable->setHorizontalHeaderLabels(headers);
    levelStatsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    LoqueClient client;
    std::vector<LevelInfo> levels;
    client.getAllLevels(levels);
    int row = levelStatsTable->rowCount();
    std::map<int, std::string> levelMap;

    for(LevelInfo info : levels){
        levelMap[info.id] = info.name;
    }

    for(int i = 0; i < classStats.studentStats.size(); i++){
        UserStats user = classStats.studentStats.at(i);
        for(auto kv : user.highScores){
            levelStatsTable->insertRow(row);
            QTableWidgetItem *studentNameCell = new QTableWidgetItem(QString::fromStdString(user.username));
            QTableWidgetItem *levIDCell = new QTableWidgetItem(QString::number(kv.first));
            QTableWidgetItem *levelNameCell = new QTableWidgetItem(QString::fromStdString(levelMap.at(kv.first)));
            QTableWidgetItem *scoreCell = new QTableWidgetItem(QString::number(kv.second));
            QTableWidgetItem *timeCell = new QTableWidgetItem(getFormattedTime(0));
            levelStatsTable->setItem(row, 0, studentNameCell);
            levelStatsTable->setItem(row, 1, levIDCell);
            levelStatsTable->setItem(row, 2, levelNameCell);
            levelStatsTable->setItem(row, 3, scoreCell);
            levelStatsTable->setItem(row++, 4, timeCell);
        }
    }
    levelStatsTable->sortByColumn(0, Qt::AscendingOrder);
    levelStatsTable->setSelectionMode(QAbstractItemView::NoSelection);
}
