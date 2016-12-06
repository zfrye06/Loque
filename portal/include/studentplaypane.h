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
    explicit StudentPlayPane(QWidget *parent = 0);
    ~StudentPlayPane();

    void setUser(UserInfo);
    void updateLevelInfo();
    void addClass(MapData info);

private:
    UserInfo user;
    std::unique_ptr<UserLevelInfo> levelInfo;
    QWidget *window;
    QSplitter *splitter;
    QVBoxLayout *layout;
    // List containing lists of maps
    QListWidget *vertList;
    // Contains level thumbnail and levelInfo
    QHBoxLayout *descriptionAreaLayout;
    QWidget *descriptionAreaWidget;
    // Contains level name, description, best score, and play button
    QWidget *levelInfoWidget;
    QVBoxLayout *levelInfoLayout;

    QLabel *levelThumbnail;
    QLabel *levelName;
    QLabel *levelDescription;
    QLabel *bestScore;
    QPushButton *playButton;


    void addClassRow(const ClassLevelInfo&);
    void updateDisplay();
};

#endif // STUDENTPLAYPANE_H
