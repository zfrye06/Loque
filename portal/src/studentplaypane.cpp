#include "studentplaypane.h"
#include "ui_studentplaypane.h"
#include <QAbstractItemView>

#include <iostream>
StudentPlayPane::StudentPlayPane(QWidget *parent) :
    QWidget(parent),
    levelInfo(nullptr),
    splitter(new QSplitter(this)),
    layout(new QVBoxLayout()),
    vertList(new QListWidget())
{
    vertList->setSelectionMode(QAbstractItemView::SingleSelection);
    vertList->setFlow(QListView::TopToBottom);
    layout->addWidget(splitter);
    splitter->addWidget(vertList);
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
