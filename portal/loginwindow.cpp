#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "../shared/loqueclient.h"
#include "SFML/Network.hpp"
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->submit, SIGNAL(clicked()), this, SLOT(sendUserInfo()));
    connect(ui->signup, SIGNAL(clicked()), this, SLOT(createAccount()));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
void LoginWindow::sendUserInfo()
{
    std::string username = ui->usernameField->text().toStdString();
    std::string password = ui->passwordField->text().toStdString();
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
void LoginWindow::createAccount()
{
    std::string username = ui->usernameField->text().toStdString();
    std::string password = ui->passwordField->text().toStdString();
    std::string address = "127.0.0.1";
    int port = 5002;
    LoginResult result;
    LoqueClient client(address, port);
    Status returnStatus = client.createAccount(username, password, Student, result);
    if(returnStatus != sf::Socket::Done)
    {
        qDebug() << "Create account has network problems.";
        //we messed up again.
    }
    else
    {
        qDebug() << "Create account works fine.";
        //launch mainWindow
    }
}
