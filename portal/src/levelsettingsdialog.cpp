#include "levelsettingsdialog.h"
#include "ui_levelsettingsdialog.h"

LevelSettingsDialog::LevelSettingsDialog(std::vector<int> enabledLevels, std::vector<LevelInfo> allLevels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LevelSettingsDialog)
{
    ui->setupUi(this);
    QGridLayout *mainLayout = new QGridLayout;

    int row = 0;
    int col = 0;

    for(int i = 0; i < allLevels.size(); i++){
        mainLayout->addWidget(new LevelBox(allLevels[i], std::find(enabledLevels.begin(), enabledLevels.end(), allLevels[i].id) != enabledLevels.end(), this), row, col++, 1, 1);
        if(col > 2){
            row++;
            col = 0;
        }
    }
//    mainLayout->addWidget(new LevelBox(allLevels[0], std::find(enabledLevels.begin(), enabledLevels.end(), allLevels[0].id) != enabledLevels.end(), this), row, col++, 1, 1);

    setLayout(mainLayout);
}

LevelSettingsDialog::~LevelSettingsDialog()
{
    delete ui;
}

LevelBox::LevelBox(LevelInfo lvlInfo, bool enabled, QWidget *parent) : enabled(enabled), QWidget(parent)
{
    QGroupBox *box = new QGroupBox;
    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QPixmap img;
    for( int i=0;i<MapCount;i++ ) {
        if ( Maps[i].id == lvlInfo.id ) {
            img = QPixmap(Maps[i].thumbnail.c_str());
        }
    }
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
    box->setParent(parent);
    box->setFixedSize(QSize(500, 250));
}

LevelBox::~LevelBox()
{

}
