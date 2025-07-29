#ifndef ADDNHANVIENDIALOG_H
#define ADDNHANVIENDIALOG_H

#include <QDialog>
#include "nhanvien.h"

namespace Ui {
class AddNhanVienDialog;
}

class AddNhanVienDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNhanVienDialog(QWidget *parent = nullptr);
    ~AddNhanVienDialog();

    NhanVien getNhanVien() const;

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::AddNhanVienDialog *ui;
    bool isValidInput();
};

#endif // ADDNHANVIENDIALOG_H
