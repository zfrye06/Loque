#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QStackedWidget>
#include "adminpane.h"
#include "adminplaypane.h"
#include "loginpane.h"
#include "studentplaypane.h"
#include "registerpane.h"
#include "../shared/loqueclient.h"
#include "SFML/Network.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QStackedWidget *paneContainer;
    LoginPane *loginPane;
    RegisterPane *registerPane;
    StudentPlayPane *studentPlayPane;
    AdminPane *adminPane;
    AdminPlayPane *adminPlayPane;

    QAction *addClassAction; // Does not own.

    void handleLogin(UserInfo);
    void logout();
};

#endif // MAINWINDOW_H
