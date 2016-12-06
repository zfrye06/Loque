#ifndef CLASSTAB_H
#define CLASSTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include "../../shared/loqueclient.h"


namespace Ui {
class ClassTab;
}

class ClassTab : public QWidget
{
    Q_OBJECT

public:
    explicit ClassTab(ClassStats classStats, QWidget *parent = 0);
    ~ClassTab();

private:

    Ui::ClassTab *ui;
    QTableWidget *userStatsTable;
    QTableWidget *levelStatsTable;
    QVBoxLayout *mainLayout;
    QGroupBox *summaryBox;
    QVBoxLayout *summaryLayout;
    QGroupBox *levelArea;

    QLabel *classSummaryLabel;
    QLabel *classNameLabel;
    QLabel *classPointsLabel;
    QLabel *classTimeLabel;
    QLabel *enabledLevelsLabel;
    QLabel *userStatsLabel;
    QLabel *mapStatsLabel;

    void initWidgets();
    void initConnections();
    void setSummaryBox(const ClassStats& classStats);
    void setUserTable(const ClassStats& classStats);
    void setMapTable(const ClassStats& classStats);
    void setEnabledLevels(int classID);
    QColor getLevelColor(const UserStats& user, int levelID);
    QString getFormattedTime(int seconds);
};

#endif // CLASSTAB_H
