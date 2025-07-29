#include "addvattudialog.h"
#include "ui_addvattudialog.h"
#include <QMessageBox>

AddVatTuDialog::AddVatTuDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddVatTuDialog)
{
    ui->setupUi(this);
    ui->spinTonKho->setMinimum(0);
    ui->spinTonKho->setMaximum(999999);
}

AddVatTuDialog::~AddVatTuDialog()
{
    delete ui;
}

VatTu AddVatTuDialog::getVatTu() const {
    VatTu vt;

    QString maVT = ui->txtMaVT->text().trimmed().toUpper();
    strncpy(vt.MAVT, maVT.toStdString().c_str(), sizeof(vt.MAVT));
    vt.MAVT[sizeof(vt.MAVT) - 1] = '\0';

    strncpy(vt.TENVT, ui->txtTenVT->text().toStdString().c_str(), sizeof(vt.TENVT));
    strncpy(vt.DVT, ui->txtDVT->text().toStdString().c_str(), sizeof(vt.DVT));
    vt.TONKHO = ui->spinTonKho->value();
    return vt;
}

bool AddVatTuDialog::isValidInput() {
    QString maVT = ui->txtMaVT->text().trimmed();

    // 1. Không được để trống
    if (maVT.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Mã vật tư không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ hoặc số (không có ký tự đặc biệt)
    static const QRegularExpression validPattern("^[A-Za-z0-9]+$");
    if (!validPattern.match(maVT).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Mã vật tư chỉ được chứa chữ cái hoặc số (không chứa ký tự đặc biệt)!");
        return false;
    }

    return true;
}


void AddVatTuDialog::on_btnOK_clicked() {
    if (!isValidInput()) return;
    accept();
}


void AddVatTuDialog::on_btnCancel_clicked() {
    reject();
}

