#ifndef STUDENTPLAYPANE_H
#define STUDENTPLAYPANE_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QAbstractItemView>
#include "ui_studentplaypane.h"
#include "../shared/loqueclient.h"

namespace Ui {
class StudentPlayPane;
}

class StudentPlayPane : public QWidget
{
    Q_OBJECT

public:
    explicit StudentPlayPane(UserInfo, QWidget *parent = 0);
    ~StudentPlayPane();

private:
    UserInfo user;
    std::unique_ptr<UserLevelInfo> levelInfo;
    LevelRecord *activeLevelRecord;

    Ui::StudentPlayPane *ui;

    void addClassRow(int, const ClassLevelInfo&);
    void updateLevelInfo();
    void updateDisplay();
    void thumbnailClicked(int, int); 
};

#endif // STUDENTPLAYPANE_H
