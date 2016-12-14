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

    void showAddClassDialog();
    void updateInfo();

private:
    UserInfo user;
    UserLevelInfo* levelInfo;
    ClassLevelInfo *activeClass;
    LevelRecord *activeLevelRecord;
    Ui::StudentPlayPane *ui;

    void updateLevelInfo();
    void updateDisplay();
    void refreshDisplay();
    void classClicked(int);
    void levelThumbnailClicked(int);
    void playButtonClicked();
    void clearLevelDescription();
};

#endif // STUDENTPLAYPANE_H
