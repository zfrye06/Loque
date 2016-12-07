#ifndef ADDCLASSDIALOG_H
#define ADDCLASSDIALOG_H

#include <QDialog>

namespace Ui {
class AddClassDialog;
}

class AddClassDialog : public QDialog
{
    Q_OBJECT

signals:
    void nameSubmitted(QString name);

public:
    explicit AddClassDialog(QWidget *parent = 0);
    ~AddClassDialog();

private:
    Ui::AddClassDialog *ui;


};

#endif // ADDCLASSDIALOG_H
