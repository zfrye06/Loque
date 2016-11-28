#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
public slots:
    void sendUserInfo();
    void createAccount();
private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
