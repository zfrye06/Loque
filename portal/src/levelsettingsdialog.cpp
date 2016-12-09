#include "levelsettingsdialog.h"
#include "ui_levelsettingsdialog.h"
#include <QListView>

LevelSettingsDialog::LevelSettingsDialog(std::vector<int> enabledLevels, std::vector<LevelInfo> allLevels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LevelSettingsDialog)
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(true);
    QFrame *frame = new QFrame(ui->scrollArea);
    ui->scrollArea->setWidget(frame);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QGridLayout *layout = new QGridLayout(frame);

    int row = 0;
    int col = 0;
    for(size_t i = 0; i < allLevels.size(); i++){
        layout->addWidget(addLevel(allLevels[i], std::find(enabledLevels.begin(), enabledLevels.end(), allLevels[i].id) != enabledLevels.end()), row, col++);
        if(col > 1){
            row++;
            col = 0;
        }
    }

//    layout->addWidget(addLevel(allLevels[0], std::find(enabledLevels.begin(), enabledLevels.end(), allLevels[0].id) != enabledLevels.end()), row, col++);
    frame->setLayout(layout);
}

QGroupBox* LevelSettingsDialog::addLevel(LevelInfo lvlInfo, bool enabled){
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
    imgLabel->setPixmap(img.scaled(250, 250, Qt::KeepAspectRatio));
    QLabel *idLabel = new QLabel(QString::number(lvlInfo.id));
    QLabel *nameLabel = new QLabel(QString::fromStdString(lvlInfo.name));
    QLabel *descriptionLabel = new QLabel(QString::fromStdString(lvlInfo.description));
    descriptionLabel->setWordWrap(true);
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
