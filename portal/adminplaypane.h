#ifndef ADMINPLAYPANE_H
#define ADMINPLAYPANE_H

#include <QWidget>

namespace Ui {
class AdminPlayPane;
}

class AdminPlayPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPlayPane(QWidget *parent = 0);
    ~AdminPlayPane();

private:
    Ui::AdminPlayPane *ui;
};

#endif // ADMINPLAYPANE_H
