#include "studentplaypane.h"

#include <iostream>
StudentPlayPane::StudentPlayPane(QWidget *parent) :
    QWidget(parent),
    levelInfo(nullptr),
    splitter(new QSplitter(this)),
    layout(new QVBoxLayout()),
    vertList(new QListWidget()),
    descriptionAreaLayout(new QHBoxLayout()),
    descriptionAreaWidget(new QWidget()),
    levelInfoWidget(new QWidget()),
    levelInfoLayout(new QVBoxLayout()),
    levelThumbnail(new QLabel()),
    levelName(new QLabel()),
    levelDescription(new QLabel()),
    bestScore(new QLabel()),
    playButton(new QPushButton())
{
    splitter->setOrientation(Qt::Vertical);
    vertList->setSelectionMode(QAbstractItemView::SingleSelection);
    vertList->setFlow(QListView::TopToBottom);
    layout->addWidget(splitter);
    splitter->addWidget(vertList);

    descriptionAreaWidget->setLayout(descriptionAreaLayout);
    splitter->addWidget(descriptionAreaWidget);

    // Setting sample level thumbnail
    QPixmap p(":/assets/assets/candySky.jpg");
    levelThumbnail->setPixmap(p.scaled(levelThumbnail->width(),levelThumbnail->height(), Qt::KeepAspectRatio));

    levelInfoWidget->setLayout(levelInfoLayout);
    levelInfoLayout->addWidget(levelName);
    levelInfoLayout->addWidget(levelDescription);
    levelInfoLayout->addWidget(bestScore);
    levelInfoLayout->addWidget(playButton);

    // Setting sample text to know where everything will appear
    levelName->setText("Level Name");
    levelDescription->setText("Level Description");
    bestScore->setText("Best Score");
    playButton->setText("Play");

    descriptionAreaLayout->addWidget(levelThumbnail);
    descriptionAreaLayout->addWidget(levelInfoWidget);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    this->setLayout(layout);
}

StudentPlayPane::~StudentPlayPane() {
    delete splitter;
}

void StudentPlayPane::setUser(UserInfo user) {
   this->user = user;
}

void StudentPlayPane::updateLevelInfo() {
    LoqueClient client;
    std::unique_ptr<UserLevelInfo> newInfo(new std::vector<ClassLevelInfo>());
    auto status = client.getUserLevelInfo(user.userId, *newInfo);
    if (status != Status::OK) {
        // TODO: show error
        std::cerr << "ERROR: Unable to update level info. Status returned was " << status << std::endl;
        return;
    }
    levelInfo.reset(newInfo.release());
}

void StudentPlayPane::addClassRow(const ClassLevelInfo& classInfo) {
    vertList->addItem(QString::fromStdString(classInfo.className));
    QListWidgetItem* item = new QListWidgetItem();
    QListWidget* horizList = new QListWidget();
    horizList->setFlow(QListView::LeftToRight);
    horizList->setSelectionMode(QAbstractItemView::SingleSelection);
    item->setSizeHint(QSize(500, 100));
    vertList->addItem(item);
    vertList->setItemWidget(item, horizList);
    for (auto& record : classInfo.levelRecords) {
        std::string thumbnail = Maps[record.level.id].thumbnail; 
        horizList->addItem(new QListWidgetItem(QString::fromStdString(thumbnail), NULL));
    }
}

void StudentPlayPane::updateDisplay() {
    if (levelInfo == nullptr) return;
    for (auto& level : *levelInfo) {
        addClassRow(level);
    }
}
