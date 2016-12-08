#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "../shared/loqueclient.h"

namespace Ui {
class AdminPane;
}

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(int teacherID, QWidget *parent = 0);
    ~AdminPane();

    void updateClassStats();
    void setUser(UserInfo user);

signals:
    void classCreated(int classID);

private:
    int teacherID;

    QHBoxLayout *mainLayout;
    QTabWidget *tabs;
    QPushButton *addClassButton;
    QIcon addClassIcon = QIcon(QPixmap(":/assets/assets/add"));

    void initWidgets(const std::vector<ClassStats>& classStats);
    void createClassroom(QString name);

};

#endif // ADMINPANE_H
