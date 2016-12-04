#include "studentplaypane.h"
#include "ui_studentplaypane.h"

#include <iostream>
StudentPlayPane::StudentPlayPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentPlayPane)
{
    ui->setupUi(this);
    //ui->listWidget->setViewMode(QListWidget::IconMode);
    //ui->listWidget->addItem(new QListWidgetItem());
    ui->listWidget->addItem(QString("hey"));
    ui->listWidget->addItem(QString("there"));
    connect(ui->listWidget, &QListWidget::clicked,
            this, [this] {
        std::cout << ui->listWidget->currentIndex().row() << std::endl;
    });
}

StudentPlayPane::~StudentPlayPane() {
    delete ui;
}

void StudentPlayPane::setUser(UserInfo user) {
    
}

void StudentPlayPane::updateLevelInfo() {
    
}
