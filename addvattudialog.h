#ifndef ADDVATTUDIALOG_H
#define ADDVATTUDIALOG_H

#include "vattu.h"
#include <QDialog>

namespace Ui {
class AddVatTuDialog;
}

class AddVatTuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddVatTuDialog(QWidget *parent = nullptr);
    ~AddVatTuDialog();

    VatTu getVatTu() const;

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::AddVatTuDialog *ui;
    bool isValidInput();
};

#endif // ADDVATTUDIALOG_H
