#ifndef DELETECLASSCONFIRMATION_H
#define DELETECLASSCONFIRMATION_H

#include <QDialog>

namespace Ui {
class DeleteClassConfirmation;
}

class DeleteClassConfirmation : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteClassConfirmation(QWidget *parent = 0);
    ~DeleteClassConfirmation();

private:
    Ui::DeleteClassConfirmation *ui;
};

#endif // DELETECLASSCONFIRMATION_H
