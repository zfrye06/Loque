#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->createAccountButton, &QPushButton::pressed, this, [this] { resetRegistrationErrorMessage(); ui->worldTreeWidget->setCurrentIndex(1); });
    connect(ui->haveAnAccountButton, &QPushButton::pressed, this, [this] { resetLoginMessage(); ui->worldTreeWidget->setCurrentIndex(0); });
    connect(ui->loginButton, &QPushButton::pressed, this, &MainWindow::handleLogin);
    connect(ui->registerButton, &QPushButton::pressed, this, &MainWindow::handleRegistration);
}

void MainWindow::handleLogin()
{
    std::string username = ui->usernameTextbox->text().toStdString();
    std::string password = ui->passwordTextbox->text().toStdString();
    std::string address = "127.0.0.1";
    int port = 5002;
    LoginResult result;
    LoqueClient client(address, port);
    Status returnStatus = client.attemptLogin(username, password, result);
    if (returnStatus != sf::Socket::Done)
    {
        qDebug() << "Login has network problems.";
        //network problems
    }
    else
    {
        qDebug() << "Login sends and receives fine.";
        //launch mainWindow
    }
}

void MainWindow::displayLoginMessage(const char *loginMessage, const char *color)
{
    ui->registrationMessage->setText(QString(loginMessage));
    ui->registrationMessage->setStyleSheet("QLabel { color : " + color + " }");
    ui->registrationMessage->setMaximumSize(10000, 10000);
}

void MainWindow::resetLoginMessage()
{
    ui->loginMessage->setMaximumSize(0, 0);
}

void MainWindow::handleRegistration()
{
    if (ui->usernameField->text().size() == 0) {
        displayRegistrationErrorMessage("Username cannot be blank.");
    } else if (ui->usernameField->text().size() < 6 || ui->usernameField->text().size() > 20) {
        displayRegistrationErrorMessage("Username must be between six and twenty characters long.");
    } else if (ui->passwordField->text().size() == 0) {
        displayRegistrationErrorMessage("Password cannot be blank.");
    } else if (ui->passwordField->text().size() < 6 || ui->usernameField->text().size() > 20) {
        displayRegistrationErrorMessage("Password must be between six and twenty characters long.");
    } else if (ui->passwordField->text().toStdString() != ui->confirmPasswordField->text().toStdString()) {
        displayRegistrationErrorMessage("Passwords do not match.");
    } else {
        std::string username = ui->usernameField->text().toStdString();
        std::string password = ui->passwordField->text().toStdString();
        std::string address = "127.0.0.1";
        int port = 5002;
        LoginResult result;
        LoqueClient client(address, port);
        UserType type = (ui->accountType->currentText().toStdString() == "Student") ? UserType::STUDENT :
                        (ui->accountType->currentText().toStdString() == "Admin") ? UserType::ADMIN : UserType::DNE;
        Status returnStatus = client.createAccount(username, password, type, result);
        if(returnStatus == Status::NETWORK_ERR)
        {
            qDebug() << "Create account has network problems.";
            displayRegistrationErrorMessage("Failed to create account due to network problems.");
        }
        else if (returnStatus == Status::DB_ERR)
        {
            qDebug() << "Create account has database problems.";
            displayRegistrationErrorMessage("Failed to create account due to database problems.");
        }
        else
        {
            ui->worldTreeWidget->setCurrentIndex(0);
            displayLoginMessage("Account was created successfully.");
        }
    }
}

void MainWindow::displayRegistrationErrorMessage(const char *errorMessage)
{
    ui->registrationMessage->setText(QString(errorMessage));
    ui->registrationMessage->setStyleSheet("QLabel { color : red }");
    ui->registrationMessage->setMaximumSize(10000, 10000);
}

void MainWindow::resetRegistrationErrorMessage()
{
    ui->registrationMessage->setMaximumSize(0, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


