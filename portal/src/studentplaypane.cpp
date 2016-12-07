#include <QProcess>
#include <iostream>
#include "studentplaypane.h"

QListWidget *classRowList();

StudentPlayPane::StudentPlayPane(QWidget *parent) :
    QWidget(parent),
    levelInfo(nullptr),
    activeLevelRecord(nullptr), 
    splitter(new QSplitter(this)),
    layout(new QVBoxLayout()),
    descriptionAreaLayout(new QHBoxLayout()),
    levelInfoLayout(new QVBoxLayout()),
    vertList(new QListWidget()),
    descriptionAreaWidget(new QWidget()),
    levelInfoWidget(new QWidget()),
    activeLevelThumbnail(new QLabel()),
    levelNameLabel(new QLabel()),
    levelDescLabel(new QLabel()),
    highScoreLabel(new QLabel()),
    playButton(new QPushButton())
{
    splitter->setHandleWidth(0);
    splitter->setOrientation(Qt::Vertical);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);
    splitter->addWidget(vertList);
    layout->addWidget(splitter);

    QPalette palette;
    palette.setColor(QPalette::Highlight, Qt::transparent);
    palette.setColor(QPalette::HighlightedText, vertList->palette().color(QPalette::Text));
    vertList->setPalette(palette);
    vertList->setStyleSheet("background-color: transparent;");
    vertList->setSelectionMode(QAbstractItemView::SingleSelection);
    vertList->setAutoFillBackground(false);
    vertList->setFlow(QListView::TopToBottom);
    vertList->setFocusPolicy(Qt::NoFocus);
    vertList->setFrameShape(QFrame::NoFrame);

    descriptionAreaLayout->addWidget(activeLevelThumbnail);
    descriptionAreaLayout->addWidget(levelInfoWidget);
    descriptionAreaWidget->setLayout(descriptionAreaLayout);
    splitter->addWidget(descriptionAreaWidget);

    levelInfoLayout->addWidget(levelNameLabel);
    levelInfoLayout->addWidget(levelDescLabel);
    levelInfoLayout->addWidget(highScoreLabel);
    levelInfoLayout->addWidget(playButton);
    levelInfoWidget->setLayout(levelInfoLayout);

    playButton->setText("Play");
    connect(playButton, &QPushButton::clicked,
            this, [this] {
        if (activeLevelRecord != nullptr) {
            QStringList args;
            args << QString::number(activeLevelRecord->level.id) << QString::number(user.userId);
            QString loqueExec("/Users/asteele/Sandbox/edu-app-unescaped-characters/game-client/bin/loque");
            QString loqueWorkingDir("/Users/asteele/Sandbox/edu-app-unescaped-characters/game-client");
            QProcess::startDetached(loqueExec, args, loqueWorkingDir);
        }
    });

    this->setLayout(layout);

    // TODO: Sample levelInfo. Remove.
    user.userId = 1;
    levelInfo.reset(new std::vector<ClassLevelInfo>);
    ClassLevelInfo info1;
    info1.classId = 1;
    info1.className = "Mr. Johnson's";
    LevelRecord lr1;
    lr1.highScore = 48;
    lr1.bestCompletionTimeSecs = 3;
    lr1.level.id = 0;
    lr1.level.name = "LEVEL 0";
    lr1.level.description = "FUN FUN FUN";
    info1.levelRecords.push_back(lr1);
    levelInfo->push_back(info1);
    updateDisplay();
}

StudentPlayPane::~StudentPlayPane() {
    delete splitter;
}

void StudentPlayPane::thumbnailClicked(int row, int col) {
    activeLevelRecord = &levelInfo->at(row).levelRecords.at(col);
    std::string thumbnailPath = Maps[activeLevelRecord->level.id].qtThumbnailPath();
    QPixmap thumbnail(QString::fromStdString(thumbnailPath));
    int width = activeLevelThumbnail->width();
    int height = activeLevelThumbnail->height(); 
    activeLevelThumbnail->setPixmap(thumbnail.scaled(width, height, Qt::KeepAspectRatio));
    levelNameLabel->setText(QString::fromStdString(activeLevelRecord->level.name));
    levelDescLabel->setText(QString::fromStdString(activeLevelRecord->level.description));
    highScoreLabel->setText(QString::number(activeLevelRecord->highScore));
}

void StudentPlayPane::addClassRow(int row, const ClassLevelInfo& classInfo) {
    vertList->addItem(QString::fromStdString(classInfo.className));
    QListWidgetItem* item = new QListWidgetItem();
    QListWidget *horizList = classRowList();
    item->setSizeHint(QSize(500, 100));
    vertList->addItem(item);
    vertList->setItemWidget(item, horizList);
    for (auto& record : classInfo.levelRecords) {
        std::string path = Maps[record.level.id].qtThumbnailPath();
        QIcon thumbnail(QString::fromStdString(path));
        horizList->addItem(new QListWidgetItem(thumbnail, NULL));
    }
    connect(horizList, &QListWidget::currentRowChanged,
            this, [this, row] (int col) { this->thumbnailClicked(row, col); });
}

void StudentPlayPane::updateDisplay() {
    if (levelInfo == nullptr) return;
    vertList->clear();
    activeLevelThumbnail->clear();
    levelNameLabel->clear();
    levelDescLabel->clear();
    highScoreLabel->clear();
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

void StudentPlayPane::setUser(UserInfo user) {
   this->user = user;
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
    list->setIconSize(QSize(150, 150));
    list->setFrameShape(QFrame::NoFrame);
    return list;
}
