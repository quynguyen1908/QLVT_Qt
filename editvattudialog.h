#ifndef EDITVATTUDIALOG_H
#define EDITVATTUDIALOG_H

#include <QDialog>
#include "vattu.h"

namespace Ui {
class EditVatTuDialog;
}

class EditVatTuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditVatTuDialog(QWidget *parent = nullptr);
    ~EditVatTuDialog();

    void setVatTu(const VatTu &vt);
    void updateVatTu(VatTu &vt) const;

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::EditVatTuDialog *ui;
    bool isValidInput();
};

#endif // EDITVATTUDIALOG_H
