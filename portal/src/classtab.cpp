#include <iostream>
#include <QHeaderView>
#include "classtab.h"
#include <QToolButton>

ClassTab::ClassTab(ClassStats classStats, int teacherID, QWidget *parent) :
    QWidget(parent), cstats(classStats), teacherID(teacherID), currClassDialog(nullptr), levelDialog(nullptr), dcc(nullptr)
{
    LoqueClient client;
    client.getAllLevels(allLevels);

    initWidgets();
    setSummaryBox();
    setUserTable();
    setMapTable();
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
    userStatsLabel = new QLabel("Student Stats");
    levelStatsLabel = new QLabel("Level Stats");
    classIdLabel = new QLabel;
    QToolButton *levelButton = new QToolButton;
    QIcon lvl(QPixmap(":/assets/levelView.png"));
    levelButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    levelButton->setIcon(lvl);
    levelButton->setText("Level Settings");
    levelButton->setIconSize(QSize(75,75));
    levelButton->setFixedSize(QSize(100, 100));
    QToolButton *addClassButton = new QToolButton;
    QIcon plus(QPixmap(":/assets/plus.png"));
    addClassButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addClassButton->setIcon(plus);
    addClassButton->setText("Add Class");
    addClassButton->setIconSize(QSize(75,75));
    addClassButton->setFixedSize(QSize(100, 100));
    QPushButton *deleteClassButton = new QPushButton("Delete Class");
    deleteClassButton->setFixedWidth(100);

    summaryBox = new QGroupBox;
    summaryLayout = new QVBoxLayout;
    userStatsTable = new QTableWidget;
    levelStatsTable = new QTableWidget;
    mainLayout = new QVBoxLayout;
    topLayout = new QHBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;

    summaryLayout->addWidget(classIdLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classNameLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classPointsLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classTimeLabel, 0, Qt::AlignCenter);
    summaryBox->setLayout(summaryLayout);
    summaryBox->setFixedSize(200, 100);

    topLayout->addWidget(levelButton);
    topLayout->addWidget(summaryBox);
    topLayout->addWidget(addClassButton);
    bottomLayout->addWidget(deleteClassButton);
    mainLayout->addLayout(topLayout);

    mainLayout->addWidget(userStatsLabel);
    mainLayout->addWidget(userStatsTable);
    mainLayout->addWidget(levelStatsLabel);
    mainLayout->addWidget(levelStatsTable);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    LoqueClient client;
    client.getEnabledClassLevels(cstats.classId, enabledLevels);

    connect(addClassButton, &QToolButton::clicked, this, [this]{
        currClassDialog.reset(new AddClassDialog(teacherID));
        currClassDialog->show();
        connect(currClassDialog.get(), &AddClassDialog::classCreated, this, &ClassTab::classCreated);
    });

    connect(levelButton, &QToolButton::clicked, this, [this]{
        levelDialog.reset(new LevelSettingsDialog(cstats.classId, enabledLevels, allLevels, teacherID));
        levelDialog->show();
        connect(levelDialog.get(), &LevelSettingsDialog::refresh, this, &ClassTab::refresh);
    });

    connect(deleteClassButton, &QPushButton::clicked, this, [this]{
        dcc.reset(new DeleteClassConfirmation);
        dcc->show();
        connect(dcc.get(), &DeleteClassConfirmation::accepted, this, &ClassTab::deleteClass);
    });
}



void ClassTab::setSummaryBox(){
    int totalPoints = 0;
    int totalTime = 0;
    for(auto& user : cstats.studentStats){
        totalPoints += user.totalScore;
        totalTime += user.totalSecPlayed;
    }
    classIdLabel->setText("Class ID: " + QString::number(cstats.classId));
    classNameLabel->setText("Class Name: " + QString::fromStdString(cstats.className));
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

void ClassTab::setUserTable(){
    QStringList headers;
    headers.append("Student");
    headers.append("Total Score");
    headers.append("Time Played");

    int numlvls = allLevels.size();

    for(int i = 0; i < numlvls; i++){
        headers.append("Level " + QString::number(i+1) + " Completed");
    }

    userStatsTable->setRowCount(cstats.studentStats.size());
    userStatsTable->setColumnCount(3 + numlvls);
    userStatsTable->setHorizontalHeaderLabels(headers);
    userStatsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    int col = 3;
    for(auto& user : cstats.studentStats){
        QTableWidgetItem *nameCell = new QTableWidgetItem(QString::fromStdString(user.username));
        QTableWidgetItem *scoreCell = new QTableWidgetItem(QString::number(user.totalScore));
        QTableWidgetItem *timeCell = new QTableWidgetItem(QString::number(user.totalSecPlayed));
        nameCell->setTextAlignment(Qt::AlignCenter);
        scoreCell->setTextAlignment(Qt::AlignCenter);
        timeCell->setTextAlignment(Qt::AlignCenter);

        userStatsTable->setItem(row, 0, nameCell);
        userStatsTable->setItem(row, 1, scoreCell);
        userStatsTable->setItem(row, 2, timeCell);

        QTableWidgetItem *lvlCell = new QTableWidgetItem;
        for(int i = 0; i < numlvls; i++){
            lvlCell = new QTableWidgetItem;
            lvlCell->setBackgroundColor(getLevelColor(user, i));
            lvlCell->setTextAlignment(Qt::AlignCenter);
            userStatsTable->setItem(row, col++, lvlCell);
        }
        row++;
        col = 3;
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

void ClassTab::setMapTable(){
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
    int row = levelStatsTable->rowCount();
    std::map<int, std::string> levelMap;

    for(LevelInfo info : allLevels){
        levelMap[info.id] = info.name;
    }

    for(size_t i = 0; i < cstats.studentStats.size(); i++){
        const UserStats& user = cstats.studentStats.at(i);
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

void ClassTab::deleteClass(){
    LoqueClient client;
    auto status = client.deleteClass(cstats.classId);
    if(status == Status::OK){
        emit classDeleted();
    } else if(status == Status::DB_ERR){
        std::cout << "ERROR: Unable to delete class from the database. Client returned status " << status << std::endl;
    } else if(status == Status::NETWORK_ERR){
        std::cout << "ERROR: Unable to connect to the server. Client returned status " << status << std::endl;
    }
}

void ClassTab::refresh(){
    enabledLevels.clear();
    LoqueClient client;
    client.getEnabledClassLevels(cstats.classId, enabledLevels);
}
