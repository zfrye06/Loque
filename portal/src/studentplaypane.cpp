#include "studentplaypane.h"
#include "ui_studentplaypane.h"
#include <QAbstractItemView>

#include <iostream>
StudentPlayPane::StudentPlayPane(QWidget *parent) :
    QWidget(parent)
{
    splitter = new QSplitter(this);
    layout = new QVBoxLayout();
    vertList = new QListWidget();

    layout->addWidget(splitter);
    splitter->addWidget(vertList);
    vertList->addItem("asdf");
    vertList->setFlow(QListView::LeftToRight);
    vertList->setSelectionMode(QAbstractItemView::SingleSelection);
    vertList->setFlow(QListView::TopToBottom);

    //example of adding horizontal list
    QListWidgetItem* item = new QListWidgetItem();
    QListWidget* horiz1 = new QListWidget();
    horiz1->setFlow(QListView::LeftToRight);
    horiz1->setSelectionMode(QAbstractItemView::SingleSelection);
    item->setSizeHint(QSize(500, 100));
    vertList->addItem(item);
    vertList->setItemWidget(item, horiz1);
    horiz1->addItem("horiz1");
    horiz1->addItem("item2");

    this->setLayout(layout);
}

StudentPlayPane::~StudentPlayPane() {
}

void StudentPlayPane::setUser(UserInfo user) {
    

}

void StudentPlayPane::updateLevelInfo() {

}
