#ifndef STUDENTPLAYPANE_H
#define STUDENTPLAYPANE_H

#include <QWidget>

namespace Ui {
class StudentPlayPane;
}

class StudentPlayPane : public QWidget
{
    Q_OBJECT

public:
    explicit StudentPlayPane(QWidget *parent = 0);
    ~StudentPlayPane();

private:
    Ui::StudentPlayPane *ui;
};

#endif // STUDENTPLAYPANE_H
