#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <memory>
#include "addclassdialog.h"
#include "../shared/loqueclient.h"

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(UserInfo user, QWidget *parent = 0);
    ~AdminPane();

private:
    UserInfo user; 

    QIcon addClassIcon = QIcon(QPixmap(":/assets/assets/add"));
    QHBoxLayout *mainLayout;
    QTabWidget *tabs;
    QPushButton *addClassButton;
    std::unique_ptr<AddClassDialog> currClassDialog;

    void createClassroom(QString name);
};

#endif // ADMINPANE_H
