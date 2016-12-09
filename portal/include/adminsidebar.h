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

public slots:

private:
    UserInfo user;
    QListWidget *sidebar;
    QVBoxLayout *vertLayout;
    void initWidgets();
    void refreshSidebar();
};

#endif // ADMINSIDEBAR_H
