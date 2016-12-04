#ifndef LOGINPANE_H
#define LOGINPANE_H

#include <QWidget>
#include "../shared/loqueclient.h"

namespace Ui {
class LoginPane;
}

class LoginPane : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPane(QWidget *parent = 0);
    ~LoginPane();

signals:

    void onLogin(UserInfo);
    void onSignupRequested();

private:
    
    Ui::LoginPane *ui;

    void attemptLogin();
};

#endif // LOGINPANE_H
