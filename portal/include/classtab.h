#ifndef CLASSTAB_H
#define CLASSTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGroupBox>


namespace Ui {
class ClassTab;
}

class ClassTab : public QWidget
{
    Q_OBJECT

public:
    explicit ClassTab(QWidget *parent = 0);
    ~ClassTab();

private:
    Ui::ClassTab *ui;
    QTableWidget *userStatsTable;
    QTableWidget *levelStatsTable;
    QVBoxLayout *mainLayout;
    QGroupBox *summaryBox;
    QVBoxLayout *summaryLayout;

    QLabel *classSummaryLabel;
    QLabel *classNameLabel;
    QLabel *classPointsLabel;
    QLabel *classTimeLabel;
    QLabel *enabledLevelsLabel;
    QLabel *userStatsLabel;
    QLabel *userLabel;
    QLabel *totalScoreLabel;
    QLabel *totalTimeLabel;
    QLabel *lvl1Label;
    QLabel *lvl2Label;
    QLabel *lvl3Label;
    QLabel *mapStatsLabel;
    QLabel *mapUserLabel;
    QLabel *levelLabel;
    QLabel *levelNameLabel;
    QLabel *levelScoreLabel;
    QLabel *completionTimeLabel;

    void initWidgets();
    void initConnections();
};

#endif // CLASSTAB_H
