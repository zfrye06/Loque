#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <memory>
#include "createclassdialog.h"
#include "deleteclassconfirmation.h"
#include "levelsettingsdialog.h"
#include "../shared/loqueclient.h"
#include "ui_adminpane.h"

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(UserInfo user, QWidget *parent = 0);
    ~AdminPane();

    void updateClassStats();
    void setUser(UserInfo user);

signals:
    void classCreated(int classID);
private:
    UserInfo user;
    std::vector<LevelInfo> allLevels; 
    std::unique_ptr<std::vector<ClassStats>> allClassStats;
    int activeClassIdx;
    Ui::AdminPane *ui;
    std::unique_ptr<CreateClassDialog> createClassDialog; 
    std::unique_ptr<LevelSettingsDialog> levelSettingsDialog;
    std::unique_ptr<DeleteClassConfirmation> deleteClassConfirmation;
    std::map<std::string, UserStats> nameStats;
    std::string curUserTab;
    
    void refreshClassTabs();
    void rowSelected(QModelIndex);
    void classClicked(int);
    void showCreateClassDialog();
    void showHtmlReportDialog(); 
    void setUserTable();
    void setMapTable(std::string username);
    void setSummaryBox(std::string username);
    void deleteClass();
    void refreshEnabledLevels();
    void setUserList();
    void userSelected(QModelIndex index);
    QColor getLevelColor(const UserStats& user, int levelID);
    QIcon getIcon(const UserStats& user, int levelID);
    QString getFormattedTime(int seconds);
};



#endif // ADMINPANE_H
