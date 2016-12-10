#ifndef ADDCLASSDIALOG_H
#define ADDCLASSDIALOG_H

#include <QDialog>
#include "../../shared/loqueclient.h"
#include <QMessageBox>

namespace Ui {
class AddClassDialog;
}

class CreateClassDialog : public QDialog
{
    Q_OBJECT

signals:
    void classCreated(ClassStats cstats);

public:
    explicit CreateClassDialog(int userId, QWidget *parent = 0);
    ~CreateClassDialog();

private:
    Ui::AddClassDialog *ui;
    int userId;
    void createClassroom(QString name);
    void showLabel(int classID);


};

#endif // ADDCLASSDIALOG_H
