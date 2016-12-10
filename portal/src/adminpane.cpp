#include <QFileDialog>
#include <fstream>
#include <iostream>
#include "adminpane.h"
#include "classtab.h"
#include "report.h"

AdminPane::AdminPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    user(user),
    allClassStats(new std::vector<ClassStats>()),
    activeClassIdx(0),
    ui(new Ui::AdminPane),
    createClassDialog(nullptr),
    levelSettingsDialog(nullptr),
    deleteClassConfirmation(nullptr)
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
            this, &AdminPane::showHtmlReportDialog); 

    connect(ui->levelSettingsButton, &QPushButton::clicked,
            this, [this] {
        if (activeClassIdx == -1) return;
        auto& activeClass = this->allClassStats->at(activeClassIdx);
        int activeClassId = activeClass.classId;
        levelSettingsDialog.reset(new LevelSettingsDialog(this->user.userId, activeClassId,
                                                         activeClass.enabledLevels, this->allLevels));
        levelSettingsDialog->show();
    });

    connect(ui->deleteClassButton, &QPushButton::clicked,
            this, [this] {
        deleteClassConfirmation.reset(new DeleteClassConfirmation);
        deleteClassConfirmation->show();
        connect(deleteClassConfirmation.get(), &DeleteClassConfirmation::accepted,
                this, &AdminPane::refreshClassTabs);
    });

    // Populate list of all levels. This is a one-time operation.
    LoqueClient client;
    auto status = client.getAllLevels(this->allLevels);
    if (status != Status::OK) {
        std::cerr << "ERROR: Unable to populate list of all levels. " <<
            "Client returned status " << status << std::endl;

        // TODO: Should we really disable level settings in this case?
        ui->levelSettingsButton->setEnabled(false);
    }
    refreshClassTabs();

}

AdminPane::~AdminPane() {
    delete ui;
}

void AdminPane::refreshClassTabs() {
    std::unique_ptr<std::vector<ClassStats>> newClassStats(new std::vector<ClassStats>);
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
    // TODO: CLEAR THE TABLES HERE
    if (allClassStats->size() == 0) {
        activeClassIdx = -1;
        ui->stackedWidget->setCurrentWidget(ui->noClassesPage);
        return;
    }
    for (auto& classStats : *allClassStats) {
        ui->classList->addItem(QString::fromStdString(classStats.className));
    }
    setSummaryBox();
    setUserTable();
    setMapTable();
    ui->classList->item(0)->setSelected(true);
    classClicked(0);
}

void AdminPane::classClicked(int row) {
     if (row == -1) return;
     activeClassIdx = row;
     const ClassStats& currStats = allClassStats->at(activeClassIdx);
     ui->currClassLabel->setText(QString::fromStdString(currStats.className));
     setSummaryBox();
     setUserTable();
     setMapTable();
}

void AdminPane::showCreateClassDialog() {
    createClassDialog.reset(new CreateClassDialog(user.userId));
    createClassDialog->show();
    connect(createClassDialog.get(), &CreateClassDialog::classCreated,
            this, &AdminPane::refreshClassTabs); 
}

void AdminPane::showHtmlReportDialog() {
    if (activeClassIdx < 0) return;
    try {
        QString filter = "HTML File (*.html)";
        QString filename = QFileDialog::getSaveFileName(this, tr("Create HTML Report"),
                                                        QString(), filter, &filter);
        std::ofstream out(filename.toStdString());
        writeHtmlReport(out, allClassStats->at(activeClassIdx));
        out.close();
    } catch (std::exception& e) {
        std::cerr << "ERROR: Unable to save html report. " << e.what() << std::endl;
    }
}

void AdminPane::setSummaryBox(){
    int totalPoints = 0;
    int totalTime = 0;
    const ClassStats &cstats = allClassStats->at(activeClassIdx);
    for(auto& user : cstats.studentStats){
        totalPoints += user.totalScore;
        totalTime += user.totalSecPlayed;
    }
    QLabel *classIdLabel = new QLabel("Class ID: " + QString::number(cstats.classId));
    QLabel *classNameLabel = new QLabel("Class Name: " + QString::fromStdString(cstats.className));
    QLabel *classPointsLabel = new QLabel("Total Points: " + QString::number(totalPoints));
    QLabel *classTimeLabel = new QLabel("Total Time Played: " + getFormattedTime(totalTime));

    QGroupBox *summaryBox = new QGroupBox;
    QVBoxLayout *summaryLayout = new QVBoxLayout;

    summaryLayout->addWidget(classIdLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classNameLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classPointsLabel, 0, Qt::AlignCenter);
    summaryLayout->addWidget(classTimeLabel, 0, Qt::AlignCenter);
    summaryBox->setLayout(summaryLayout);
    summaryBox->setFixedSize(200, 100);
}

