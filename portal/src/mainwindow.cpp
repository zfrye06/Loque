#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QPushButton>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    paneContainer(new QStackedWidget),
    loginPane(new LoginPane),
    registerPane(new RegisterPane),
    studentPlayPane(nullptr),
    adminPane(nullptr)
{
    ui->setupUi(this);

    paneContainer->addWidget(loginPane);
    paneContainer->addWidget(registerPane);
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

    ui->menuFile->setDisabled(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleLogin(UserInfo user) {
    if (user.type == UserType::ADMIN) {
        adminPane = new AdminPane(user);
        paneContainer->addWidget(adminPane);
        paneContainer->setCurrentWidget(adminPane);
    } else {
        studentPlayPane = new StudentPlayPane(user);
        paneContainer->addWidget(studentPlayPane);
        paneContainer->setCurrentWidget(studentPlayPane);

        addClassAction = new QAction(tr("Add Class"), 0);
        connect(addClassAction, &QAction::triggered,
                studentPlayPane, &StudentPlayPane::showAddClassDialog);
        ui->menuFile->addAction(addClassAction);
        refreshAction = new QAction(tr("Refresh"), 0);
        ui->menuFile->addAction(refreshAction);
        connect(refreshAction, &QAction::triggered,
                studentPlayPane, &StudentPlayPane::updateInfo);
    }
    logoutAction = new QAction(tr("Log Out"), 0);
    connect(logoutAction, &QAction::triggered,
            this, &MainWindow::logout); 
    ui->menuFile->addAction(logoutAction);
    ui->menuFile->setEnabled(true);
}

void MainWindow::logout() {
    paneContainer->setCurrentWidget(loginPane);
    if (adminPane != nullptr) {
        paneContainer->removeWidget(adminPane);
        delete adminPane;
        adminPane = nullptr; 
    } else {
        ui->menuFile->removeAction(addClassAction);
        ui->menuFile->removeAction(refreshAction);
        delete refreshAction;
        delete addClassAction;
        refreshAction = nullptr;
        addClassAction = nullptr; 
        paneContainer->removeWidget(studentPlayPane);
        delete studentPlayPane;
        studentPlayPane = nullptr;
    }
    ui->menuFile->removeAction(logoutAction);
    delete logoutAction;
    logoutAction = nullptr;
    ui->menuFile->setDisabled(true);
}
