#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "../shared/loqueclient.h"
#include "SFML/Network.hpp"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
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
    status returnStatus = client.attemptLogin(username, password, result);
    if (returnStatus != sf::Socket::Done)
    {
        //guys, we messed up.
        //take action based on that.
    }
}
