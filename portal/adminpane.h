#ifndef ADMINPANE_H
#define ADMINPANE_H

#include <QWidget>

namespace Ui {
class AdminPane;
}

class AdminPane : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPane(QWidget *parent = 0);
    ~AdminPane();

private:
    Ui::AdminPane *ui;
};

#endif // ADMINPANE_H
