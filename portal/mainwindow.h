#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../shared/loqueclient.h"
#include "SFML/Network.hpp"
#include <QDebug>
#include <QMainWindow>

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
    void handleLogin();
    void displayLoginMessage(const char *loginMessage, const char *color);
    void resetLoginMessage();
    void handleRegistration();
    void displayRegistrationErrorMessage(const char *errorMessage);
    void resetRegistrationErrorMessage();
};

#endif // MAINWINDOW_H
