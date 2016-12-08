#include <QProcess>
#include <QScrollBar>
#include <iostream>
#include "studentplaypane.h"
#include "ui_studentplaypane.h"

QListWidget *classRowList();

StudentPlayPane::StudentPlayPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    user(user), 
    levelInfo(nullptr),
    activeLevelRecord(nullptr),
    ui(new Ui::StudentPlayPane)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Highlight, Qt::transparent);
    palette.setColor(QPalette::HighlightedText, ui->vertList->palette().color(QPalette::Text));
    ui->vertList->setPalette(palette);
    ui->vertList->setStyleSheet("background-color: transparent;");
    ui->vertList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->vertList->setAutoFillBackground(false);
    ui->vertList->setFlow(QListView::TopToBottom);
    ui->vertList->setFocusPolicy(Qt::NoFocus);
    ui->vertList->setFrameShape(QFrame::NoFrame);
    ui->vertList->verticalScrollBar()->hide();
    ui->vertList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->vertList->horizontalScrollBar()->hide();
    ui->vertList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->playButton, &QPushButton::clicked,
            this, [this] {
        if (activeLevelRecord != nullptr) {
            QStringList args;
            args << QString::number(activeLevelRecord->level.id) << QString::number(this->user.userId);
            QString loqueExec("/Users/asteele/Sandbox/edu-app-unescaped-characters/game-client/bin/loque");
            QString loqueWorkingDir("/Users/asteele/Sandbox/edu-app-unescaped-characters/game-client");
            QProcess::startDetached(loqueExec, args, loqueWorkingDir);
        }
    });

    updateLevelInfo();
    updateDisplay();
    ui->stackedWidget->showFullScreen();

    // TODO: Sample levelInfo. Remove.
//    user.userId = 1;
//    levelInfo.reset(new std::vector<ClassLevelInfo>);
//    ClassLevelInfo info1;
//    info1.classId = 1;
//    info1.className = "Mr. Johnson's";
//    LevelRecord lr1;
//    lr1.highScore = 48;
//    lr1.bestCompletionTimeSecs = 3;
//    lr1.level.id = 0;
//    lr1.level.name = "LEVEL 0";
//    lr1.level.description = "FUN FUN FUN";
//    info1.levelRecords.push_back(lr1);
//    levelInfo->push_back(info1);
//    updateDisplay();
}

StudentPlayPane::~StudentPlayPane() {
    delete ui;
}

void StudentPlayPane::thumbnailClicked(int row, int col) {
    activeLevelRecord = &levelInfo->at(row).levelRecords.at(col);
    std::string thumbnailPath = Maps[activeLevelRecord->level.id].qtThumbnailPath();
    QPixmap thumbnail(QString::fromStdString(thumbnailPath));
    int width = ui->activeLevelThumbnail->width();
    int height = ui->activeLevelThumbnail->height();
    ui->activeLevelThumbnail->setPixmap(thumbnail.scaled(width, height, Qt::KeepAspectRatio));
    ui->levelNameLabel->setText(QString::fromStdString(activeLevelRecord->level.name));
    ui->levelDescLabel->setText(QString::fromStdString(activeLevelRecord->level.description));
    ui->highScoreLabel->setText(QString::number(activeLevelRecord->highScore));
    ui->playButton->show();
}

void StudentPlayPane::addClassRow(int row, const ClassLevelInfo& classInfo) {
    ui->vertList->addItem(QString::fromStdString(classInfo.className));
    QListWidgetItem* item = new QListWidgetItem();
    QListWidget *horizList = classRowList();
    item->setSizeHint(QSize(500, 100));
    ui->vertList->addItem(item);
    ui->vertList->setItemWidget(item, horizList);
    for (auto& record : classInfo.levelRecords) {
        std::string path = Maps[record.level.id].qtThumbnailPath();
        QIcon thumbnail(QString::fromStdString(path));
        //QString label(QString::fromStdString(record.level.name));
        horizList->addItem(new QListWidgetItem(thumbnail, NULL));
    }
    connect(horizList, &QListWidget::currentRowChanged,
            this, [this, row] (int col) { this->thumbnailClicked(row, col); });
}

void StudentPlayPane::updateDisplay() {
    if (levelInfo == nullptr) return;
    ui->vertList->clear();
    ui->activeLevelThumbnail->clear();
    ui->levelNameLabel->clear();
    ui->levelDescLabel->clear();
    ui->highScoreLabel->clear();
    ui->playButton->hide();
    for (int i = 0; i < (int) levelInfo->size(); i++) {
        addClassRow(i, levelInfo->at(i));
    }
}

void StudentPlayPane::updateLevelInfo() {
    LoqueClient client;
    std::unique_ptr<UserLevelInfo> newInfo(new std::vector<ClassLevelInfo>());
    auto status = client.getUserLevelInfo(user.userId, *newInfo);
    if (status != Status::OK) {
        // TODO: show error
        std::cerr << "ERROR: Unable to update level info. Status returned was " <<
            status << std::endl;
        return;
    }
    levelInfo.reset(newInfo.release());
}

QListWidget *classRowList() {
    QListWidget* list = new QListWidget();
    QPalette palette;
    palette.setColor(QPalette::Highlight, list->palette().color(QPalette::Base));
    palette.setColor(QPalette::HighlightedText, list->palette().color(QPalette::Text));
    list->setPalette(palette);
    list->setFlow(QListView::LeftToRight);
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setFocusPolicy(Qt::NoFocus);
    list->setIconSize(QSize(200, 200));
    list->setFrameShape(QFrame::NoFrame);
    list->verticalScrollBar()->hide();
    list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list->horizontalScrollBar()->hide();
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    return list;
}
