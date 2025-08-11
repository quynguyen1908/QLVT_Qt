#include "addvattudialog.h"
#include "ui_addvattudialog.h"
#include <QMessageBox>

AddVatTuDialog::AddVatTuDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddVatTuDialog)
{
    ui->setupUi(this);
    ui->spinTonKho->setMinimum(1);
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

    QString tenVT = normalizeName(ui->txtTenVT->text());
    strncpy(vt.TENVT, tenVT.toStdString().c_str(), sizeof(vt.TENVT));
    vt.TENVT[sizeof(vt.TENVT) - 1] = '\0';

    QString dvt = normalizeName(ui->txtDVT->text());
    strncpy(vt.DVT, dvt.toStdString().c_str(), sizeof(vt.DVT));
    vt.DVT[sizeof(vt.DVT) -1] = '\0';

    vt.TONKHO = ui->spinTonKho->value();
    return vt;
}

bool AddVatTuDialog::isValidInput() {
    // Kiểm tra mã vật tư
    QString maVT = ui->txtMaVT->text().trimmed().toUpper();

    // 1. Không được để trống
    if (maVT.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Mã vật tư không được để trống!");
        return false;
    }

    // 2. Chỉ chứa ký tự chữ hoặc số
    static const QRegularExpression validPattern("^[A-Za-z0-9]+$");
    if (!validPattern.match(maVT).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Mã vật tư chỉ được chứa chữ cái hoặc số!");
        return false;
    }

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


void AddVatTuDialog::on_btnOK_clicked() {
    if (!isValidInput()) return;
    accept();
}


void AddVatTuDialog::on_btnCancel_clicked() {
    reject();
}

