#ifndef REGISTERPANE_H
#define REGISTERPANE_H

#include <QWidget>
#include <QCheckBox>
#include "../shared/loqueclient.h"

namespace Ui {
class RegisterPane;
}

class RegisterPane : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPane(QWidget *parent = 0);
    ~RegisterPane();

signals:

    void onRegistration(UserInfo);
    void onLoginRequested();

private:
    Ui::RegisterPane *ui;

    void attemptRegistration();
    void adminBoxChanged(int state);
    void returnToLogin();
    void displayErrorMessage(const char *errorMessage);
};

#endif // REGISTERPANE_H
