#ifndef LOGINPANE_H
#define LOGINPANE_H

#include <QWidget>

namespace Ui {
class LoginPane;
}

class LoginPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPane(QWidget *parent = 0);
    ~LoginPane();

private:
    
    Ui::LoginPane *ui;
};

#endif // LOGINPANE_H
