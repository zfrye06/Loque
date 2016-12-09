#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QPushButton>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    paneContainer(new QStackedWidget),
    loginPane(new LoginPane),
    registerPane(new RegisterPane),
    studentPlayPane(nullptr),
    adminPane(nullptr),
    adminPlayPane(nullptr)
{
    ui->setupUi(this);

    //For testing purposes, we want to look at adminPane
//    adminPane = new AdminPane(UserInfo());
//    paneContainer->addWidget(adminPane);

    paneContainer->addWidget(loginPane);
    paneContainer->addWidget(registerPane);
    setCentralWidget(paneContainer);
    menuBar()->setHidden(true);
    connect(loginPane, &LoginPane::onLogin,
            this, &MainWindow::handleLogin);


    connect(loginPane, &LoginPane::onSignupRequested,
            this, [this] {
        paneContainer->setCurrentWidget(registerPane);
    });

    connect(registerPane, &RegisterPane::onRegistration,
            this, &MainWindow::handleLogin);

    connect(registerPane, &RegisterPane::onLoginRequested,
            this, [this] {
        paneContainer->setCurrentWidget(loginPane);
    });
}

void MainWindow::handleLogin(UserInfo user) {
    if (user.type == UserType::ADMIN) {
        // TODO: ADD adminPlayPane. 
        adminPane = new AdminPane(user);
        paneContainer->addWidget(adminPane);
        paneContainer->setCurrentWidget(adminPane); 
    } else {
        menuBar()->setHidden(false);
        QList<QMenu*> menus = menuBar()->findChildren<QMenu*>();
        for(int i = 0; i < menus.size(); i++){
            if(menus.at(i)->title() == "File"){
                menus.at(i)->addAction("Add Class");
                break;
            }
        }
        studentPlayPane = new StudentPlayPane(user);
        paneContainer->addWidget(studentPlayPane);
        paneContainer->setCurrentWidget(studentPlayPane);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
