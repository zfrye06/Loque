#include <QFileDialog>
#include <fstream>
#include <iostream>
#include "adminpane.h"
#include "classtab.h"
#include "report.h"

AdminPane::AdminPane(UserInfo user, QWidget *parent) :
    QWidget(parent),
    user(user),
    allClassStats(new std::vector<ClassStats>()),
    activeClassIdx(-1),
    ui(new Ui::AdminPane),
    createClassDialog(nullptr),
    levelSettingsDialog(nullptr),
    deleteClassConfirmation(nullptr)
{
    ui->setupUi(this);
    ui->classList->setAttribute(Qt::WA_MacShowFocusRect, 0);

    connect(ui->classList, &QListWidget::currentRowChanged,
            this, &AdminPane::classClicked);

    connect(ui->clcreateClassButton, &QPushButton::clicked,
            this, &AdminPane::showCreateClassDialog);

    connect(ui->ncCreateClassButton, &QPushButton::clicked,
            this, &AdminPane::showCreateClassDialog);

    connect(ui->exportHtmlButton, &QPushButton::clicked,
            this, &AdminPane::showHtmlReportDialog); 

    connect(ui->levelSettingsButton, &QPushButton::clicked,
            this, [this] {
        if (activeClassIdx == -1) return;
        auto& activeClass = this->allClassStats->at(activeClassIdx);
        int activeClassId = activeClass.classId;
       levelSettingsDialog.reset(new LevelSettingsDialog(this->user.userId, activeClassId,
                                                         activeClass.enabledLevels, this->allLevels));
       levelSettingsDialog->show();
    });

    connect(ui->deleteClassButton, &QPushButton::clicked,
            this, [this] {
        deleteClassConfirmation.reset(new DeleteClassConfirmation);
        deleteClassConfirmation->show();
        connect(deleteClassConfirmation.get(), &DeleteClassConfirmation::accepted,
                this, &AdminPane::refreshClassTabs);
    });

    refreshClassTabs();

    // Populate list of all levels. This is a one-time operation.
    LoqueClient client;
    auto status = client.getAllLevels(this->allLevels);
    if (status != Status::OK) {
        std::cerr << "ERROR: Unable to populate list of all levels. " <<
            "Client returned status " << status << std::endl;

        // TODO: Should we really disable level settings in this case? 
        ui->levelSettingsButton->setEnabled(false); 
    }
}

AdminPane::~AdminPane() {
    delete ui;
}

void AdminPane::refreshClassTabs() {
    std::unique_ptr<std::vector<ClassStats> > newClassStats(new std::vector<ClassStats>);
    LoqueClient client;
    auto status = client.getAllClassStats(user.userId, *newClassStats);
    if (status != Status::OK) {
        // TODO: Show the user that we couldn't get the info.
        std::cerr << "ERROR: Unable to download class stats. " <<
            "Client returned status" << status << std::endl;
        return;
    }
    allClassStats.swap(newClassStats);
    // ui->classList->clear();
    // ui->studentsList->clear();
    // if (allClassStats->size() == 0) {
    //     activeClassIdx = -1;
    //     ui->stackedWidget->setCurrentWidget(ui->noClassesPage);
    //     return;
    // }
    // for (auto& classStats : *allClassStats) {
    //     ui->classList->addItem(QString::fromStdString(classStats.className));
    // }
    // ui->classList->item(0)->setSelected(true);
    // classClicked(0);
}

void AdminPane::classClicked(int row) {
    // if (row == -1) return;
    // activeClassIdx = row;
    // const ClassStats& currStats = allClassStats->at(activeClassIdx);
    // ui->studentsList->clear();
    // for (auto& ustats : currStats.studentStats) {
    //     ui->studentsList->addItem(QString::fromStdString(ustats.username));
    // }
    // ui->currClassLabel->setText(QString::fromStdString(currStats.className));
}

void AdminPane::showCreateClassDialog() {
    createClassDialog.reset(new CreateClassDialog(user.userId));
    createClassDialog->show();
    connect(createClassDialog.get(), &CreateClassDialog::classCreated,
            this, &AdminPane::refreshClassTabs); 
}

void AdminPane::showHtmlReportDialog() {
    if (activeClassIdx < 0) return;
    try {
        QString filter = "HTML File (*.html)";
        QString filename = QFileDialog::getSaveFileName(this, tr("Create HTML Report"),
                                                        QString(), filter, &filter);
        std::ofstream out(filename.toStdString());
        writeHtmlReport(out, allClassStats->at(activeClassIdx));
        out.close();
    } catch (std::exception& e) {
        std::cerr << "ERROR: Unable to save html report. " << e.what() << std::endl;
    }
}
