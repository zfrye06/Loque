#include <iostream>
#include "adminsidebar.h"

AdminSidebar::AdminSidebar(UserInfo user, QWidget *parent) : QWidget(parent), user(user), sidebar(new QListWidget), vertLayout(new QVBoxLayout)
{ 
    initWidgets();
}

void AdminSidebar::initWidgets()
{
    sidebar->setIconSize(QSize(54, 54));
    setMaximumSize(100, 100000);
    setMinimumSize(100, 0);
    sidebar->setViewMode(QListWidget::ViewMode::IconMode);
    sidebar->setFlow(QListWidget::Flow::TopToBottom);
    sidebar->setStyleSheet(QString("QListWidget { background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(100, 100, 100, 255), stop:1 rgba(150, 150, 150, 255));      border-right: 2px groove gray;     }        QListWidget::item:hover     {     background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(200, 200, 200, 255));     }        QListWidget::item:selected     {     background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(200, 200, 200, 200), stop:1 rgba(220, 220, 220, 255));     }        QListWidget::item     {     border-bottom: 2px groove gray;     padding: 10px; color: white;     }"));
    sidebar->setFocusPolicy(Qt::NoFocus);
    refreshSidebar();
    QListWidgetItem *addClassIcon = new QListWidgetItem(QIcon(":/assets/plus.png"), "");
    QListWidgetItem *levelViewIcon = new QListWidgetItem(QIcon(":/assets/levelView.png"), "");
    sidebar->addItem(addClassIcon);
    sidebar->addItem(levelViewIcon);
    vertLayout->addWidget(sidebar);
    setLayout(vertLayout);
}

void AdminSidebar::refreshSidebar()
{
    std::vector<ClassStats> classStats;
    LoqueClient client;
    auto status = client.getAllClassStats(user.userId, classStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't get the info.
        std::cerr << "ERROR: Unable to download class stats. Client returned status" << status << std::endl;
        return;
    }

    for(ClassStats& cstats : classStats) {
        QListWidgetItem *item = new QListWidgetItem(cstats.className.c_str());
        item->setSizeHint(QSize(79, 79));
        sidebar->addItem(item);
    }
}
