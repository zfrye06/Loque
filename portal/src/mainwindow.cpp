#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    paneContainer(new QStackedWidget),
    loginPane(new LoginPane),
    registerPane(new RegisterPane),
    studentPlayPane(new StudentPlayPane),
    adminPane(new AdminPane(19)),
    adminPlayPane(new AdminPlayPane)
{
    ui->setupUi(this);
    paneContainer->addWidget(loginPane);
    paneContainer->addWidget(registerPane);
    paneContainer->addWidget(studentPlayPane);
    paneContainer->addWidget(adminPane);
    paneContainer->addWidget(adminPlayPane);
    setCentralWidget(paneContainer);

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
        adminPane->setUser(user);
        adminPane->updateClassStats(); 
        paneContainer->setCurrentWidget(adminPane); 
    } else {
        studentPlayPane->setUser(user);
        studentPlayPane->updateLevelInfo(); 
        paneContainer->setCurrentWidget(studentPlayPane); 
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
