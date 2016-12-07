#include <iostream>
#include <QHeaderView>
#include "classtab.h"

ClassTab::ClassTab(ClassStats classStats, int teacherID, QWidget *parent) :
    QWidget(parent), teacherID(teacherID), classID(classStats.classId)
{
    initWidgets();
    setSummaryBox(classStats);
    setEnabledLevels();
    setUserTable(classStats);
    setMapTable(classStats);
}

ClassTab::~ClassTab()
{
//    delete classSummaryLabel;
//    delete classNameLabel;
//    delete classPointsLabel;
//    delete classTimeLabel;
//    delete enabledLevelsLabel;
//    delete userStatsLabel;
//    delete mapStatsLabel;
//    delete ui;
//    delete userStatsTable;
//    delete levelStatsTable;
//    delete summaryBox;
//    delete levelArea;
//    delete mainLayout;
//    delete summaryLayout;
}

void ClassTab::initWidgets(){
    classSummaryLabel = new QLabel("Class Summary");
    classNameLabel = new QLabel;
    classPointsLabel = new QLabel;
    classTimeLabel = new QLabel;
    enabledLevelsLabel = new QLabel("Enabled Levels: ");
    userStatsLabel = new QLabel("Student Stats");
    mapStatsLabel = new QLabel("Map Stats");

    summaryBox = new QGroupBox;
    summaryLayout = new QVBoxLayout;
    userStatsTable = new QTableWidget;
    levelStatsTable = new QTableWidget;
    mainLayout = new QVBoxLayout;
    levelArea = new QGroupBox;

    summaryLayout->addWidget(classNameLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classPointsLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classTimeLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(enabledLevelsLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(levelArea, 0, Qt::AlignCenter);
    summaryBox->setLayout(summaryLayout);
    summaryBox->setFixedSize(400, 300);

    mainLayout->addWidget(summaryBox, 0, Qt::AlignCenter);
    mainLayout->addWidget(userStatsLabel);
    mainLayout->addWidget(userStatsTable);
    mainLayout->addWidget(mapStatsLabel);
    mainLayout->addWidget(levelStatsTable);
    setLayout(mainLayout);
}

void ClassTab::initConnections(){

}

void ClassTab::setSummaryBox(const ClassStats& classStats){
    int totalPoints = 0;
    int totalTime = 0;
    for(auto& user : classStats.studentStats){
        totalPoints += user.totalScore;
        totalTime += user.totalSecPlayed;
    }
    classNameLabel->setText("Class Name: " + QString::fromStdString(classStats.className));
    classPointsLabel->setText("Total Points: " + QString::number(totalPoints));
    classTimeLabel->setText("Total Time Played: " + getFormattedTime(totalTime));
}

QString ClassTab::getFormattedTime(int seconds){
    QString hourString;
    int hours = seconds / 3600;
    if(hours == 0){
        hourString = "";
    }
    else if(hours < 10){
        hourString = QString::number(0) + QString::number(seconds / 3600) + ":";
    } else{
        hourString = QString::number(seconds / 3600) + ":";
    }
    QString minuteString;
    int minutes = (seconds % 3600) / 60;
    if(minutes < 10){
        minuteString = QString::number(0) + QString::number((seconds % 3600) / 60) + ":";
    } else{
        minuteString = QString::number((seconds % 3600) / 60) + ":";
    }

    QString secString;
    int secs = (seconds % 3600) % 60;
    if(secs < 10){
        secString = QString::number(0) + QString::number((seconds % 3600) % 60);
    } else{
        secString = QString::number((seconds % 3600) % 60);
    }
    return  hourString + minuteString + secString;
}

void ClassTab::setUserTable(const ClassStats& classStats){
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
    for(auto& user : classStats.studentStats){
        QTableWidgetItem *nameCell = new QTableWidgetItem(QString::fromStdString(user.username));
        QTableWidgetItem *scoreCell = new QTableWidgetItem(QString::number(user.totalScore));
        QTableWidgetItem *timeCell = new QTableWidgetItem(QString::number(user.totalSecPlayed));
        QTableWidgetItem *lvl1Cell = new QTableWidgetItem();
        QTableWidgetItem *lvl2Cell = new QTableWidgetItem();
        QTableWidgetItem *lvl3Cell = new QTableWidgetItem();
        lvl1Cell->setBackgroundColor(getLevelColor(user, 1));
        lvl2Cell->setBackgroundColor(getLevelColor(user, 2));
        lvl3Cell->setBackgroundColor(getLevelColor(user, 3));
        nameCell->setTextAlignment(Qt::AlignCenter);
        scoreCell->setTextAlignment(Qt::AlignCenter);
        timeCell->setTextAlignment(Qt::AlignCenter);
        lvl1Cell->setTextAlignment(Qt::AlignCenter);
        lvl2Cell->setTextAlignment(Qt::AlignCenter);
        lvl3Cell->setTextAlignment(Qt::AlignCenter);
        userStatsTable->setItem(row, 0, nameCell);
        userStatsTable->setItem(row, 1, scoreCell);
        userStatsTable->setItem(row, 2, timeCell);
        userStatsTable->setItem(row, 3, lvl1Cell);
        userStatsTable->setItem(row, 4, lvl2Cell);
        userStatsTable->setItem(row++, 5, lvl3Cell);
    }
    userStatsTable->sortByColumn(0, Qt::AscendingOrder);
    userStatsTable->setSelectionMode(QAbstractItemView::NoSelection);
    userStatsTable->verticalHeader()->setVisible(false);
}

QColor ClassTab::getLevelColor(const UserStats& user, int levelID){
    QColor color;
    if(user.highScores.find(levelID) == user.highScores.end() ||user.highScores.at(levelID) == 0){
        color.setRgb(235, 16, 16);
    } else {
        color.setRgb(0, 186, 6);
    }
    return color;
}

void ClassTab::setMapTable(const ClassStats& classStats){
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
        const UserStats& user = classStats.studentStats.at(i);
        for(auto& kv : user.highScores){
            levelStatsTable->insertRow(row);
            QTableWidgetItem *studentNameCell = new QTableWidgetItem(QString::fromStdString(user.username));
            QTableWidgetItem *levIDCell = new QTableWidgetItem(QString::number(kv.first));
            QTableWidgetItem *levelNameCell = new QTableWidgetItem(QString::fromStdString(levelMap.at(kv.first)));
            QTableWidgetItem *scoreCell = new QTableWidgetItem(QString::number(kv.second));
            QTableWidgetItem *timeCell = new QTableWidgetItem(getFormattedTime(user.completionTimes.at(kv.first)));
            studentNameCell->setTextAlignment(Qt::AlignCenter);
            levIDCell->setTextAlignment(Qt::AlignCenter);
            levelNameCell->setTextAlignment(Qt::AlignCenter);
            scoreCell->setTextAlignment(Qt::AlignCenter);
            timeCell->setTextAlignment(Qt::AlignCenter);
            levelStatsTable->setItem(row, 0, studentNameCell);
            levelStatsTable->setItem(row, 1, levIDCell);
            levelStatsTable->setItem(row, 2, levelNameCell);
            levelStatsTable->setItem(row, 3, scoreCell);
            levelStatsTable->setItem(row++, 4, timeCell);
        }
    }
    levelStatsTable->sortByColumn(0, Qt::AscendingOrder);
    levelStatsTable->setSelectionMode(QAbstractItemView::NoSelection);
    levelStatsTable->verticalHeader()->setVisible(false);
}

void ClassTab::setEnabledLevels(){
    levelArea->setStyleSheet("QPushButton{background: url(:/assets/assets/candySky.jpg); background-position: top-left; width: 75; height: 75;}");
    LoqueClient client;
    std::vector<int> enabledLevels;
    std::vector<LevelInfo> allLevels;
    client.getEnabledClassLevels(classID, enabledLevels);
    client.getAllLevels(allLevels);
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *thumbnail;
    for(LevelInfo info: allLevels){
        thumbnail = new QPushButton;
        connect(thumbnail, &QPushButton::clicked, this, &ClassTab::toggleLevel);
        layout->addWidget(thumbnail);
        thumbnail->setCheckable(true);
        thumbnail->setText(QString::fromStdString(info.name));
        thumbnail->setChecked(std::find(enabledLevels.begin(), enabledLevels.end(), info.id) == enabledLevels.end());
        levelButtons[info.id] = thumbnail;
    }
    levelArea->setLayout(layout);
}

void ClassTab::toggleLevel(){
    LoqueClient client;
    for(int i = 0; i < levelButtons.size(); i++){
        if(levelButtons[i] == ((QPushButton*) QObject::sender())){
            QPushButton *btn = levelButtons[i];
            if(btn->isChecked()){
                client.disableLevel(teacherID, classID, i);
            }
            else{
                client.enableLevel(teacherID, classID, i);
            }
        }
    }
}
