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
    void createClass(std::string name);

public:
    explicit CreateClassDialog(QWidget *parent = 0);
    ~CreateClassDialog();

private:
    Ui::AddClassDialog *ui;
};

#endif // ADDCLASSDIALOG_H
