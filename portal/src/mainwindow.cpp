#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QSizePolicy>
#include "adminpane.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    paneContainer(new QStackedWidget()),
    loginPane(new LoginPane),
    registerPane(new RegisterPane),
    studentPlayPane(new StudentPlayPane),
    adminPane(new AdminPane),
    adminPlayPane(new AdminPlayPane)
{
    ui->setupUi(this);
    paneContainer->addWidget(loginPane);
    paneContainer->addWidget(registerPane);
    paneContainer->addWidget(studentPlayPane);
    paneContainer->addWidget(adminPane);
    paneContainer->addWidget(adminPlayPane);
    setCentralWidget(paneContainer);


//     ui->worldTreeWidget->setCurrentIndex(0);
//     connect(ui->createAccountButton, &QPushButton::pressed, this, [this] { resetRegistration(); ui->worldTreeWidget->setCurrentIndex(1); });
//     connect(ui->haveAnAccountButton, &QPushButton::pressed, this, [this] { resetLogin(); ui->worldTreeWidget->setCurrentIndex(0); });
// //    connect(ui->loginButton, &QPushButton::pressed, this, &MainWindow::handleLogin);
//     connect(ui->registerButton, &QPushButton::pressed, this, &MainWindow::handleRegistration);
//     d = new MyDialog(this);
//     connect(ui->loginButton, &QPushButton::pressed, this, [this] {
//         //ui->worldTreeWidget->setCurrentIndex(2);
//         setCentralWidget(d);

//     });
//    llayout = new QHBoxLayout;
//    icon = new QLabel;
//    widget = new QWidget;
//    icon->setPixmap(QPixmap("/home/liam/Downloads/thumbnail.png"));
//    label->setMaximumSize(128, 128);
//    label->setMinimumSize(128, 128);
//    icon->setMaximumSize(128, 128);
//    icon->setMinimumSize(128, 128);
//    llayout->addWidget(icon);
//    llayout->addWidget(label);
//    widget->setLayout(llayout);
//    widget->show();

//    llayout2 = new QVBoxLayout;
//    llayout2->addWidget(widget);
//    ui->statsWidget->setLayout(llayout2);
//    ui->statsWidget->show();
}

// void MainWindow::handleLogin()
// {
//     std::string username = ui->usernameTextbox->text().toStdString();
//     std::string password = ui->passwordTextbox->text().toStdString();

//     if (username.length() == 0)
//     {
//         displayLoginMessage("Username cannot be blank.", "red");
//         return;
//     }
//     if (password.length() == 0)
//     {
//         displayLoginMessage("Password cannot be blank.", "red");
//         return;
//     }

//     std::string address = "127.0.0.1";
//     int port = 5002;
//     LoginResult result;
//     LoqueClient client(address, port);
//     Status returnStatus = client.attemptLogin(username, password, result);
//     if (returnStatus == Status::NETWORK_ERR)
//     {
//         qDebug() << "Login has network problems.";
//         displayLoginMessage("Failed to login due to network problems.", "red");
//     }
//     else if (returnStatus == Status::DB_ERR)
//     {
//         qDebug() << "Login has database problems.";
//         displayLoginMessage("Failed to login due to database problems.", "red");
//     }
//     else
//     {
//         qDebug() << "Login sends and receives fine.";
//         ui->worldTreeWidget->setCurrentIndex(2);
//     }
// }

// void MainWindow::displayLoginMessage(const char *loginMessage, std::string color)
// {
//     ui->loginMessage->setText(QString(loginMessage));
//     ui->loginMessage->setStyleSheet(("QLabel { color : " + color + " }").c_str());
//     ui->loginMessage->setMaximumSize(10000, 10000);
// }

// void MainWindow::resetLogin()
// {
//     ui->loginMessage->setMaximumSize(0, 0);
//     ui->usernameTextbox->setText(QString(""));
//     ui->passwordTextbox->setText(QString(""));
// }

// void MainWindow::handleRegistration()
// {
//     if (ui->usernameField->text().size() == 0) {
//         displayRegistrationErrorMessage("Username cannot be blank.");
//     } else if (ui->usernameField->text().size() < 6 || ui->usernameField->text().size() > 20) {
//         displayRegistrationErrorMessage("Username must be between six and twenty characters long.");
//     } else if (ui->passwordField->text().size() == 0) {
//         displayRegistrationErrorMessage("Password cannot be blank.");
//     } else if (ui->passwordField->text().size() < 6 || ui->usernameField->text().size() > 20) {
//         displayRegistrationErrorMessage("Password must be between six and twenty characters long.");
//     } else if (ui->passwordField->text().toStdString() != ui->confirmPasswordField->text().toStdString()) {
//         displayRegistrationErrorMessage("Passwords do not match.");
//     } else {
//         std::string username = ui->usernameField->text().toStdString();
//         std::string password = ui->passwordField->text().toStdString();
//         std::string address = "127.0.0.1";
//         int port = 5002;
//         LoginResult result;
//         LoqueClient client(address, port);
//         UserType type = (ui->accountType->currentText().toStdString() == "Student") ? UserType::STUDENT :
//                         (ui->accountType->currentText().toStdString() == "Admin") ? UserType::ADMIN : UserType::DNE;
//         Status returnStatus = client.createAccount(username, password, type, result);
//         if(returnStatus == Status::NETWORK_ERR)
//         {
//             qDebug() << "Create account has network problems.";
//             displayRegistrationErrorMessage("Failed to create account due to network problems.");
//         }
//         else if (returnStatus == Status::DB_ERR)
//         {
//             qDebug() << "Create account has database problems.";
//             displayRegistrationErrorMessage("Failed to create account due to database problems.");
//         }
//         else
//         {
//             ui->worldTreeWidget->setCurrentIndex(0);
//             resetLogin();

//             if (ui->classKeyField->text().size() == 0)
//             {
//                 displayLoginMessage("Account was created successfully.", "green");
//             }
//             else
//             {
//                 returnStatus = client.addClassroom(result.userId, std::stoi(ui->classKeyField->text().toStdString()));
//                 if (returnStatus != Status::OK)
//                 {
//                     displayLoginMessage("Account was created successfully but failed to join class.", "yellow");
//                 }
//                 else
//                 {
//                     displayLoginMessage("Account was created successfully.", "green");
//                 }
//             }
//         }
//     }
// }

// void MainWindow::displayRegistrationErrorMessage(const char *errorMessage)
// {
//     ui->registrationMessage->setText(QString(errorMessage));
//     ui->registrationMessage->setStyleSheet("QLabel { color : red }");
//     ui->registrationMessage->setMaximumSize(10000, 10000);
// }

// void MainWindow::resetRegistration()
// {
//     ui->registrationMessage->setMaximumSize(0, 0);
//     ui->usernameField->setText(QString(""));
//     ui->passwordField->setText(QString(""));
//     ui->confirmPasswordField->setText(QString(""));
//     ui->classKeyField->setText(QString(""));
//     ui->accountType->setCurrentIndex(0);
// }

MainWindow::~MainWindow()
{
    delete ui;
}
