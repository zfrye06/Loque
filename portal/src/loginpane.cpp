#include "loginpane.h"
#include "ui_loginpane.h"

LoginPane::LoginPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPane)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginPane::attemptLogin);

    connect(ui->signupButton, &QPushButton::clicked,
            this, [this] {
        ui->usernameField->clear();
        ui->passwordField->clear();
        ui->errorLabel->setMaximumSize(0, 0);
        emit onSignupRequested();
    });
}

void LoginPane::attemptLogin() {
    ui->errorLabel->clear();
    
    std::string username = ui->usernameField->text().toStdString();
    std::string password = ui->passwordField->text().toStdString();
    if (username.length() == 0 || password.length() == 0) {
        displayErrorMessage("Be sure to enter a username and password.");
        return;
    }
    LoqueClient client;
    LoginResult login;
    auto status = client.attemptLogin(username, password, login);
    if (status != Status::OK || login.userType == UserType::DNE) {
        if (status == Status::NETWORK_ERR) {
            displayErrorMessage("Looks like we can't connect to the network.");
        } else {
            displayErrorMessage("Username or password is incorrect.");
        }
        return;
    }
    emit onLogin(UserInfo(username, login.userId, login.userType));
}

void LoginPane::displayErrorMessage(const char *errorMessage)
{
    ui->errorLabel->setText(QString(errorMessage));
    ui->errorLabel->setStyleSheet("QLabel { color : red }");
    ui->errorLabel->setMaximumSize(10000, 10000);
}

LoginPane::~LoginPane() {
    delete ui;
}
