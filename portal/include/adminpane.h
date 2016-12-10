#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <memory>
#include "addclassdialog.h"
#include "../shared/loqueclient.h"
#include "ui_adminpane.h"

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(UserInfo user, QWidget *parent = 0);
    ~AdminPane();

signals:
    void classCreated(int classID);
private:
    UserInfo user;
    std::unique_ptr<std::vector<ClassStats>> allClassStats;
    int activeClassIdx;
    Ui::AdminPane *ui;

    void refreshClassTabs();
    void classClicked(int);
    void showCreateClassDialog();
};

#endif // ADMINPANE_H
