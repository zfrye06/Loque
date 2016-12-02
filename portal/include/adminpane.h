#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class AdminPane;
}

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(QWidget *parent = 0);
    ~AdminPane();

private:
    Ui::AdminPane *ui;
    QTabWidget tabs;
    QListWidget userStatsList;
    QListWidget levelStatsList;
    QVBoxLayout summaryBoxLayout;
    QPushButton addClassButton;


    QLabel classSummaryLabel;
//    QLabel classNameLabel = QLabel("Class Name :");
//    QLabel classPointsLabel = QLabel("Total Points: ");
//    QLabel classTimeLabel = QLabel("Total Time Played");
//    QLabel enabledLevelsLabel = QLabel("Enabled Levels: ");
//    QLabel userLabel = QLabel("User");
//    QLabel totalScoreLabel = QLabel("Total Score");
//    QLabel totalTimeLabel = QLabel("Total Time Played");
//    QLabel lvl1Label = QLabel("Level 1");
//    QLabel lvl2Label = QLabel("Level 2");
//    QLabel lvl3Label = QLabel("Level 3");
//    QLabel mapUserLabel = QLabel("User");
//    QLabel levelLabel = QLabel("Level");
//    QLabel levelNameLabel = QLabel("Level Name");
//    QLabel levelScoreLabel = QLabel("Score");
//    QLabel completionTimeLabel = QLabel("Completion Time");

    void initConnections();

};

#endif // ADMINPANE_H
