#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui  ->createAccountButton, &QPushButton::pressed, this, [this] { ui->worldTreeWidget->setCurrentIndex(1); });
    //connect(ui->haveAnAccountButton, &QPushButton::pressed, this, [this] { ui->worldTreeWidget->setCurrentIndex(0); });
}

MainWindow::~MainWindow()
{
    delete ui;
}
