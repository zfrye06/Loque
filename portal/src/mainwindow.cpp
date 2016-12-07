#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    paneContainer(new QStackedWidget),
    loginPane(new LoginPane),
    registerPane(new RegisterPane),
    studentPlayPane(new StudentPlayPane),
    adminPane(nullptr),
    adminPlayPane(nullptr)
{
    ui->setupUi(this);
    paneContainer->addWidget(loginPane);
    paneContainer->addWidget(registerPane);
    paneContainer->addWidget(studentPlayPane);
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

    //paneContainer->setCurrentWidget(studentPlayPane);
}

void MainWindow::handleLogin(UserInfo user) {
    if (user.type == UserType::ADMIN) {
        // TODO: ADD adminPlayPane. 
        adminPane = new AdminPane(user);
        paneContainer->addWidget(adminPane);
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
