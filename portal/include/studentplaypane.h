#ifndef STUDENTPLAYPANE_H
#define STUDENTPLAYPANE_H

#include <QWidget>
#include "../shared/loqueclient.h"

namespace Ui {
class StudentPlayPane;
}

class StudentPlayPane : public QWidget
{
    Q_OBJECT

public:
    explicit StudentPlayPane(QWidget *parent = 0);
    ~StudentPlayPane();

    void setUser(UserInfo);
    void updateLevelInfo();

private:
    Ui::StudentPlayPane *ui;
};

#endif // STUDENTPLAYPANE_H
