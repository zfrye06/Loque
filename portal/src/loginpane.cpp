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
        emit onSignupRequested();
    });
}

void LoginPane::attemptLogin() {
    ui->errorLabel->clear();
    
    std::string username = ui->usernameField->text().toStdString();
    std::string password = ui->passwordField->text().toStdString();
    if (username.length() == 0 || password.length() == 0) {
        ui->errorLabel->setText("Oops! Be sure to enter a username and password."); 
        return;
    }
    LoqueClient client;
    LoginResult login;
    auto status = client.attemptLogin(username, password, login);
    if (status != Status::OK || login.userType == UserType::DNE) {
        if (status == Status::NETWORK_ERR) {
            ui->errorLabel->setText("Oops! Looks like we can't connect to the network.");
        } else {
            ui->errorLabel->setText("Oops! Looks like that username has been used before.");
        }
        return;
    }
    emit onLogin(UserInfo(username, login.userId, login.userType));
}

LoginPane::~LoginPane() {
    delete ui;
}
