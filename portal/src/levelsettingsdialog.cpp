#include "levelsettingsdialog.h"
#include "ui_levelsettingsdialog.h"

LevelSettingsDialog::LevelSettingsDialog(std::vector<int> enabledLevels, std::vector<LevelInfo> allLevels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LevelSettingsDialog)
{
    ui->setupUi(this);

    int row = 0;
    int col = 0;

    QListWidgetItem *item;
    for(size_t i = 0; i < allLevels.size(); i++){
        item = new QListWidgetItem;
        ui->levelList->setItemWidget(item, addLevel(allLevels[i], std::find(enabledLevels.begin(), enabledLevels.end(), allLevels[i].id) != enabledLevels.end()));
//        ui->levelList->  addWidget(new LevelBox(allLevels[i], std::find(enabledLevels.begin(), enabledLevels.end(), allLevels[i].id) != enabledLevels.end(), this), row, col++, 1, 1);
        if(col > 2){
            row++;
            col = 0;
        }
    }
}

QGroupBox* LevelSettingsDialog::addLevel(LevelInfo lvlInfo, bool enabled){
    QGroupBox *box = new QGroupBox;
    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QPixmap img(":/assets/candySky.jpg");
    QLabel *imgLabel = new QLabel;
    imgLabel->setPixmap(img);
    QLabel *idLabel = new QLabel(QString::number(lvlInfo.id));
    QLabel *nameLabel = new QLabel(QString::fromStdString(lvlInfo.name));
    QLabel *descriptionLabel = new QLabel(QString::fromStdString(lvlInfo.description));
    QPushButton *toggleButton = new QPushButton(enabled ? "Disable Level" : "Enable Level");

    vLayout->addWidget(idLabel);
    vLayout->addWidget(nameLabel);
    vLayout->addWidget(descriptionLabel);
    vLayout->addWidget(toggleButton);
    hLayout->addWidget(imgLabel);
    hLayout->addLayout(vLayout);
    box->setLayout(hLayout);
    box->setFixedSize(QSize(500, 250));
    return box;
}

LevelSettingsDialog::~LevelSettingsDialog()
{
    delete ui;
}
