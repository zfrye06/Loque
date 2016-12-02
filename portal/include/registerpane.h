#ifndef REGISTERPANE_H
#define REGISTERPANE_H

#include <QWidget>

namespace Ui {
class RegisterPane;
}

class RegisterPane : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPane(QWidget *parent = 0);
    ~RegisterPane();

private:
    Ui::RegisterPane *ui;
};

#endif // REGISTERPANE_H
