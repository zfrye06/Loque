#ifndef ADMINSIDEBAR_H
#define ADMINSIDEBAR_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QSizePolicy>
#include "../shared/loqueclient.h"

class AdminSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit AdminSidebar(UserInfo user, QWidget *parent = 0);

signals:
    void classCreated(ClassStats cstats);
    void classTabChanged(int index);
public slots:

private:
    UserInfo user;
    QListWidget *sidebar;
    QVBoxLayout *vertLayout;
    QListWidgetItem *addClassIcon;
    QListWidgetItem *levelViewIcon;
    void initWidgets();
    void refreshSidebar();
    void handleSidebarClick(QListWidgetItem *item);
    void createClass(ClassStats cstats);
};

#endif // ADMINSIDEBAR_H
