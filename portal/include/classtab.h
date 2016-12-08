#ifndef CLASSTAB_H
#define CLASSTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include "addclassdialog.h"
#include "../../shared/loqueclient.h"


namespace Ui {
class ClassTab;
}

class ClassTab : public QWidget
{
    Q_OBJECT

public:
    explicit ClassTab(ClassStats classStats, int teacherID, QWidget *parent = 0);
    ~ClassTab();

signals:
    void classCreated(int classID);

private:

    QTableWidget *userStatsTable;
    QTableWidget *levelStatsTable;
    QVBoxLayout *mainLayout;
    QGroupBox *summaryBox;
    QVBoxLayout *summaryLayout;
    QHBoxLayout *topLayout;
    QScrollArea *tableArea;
    QVBoxLayout *scrollLayout;
    std::unique_ptr<AddClassDialog> currClassDialog;
    int teacherID;
    int classID;

    QLabel *classSummaryLabel;
    QLabel *classIdLabel;
    QLabel *classNameLabel;
    QLabel *classPointsLabel;
    QLabel *classTimeLabel;
    QLabel *enabledLevelsLabel;
    QLabel *userStatsLabel;
    QLabel *levelStatsLabel;

    void initWidgets();
    void setSummaryBox(const ClassStats& classStats);
    void setUserTable(const ClassStats& classStats);
    void setMapTable(const ClassStats& classStats);
    QColor getLevelColor(const UserStats& user, int levelID);
    QString getFormattedTime(int seconds);
};

#endif // CLASSTAB_H
