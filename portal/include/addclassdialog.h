#ifndef ADDCLASSDIALOG_H
#define ADDCLASSDIALOG_H

#include <QDialog>
#include "../../shared/loqueclient.h"

namespace Ui {
class AddClassDialog;
}

class AddClassDialog : public QDialog
{
    Q_OBJECT

signals:
    void classCreated(ClassStats cstats);

public:
    explicit AddClassDialog(int userId, QWidget *parent = 0);
    ~AddClassDialog();

private:
    Ui::AddClassDialog *ui;
    int userId;
    void createClassroom(QString name);
    void showLabel(int classID);


};

#endif // ADDCLASSDIALOG_H
