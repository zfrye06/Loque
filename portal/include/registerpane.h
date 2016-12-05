#ifndef REGISTERPANE_H
#define REGISTERPANE_H

#include <QWidget>
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

private:
    Ui::RegisterPane *ui;

    void attemptRegistration();
};

#endif // REGISTERPANE_H
