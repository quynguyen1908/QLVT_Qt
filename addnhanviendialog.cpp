#include "addnhanviendialog.h"
#include "ui_addnhanviendialog.h"
#include "nhanvien.h"
#include <QMessageBox>
#include <QRegularExpression>
#include "vattu.h"

AddNhanVienDialog::AddNhanVienDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddNhanVienDialog)
{
    ui->setupUi(this);
}

AddNhanVienDialog::~AddNhanVienDialog()
{
    delete ui;
}

NhanVien AddNhanVienDialog::getNhanVien() const {
    NhanVien nv;

    QString maNV = ui->txtMaNV->text().trimmed().toUpper();
    strncpy(nv.MANV, maNV.toStdString().c_str(), sizeof(nv.MANV));
    nv.MANV[sizeof(nv.MANV) - 1] = '\0';

    QString hoNV = normalizeName(ui->txtHoNV->text());
    strncpy(nv.HO, hoNV.toStdString().c_str(), sizeof(nv.HO));
    nv.HO[sizeof(nv.HO) - 1] = '\0';

    QString tenNV = normalizeName(ui->txtTenNV->text());
    strncpy(nv.TEN, tenNV.toStdString().c_str(), sizeof(nv.TEN));
    nv.TEN[sizeof(nv.TEN) - 1] = '\0';

    if (ui->radioNam->isChecked()) {
        strncpy(nv.PHAI, "NAM", sizeof(nv.PHAI));
    } else if (ui->radioNu->isChecked()) {
        strncpy(nv.PHAI, "NU", sizeof(nv.PHAI));
    }
    nv.PHAI[sizeof(nv.PHAI) - 1] = '\0';

    nv.FirstHD = nullptr;

    return nv;
}

bool AddNhanVienDialog::isValidInput() {
    // Kiểm tra mã nhân viên
    QString maNV = ui->txtMaNV->text().trimmed();

    // 1. Không được để trống
    if (maNV.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Mã nhân viên không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ hoặc số
    static const QRegularExpression validMaPattern("^[A-Za-z0-9]+$");
    if (!validMaPattern.match(maNV).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Mã nhân viên chỉ được chứa chữ cái hoặc số!");
        return false;
    }

    // Kiểm tra họ nhân viên
    QString hoNV = ui->txtHoNV->text().trimmed();

    // 1. Không được để trống
    if (hoNV.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Họ nhân viên không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ và khoảng trắng
    static const QRegularExpression validHoPattern("^[A-Za-zÀ-Ỵà-ỹ\\s]+$");
    if (!validHoPattern.match(hoNV).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Họ nhân viên chỉ được chứa chữ cái và khoảng trắng!");
        return false;
    }

    // Kiểm tra tên nhân viên
    QString tenNV = ui->txtTenNV->text().trimmed();

    // 1. Không được để trống
    if (tenNV.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Tên nhân viên không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ
    static const QRegularExpression validTenPattern("^[A-Za-zÀ-Ỵà-ỹ]+$");
    if (!validTenPattern.match(tenNV).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Tên nhân viên chỉ được chứa chữ cái!");
        return false;
    }

    // Kiểm tra giới tính nhân viên
    if (!ui->radioNam->isChecked() && !ui->radioNu->isChecked()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn giới tính nhân viên!");
        return false;
    }

    return true;
}

void AddNhanVienDialog::on_btnOK_clicked() {
    if (!isValidInput()) return;
    accept();
}


void AddNhanVienDialog::on_btnCancel_clicked() {
    reject();
}

