#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SFML/Network.hpp"
#include <QDebug>
#include <QMainWindow>
#include <QStackedWidget>
#include "adminpane.h"
#include "adminplaypane.h"
#include "loginpane.h"
#include "studentplaypane.h"
#include "registerpane.h"
#include "../shared/loqueclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QStackedWidget *paneContainer;
    AdminPane *adminPane;
    AdminPlayPane *adminPlayPane;
    LoginPane *loginPane;
    StudentPlayPane *StudentPlayPane;
    RegisterPane *registerPane;

    void handleLogin();
    void displayLoginMessage(const char *loginMessage, std::string color);
    void resetLogin();
    void handleRegistration();
    void displayRegistrationErrorMessage(const char *errorMessage);
    void resetRegistration();
};

#endif // MAINWINDOW_H
