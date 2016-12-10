#ifndef LEVELSETTINGSDIALOG_H
#define LEVELSETTINGSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include "../../shared/loqueclient.h"

namespace Ui {
class LevelSettingsDialog;
}

class LevelSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LevelSettingsDialog(int classID, std::vector<int> enabledIDs,  std::vector<LevelInfo> allLevels, int teacherID, QWidget *parent = 0);
    ~LevelSettingsDialog();

signals:
    void clicked(const int &id);
    void refresh();

private:
    QSignalMapper *mapper;
    Ui::LevelSettingsDialog *ui;
    QGroupBox* addLevel(LevelInfo lvlInfo, bool enabled);
    std::vector<int> levelsToDisable;
    std::vector<int> levelsToEnable;
    ClassStats cstats;
    int teacherID;
    int classID;
    std::vector<int> enabledIDs;
    std::unordered_map<int, QPushButton*> btns;
    void toggleLevel(const int &levelID);
    void submitLevels();

};

#endif // LEVELSETTINGSDIALOG_H
