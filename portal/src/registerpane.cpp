#include "registerpane.h"
#include "ui_registerpane.h"

RegisterPane::RegisterPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPane)
{
    ui->setupUi(this);

    connect(ui->registerButton, &QPushButton::clicked,
            this, &RegisterPane::attemptRegistration);
}

void RegisterPane::attemptRegistration() {
    ui->errorLabel->clear();

    std::string username = ui->usernameField->text().toStdString();
    std::string password = ui->passwordField->text().toStdString();
    UserType userType = ui->adminCheck->isChecked() ? UserType::ADMIN : UserType::STUDENT;
    if (username.length() == 0 || password.length() == 0) {
        ui->errorLabel->setText("Oops! Be sure to enter a username and password."); 
        return;
    }
    LoqueClient client;
    LoginResult login;
    auto status = client.createAccount(username, password, userType, login);
    if (status != Status::OK || login.userType == UserType::DNE) {
        if (status == Status::NETWORK_ERR) {
            ui->errorLabel->setText("Oops! Looks like we can't connect to the network.");
        } else {
            ui->errorLabel->setText("Oops! Looks like that username has been used before.");
        }
        return;
    }
    emit onRegistration(UserInfo(username, login.userId, login.userType));
}

RegisterPane::~RegisterPane()
{
    delete ui;
}
