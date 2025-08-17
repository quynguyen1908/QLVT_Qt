#include "editvattudialog.h"
#include "ui_editvattudialog.h"
#include <QMessageBox>
#include <QRegularExpression>

EditVatTuDialog::EditVatTuDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditVatTuDialog)
{
    ui->setupUi(this);
    ui->txtMaVT->setEnabled(false);
}

EditVatTuDialog::~EditVatTuDialog()
{
    delete ui;
}

void EditVatTuDialog::setVatTu(const VatTu &vt) {
    ui->txtMaVT->setText(vt.MAVT);
    ui->txtTenVT->setText(vt.TENVT);
    ui->txtDVT->setText(vt.DVT);
}

void EditVatTuDialog::updateVatTu(VatTu &vt) const {
    QString tenVT = normalizeName(ui->txtTenVT->text());
    QString dvt = normalizeName(ui->txtDVT->text());

    strncpy(vt.TENVT, tenVT.toStdString().c_str(), sizeof(vt.TENVT));
    vt.TENVT[sizeof(vt.TENVT) - 1] = '\0';

    strncpy(vt.DVT, dvt.toStdString().c_str(), sizeof(vt.DVT));
    vt.DVT[sizeof(vt.DVT) - 1] = '\0';
}

void EditVatTuDialog::on_btnOK_clicked() {
    if (!isValidInput()) return;
    accept();
}

void EditVatTuDialog::on_btnCancel_clicked() {
    reject();
}

bool EditVatTuDialog::isValidInput() {
    // Kiểm tra tên vật tư
    QString tenVT = ui->txtTenVT->text().trimmed();

    // 1. Không được để trống
    if (tenVT.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Tên vật tư không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ và khoảng trắng
    static const QRegularExpression validNamePattern("^[A-Za-zÀ-Ỵà-ỹ\\s]+$");
    if (!validNamePattern.match(tenVT).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Tên vật tư chỉ được chứa chữ cái và khoảng trắng!");
        return false;
    }

    // Kiểm tra đơn vị tính
    QString dvt = ui->txtDVT->text().trimmed();

    // 1. Không được để trống
    if (dvt.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Đơn vị tính không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ và khoảng trắng
    if (!validNamePattern.match(dvt).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Đơn vị tính chỉ được chứa chữ cái và khoảng trắng!");
        return false;
    }

    return true;
}
