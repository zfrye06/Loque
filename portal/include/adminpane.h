#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <memory>

#include "adminsidebar.h"
#include "addclassdialog.h"
#include "../shared/loqueclient.h"

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(UserInfo user, QWidget *parent = 0);
    ~AdminPane();

    void updateClassStats();
    void setUser(UserInfo user);

signals:
    void classCreated(int classID);
private:
    UserInfo user;

    QIcon addClassIcon = QIcon(QPixmap(":/assets/assets/add"));
    QHBoxLayout *mainLayout;
    QTabWidget *tabs;
    AdminSidebar *sidebar;

    void addClassTab(const ClassStats& cstats);
    void deleteClassTab();
    void refreshClassTabs();
    void changeClassTab(int index);
};

#endif // ADMINPANE_H
