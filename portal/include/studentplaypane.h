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

    QWidget *window;
    QSplitter *splitter;
    QVBoxLayout *layout;
    QHBoxLayout *descriptionAreaLayout;
    QVBoxLayout *levelInfoLayout;
    QListWidget *vertList;
    QWidget *descriptionAreaWidget;
    QWidget *levelInfoWidget;
    QLabel *activeLevelThumbnail;
    QLabel *levelNameLabel;
    QLabel *levelDescLabel;
    QLabel *highScoreLabel;
    QPushButton *playButton;

    void addClassRow(int, const ClassLevelInfo&);
    void updateLevelInfo();
    void updateDisplay();
    void thumbnailClicked(int, int); 
};

#endif // STUDENTPLAYPANE_H
