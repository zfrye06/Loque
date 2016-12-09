#ifndef LEVELSETTINGSDIALOG_H
#define LEVELSETTINGSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "../../shared/loqueclient.h"

namespace Ui {
class LevelSettingsDialog;
}

class LevelSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LevelSettingsDialog(std::vector<int> enabledLevels, std::vector<LevelInfo> allLevels, QWidget *parent = 0);
    ~LevelSettingsDialog();

private:
    Ui::LevelSettingsDialog *ui;
    QGroupBox* addLevel(LevelInfo lvlInfo, bool enabled);
};

#endif // LEVELSETTINGSDIALOG_H
