#include "editnhanviendialog.h"
#include "ui_editnhanviendialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include "vattu.h"

EditNhanVienDialog::EditNhanVienDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditNhanVienDialog)
{
    ui->setupUi(this);
    ui->txtMaNV->setEnabled(false);
}

EditNhanVienDialog::~EditNhanVienDialog()
{
    delete ui;
}

void EditNhanVienDialog::setNhanVien(const NhanVien &nv) {
    ui->txtMaNV->setText(nv.MANV);
    ui->txtHoNV->setText(nv.HO);
    ui->txtTenNV->setText(nv.TEN);
    if (QString(nv.PHAI).trimmed().compare("Nam", Qt::CaseInsensitive) == 0) {
        ui->radioNam->setChecked(true);
    } else {
        ui->radioNu->setChecked(true);
    }
}

void EditNhanVienDialog::updateNhanVien(NhanVien &nv) const {
    QString hoNV = normalizeName(ui->txtHoNV->text());
    QString tenNV = normalizeName(ui->txtTenNV->text());

    strncpy(nv.HO, hoNV.toStdString().c_str(), sizeof(nv.HO));
    nv.HO[sizeof(nv.HO) - 1] = '\0';

    strncpy(nv.TEN, tenNV.toStdString().c_str(), sizeof(nv.TEN));
    nv.TEN[sizeof(nv.TEN) - 1] = '\0';

    strncpy(nv.PHAI, ui->radioNam->isChecked() ? "NAM" : "NU", sizeof(nv.PHAI));
    nv.PHAI[sizeof(nv.PHAI) - 1] = '\0';
}

bool EditNhanVienDialog::isValidInput() {
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

    return true;
}

void EditNhanVienDialog::on_btnOK_clicked() {
    if (!isValidInput()) return;
    accept();
}


void EditNhanVienDialog::on_btnCancel_clicked() {
    reject();
}

