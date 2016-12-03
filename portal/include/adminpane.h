#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QPushButton>

namespace Ui {
class AdminPane;
}

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(int teacherID, QWidget *parent = 0);
    ~AdminPane();

private:
    int teacherID;

    Ui::AdminPane *ui;
    QHBoxLayout *mainLayout;
    QTabWidget *tabs;
    QPushButton *addClassButton;
    QIcon addClassIcon = QIcon(QPixmap(":/assets/add"));

    void initWidgets();
    void initConnections();

};

#endif // ADMINPANE_H
