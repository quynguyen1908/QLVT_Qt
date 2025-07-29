#ifndef EDITNHANVIENDIALOG_H
#define EDITNHANVIENDIALOG_H

#include <QDialog>
#include "nhanvien.h"

namespace Ui {
class EditNhanVienDialog;
}

class EditNhanVienDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNhanVienDialog(QWidget *parent = nullptr);
    ~EditNhanVienDialog();

    void setNhanVien(const NhanVien &nv);
    void updateNhanVien(NhanVien &nv) const;

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::EditNhanVienDialog *ui;
    bool isValidInput();
};

#endif // EDITNHANVIENDIALOG_H
