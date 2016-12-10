#ifndef LEVELSETTINGSDIALOG_H
#define LEVELSETTINGSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include "../../shared/loqueclient.h"
#include "ui_levelsettingsdialog.h"

namespace Ui {
class LevelSettingsDialog;
}

class LevelSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    LevelSettingsDialog(int teacherId, int classId,
                        const std::vector<LevelInfo>& enabledLevels,
                        const std::vector<LevelInfo>& allLevels,
                        QWidget *parent = 0);
    ~LevelSettingsDialog();

signals:
    void clicked(const int &id);
    void refresh();

private:
    int teacherId;
    int classId;
    std::vector<int> enabledIds;
    
    Ui::LevelSettingsDialog *ui;
    QSignalMapper *mapper;
    std::vector<int> levelsToDisable;
    std::vector<int> levelsToEnable;
    std::unordered_map<int, QPushButton*> btns;
    QGroupBox* addLevel(const LevelInfo& lvlInfo, bool enabled);
    void toggleLevel(const int &levelID);
    void submitLevels();

};

#endif // LEVELSETTINGSDIALOG_H