QString AdminPane::getFormattedTime(int seconds){
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

void AdminPane::setUserTable(){
    ui->userTable->clear();
    const ClassStats &cstats = allClassStats->at(activeClassIdx);
    QStringList headers;
    headers.append("Student");
    headers.append("Total Score");
    headers.append("Time Played");

    int numlvls = allLevels.size();

    for(int i = 0; i < numlvls; i++){
        headers.append("Level " + QString::number(i+1) + " Completed");
    }

    ui->userTable->setRowCount(cstats.studentStats.size());
    ui->userTable->setColumnCount(3 + numlvls);
    ui->userTable->setHorizontalHeaderLabels(headers);
    ui->userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    int col = 3;
    for(auto& user : cstats.studentStats){
        QTableWidgetItem *nameCell = new QTableWidgetItem(QString::fromStdString(user.username));
        QTableWidgetItem *scoreCell = new QTableWidgetItem(QString::number(user.totalScore));
        QTableWidgetItem *timeCell = new QTableWidgetItem(QString::number(user.totalSecPlayed));
        nameCell->setTextAlignment(Qt::AlignCenter);
        scoreCell->setTextAlignment(Qt::AlignCenter);
        timeCell->setTextAlignment(Qt::AlignCenter);

        ui->userTable->setItem(row, 0, nameCell);
        ui->userTable->setItem(row, 1, scoreCell);
        ui->userTable->setItem(row, 2, timeCell);

        QTableWidgetItem *lvlCell = new QTableWidgetItem;
        for(int i = 0; i < numlvls; i++){
            lvlCell = new QTableWidgetItem;
            lvlCell->setBackgroundColor(getLevelColor(user, i));
            lvlCell->setTextAlignment(Qt::AlignCenter);
            ui->userTable->setItem(row, col++, lvlCell);
        }
        row++;
        col = 3;
    }
    ui->userTable->sortByColumn(0, Qt::AscendingOrder);
    ui->userTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->userTable->verticalHeader()->setVisible(false);
}

QColor AdminPane::getLevelColor(const UserStats& user, int levelID){
    QColor color;
    if(user.highScores.find(levelID) == user.highScores.end() ||user.highScores.at(levelID) == 0){
        color.setRgb(235, 16, 16);
    } else {
        color.setRgb(0, 186, 6);
    }
    return color;
}

void AdminPane::setMapTable(){
    ui->mapTable->clear();
    const ClassStats &cstats = allClassStats->at(activeClassIdx);
    QStringList headers;
    headers.append("Student");
    headers.append("Level ID");
    headers.append("Level Name");
    headers.append("Level Score");
    headers.append("Completion Time");
    ui->mapTable->setColumnCount(5);
    ui->mapTable->setHorizontalHeaderLabels(headers);
    ui->mapTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = ui->mapTable->rowCount();
    std::map<int, std::string> levelMap;

    for(LevelInfo info : allLevels){
        levelMap[info.id] = info.name;
    }

    for(size_t i = 0; i < cstats.studentStats.size(); i++){
        const UserStats& user = cstats.studentStats.at(i);
        for(auto& kv : user.highScores){
            ui->mapTable->insertRow(row);
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
            ui->mapTable->setItem(row, 0, studentNameCell);
            ui->mapTable->setItem(row, 1, levIDCell);
            ui->mapTable->setItem(row, 2, levelNameCell);
            ui->mapTable->setItem(row, 3, scoreCell);
            ui->mapTable->setItem(row++, 4, timeCell);
        }
    }
    ui->mapTable->sortByColumn(0, Qt::AscendingOrder);
    ui->mapTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->mapTable->verticalHeader()->setVisible(false);
}
