#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileio.h"
#include "addvattudialog.h"
#include "addnhanviendialog.h"
#include "editnhanviendialog.h"

#include <QMessageBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Vô hiệu các nút sửa/xóa
    ui->btnSuaNV->setEnabled(false);
    ui->btnXoaNV->setEnabled(false);

    // Kết nối sự kiện nhấn vào item trong table
    connect(ui->tableNhanVien, &QTableWidget::cellClicked, this, &MainWindow::on_tableNhanVien_cellClicked);

    // Chỉnh sửa header của các table
    ui->tableVatTu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableNhanVien->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableNV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableVT->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableCTHD->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableDoanhThu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Hiệu chỉnh trường nhập ngày
    ui->dateNgayLap->setDate(QDate::currentDate());
    ui->dateNgayLap->setMaximumDate(QDate::currentDate());
    ui->dateNgayLap->setDisplayFormat("dd/MM/yyyy");
    ui->dateFrom->setDate(QDate::currentDate());
    ui->dateFrom->setMaximumDate(QDate::currentDate());
    ui->dateFrom->setDisplayFormat("dd/MM/yyyy");
    ui->dateTo->setDate(QDate::currentDate());
    ui->dateTo->setMaximumDate(QDate::currentDate());
    ui->dateTo->setDisplayFormat("dd/MM/yyyy");

    // Đọc dữ liệu từ file .txt (cùng cấp với .cpp)
    LoadVatTuFromFile(dsvt, "../../vattu.txt");
    LoadNhanVienFromFile(dsnv, "../../nhanvien.txt");
    LoadComboBoxNhanVien();

    // Đổ dữ liệu vào bảng
    FillTableVatTu(ui->tableVatTu, 0);

    // Thêm sẵn 1 chi tiết vào bảng chi tiết
    addChiTietRow(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnVatTu_clicked() {
    FillTableVatTu(ui->tableVatTu, 0);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnNhanVien_clicked() {
    FillTableNhanVien(ui->tableNhanVien);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btnLapHD_clicked() {
    FillTableVatTu(ui->tableVT, 1);
    FillTableNhanVien(ui->tableNV);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btnThemVT_clicked() {
    AddVatTuDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        VatTu vt = dialog.getVatTu();
        if (!ThemVatTu(dsvt, vt)) {
            return;
        }
        QMessageBox::information(this, "Thông báo", QString("Đã thêm vật tư '%1' thành công!").arg(vt.MAVT));
        FillTableVatTu(ui->tableVatTu, 0);
        LoadComboBoxVatTu();
        SaveVatTuToFile(dsvt, "../../vattu.txt");
    }
}

void MainWindow::FillTableVatTu(QTableWidget* table, int choice) {
    table->clearContents();
    table->setRowCount(0);
    int row = 0;

    std::function<void(Tree_VT)> visit = [&](Tree_VT node) {
        if (!node) return;
        visit(node->left);

        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(node->vt.MAVT)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(node->vt.TENVT)));
        if (choice == 0) table->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(node->vt.DVT)));
        else table->setItem(row, 2, new QTableWidgetItem(QString::number(node->vt.TONKHO)));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(node->vt.TONKHO)));
        row++;

        visit(node->right);
    };

    visit(dsvt);
}

void MainWindow::FillTableNhanVien(QTableWidget* table) {
    table->setRowCount(0);
    for (int i = 0; i < dsnv.n; ++i) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(dsnv.nodes[i].MANV)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(dsnv.nodes[i].HO)));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(dsnv.nodes[i].TEN)));
        table->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(dsnv.nodes[i].PHAI)));
    }
}

void MainWindow::on_btnThemNV_clicked() {
    AddNhanVienDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        NhanVien nv = dialog.getNhanVien();
        if (!ThemNhanVien(dsnv, nv)) {
            return;
        }
        FillTableNhanVien(ui->tableNhanVien);
        LoadComboBoxNhanVien();
        SaveNhanVienToFile(dsnv, "../../nhanvien.txt");
    }
}

void MainWindow::on_tableNhanVien_cellClicked(int row, int column) {
    Q_UNUSED(row);
    Q_UNUSED(column);
    ui->btnSuaNV->setEnabled(true);
    ui->btnXoaNV->setEnabled(true);
}

void MainWindow::on_btnSuaNV_clicked() {
    int row = ui->tableNhanVien->currentRow();
    if (row < 0) return;

    // Lấy mã NV từ dòng được chọn
    QTableWidgetItem* item = ui->tableNhanVien->item(row, 0);
    if (!item) {
        QMessageBox::warning(this, "Lỗi", "Không thể lấy dữ liệu nhân viên.");
        return;
    }
    QString maNV = item->text();

    int index = TimNhanVien(dsnv, maNV.toStdString().c_str());
    if (index == -1) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy nhân viên!");
        return;
    }

    EditNhanVienDialog dialog(this);
    dialog.setNhanVien(dsnv.nodes[index]);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.updateNhanVien(dsnv.nodes[index]);
        FillTableNhanVien(ui->tableNhanVien);
        LoadComboBoxNhanVien();
        SaveNhanVienToFile(dsnv, "../../nhanvien.txt");
        QMessageBox::information(this, "Thông báo", "Đã sửa nhân viên!");
    }
}


void MainWindow::on_btnXoaNV_clicked() {
    int row = ui->tableNhanVien->currentRow();
    if (row < 0) return;

    QTableWidgetItem *item = ui->tableNhanVien->item(row, 0);
    if (!item) return;

    QString maNV = item->text();
    int index = TimNhanVien(dsnv, maNV.toStdString().c_str());
    if (index == -1) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy nhân viên để xóa!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Xác nhận",
        QString("Bạn có chắc muốn xóa nhân viên [%1]?").arg(maNV),
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply != QMessageBox::Yes) return;

    if (dsnv.nodes[index].FirstHD != nullptr) {
        QMessageBox::StandardButton hdReply = QMessageBox::question(
            this,
            "Xác nhận xóa hóa đơn",
            QString("Nhân viên [%1] có hóa đơn liên quan!\n"
                    "Nếu tiếp tục, toàn bộ hóa đơn và chi tiết sẽ bị xóa.\n"
                    "Bạn có chắc muốn tiếp tục?").arg(maNV),
            QMessageBox::Yes | QMessageBox::No
        );
        if (hdReply != QMessageBox::Yes) return;

        XoaToanBoHoaDon(dsnv.nodes[index]);
    }

    // Xóa nhân viên khỏi danh sách
    for (int i = index; i < dsnv.n - 1; ++i) {
        dsnv.nodes[i] = dsnv.nodes[i + 1];
    }
    dsnv.n--;

    FillTableNhanVien(ui->tableNhanVien);
    LoadComboBoxNhanVien();
    SaveNhanVienToFile(dsnv, "../../nhanvien.txt");
    QMessageBox::information(this, "Thông báo", "Đã xóa nhân viên!");
}


void MainWindow::on_btnSapXepTheoTen_clicked() {
    SortNhanVienTheoTen(dsnv);
    FillTableNhanVien(ui->tableNhanVien);
}


void MainWindow::on_btnSapXepTheoMa_clicked() {
    SortNhanVienTheoMa(dsnv);
    FillTableNhanVien(ui->tableNhanVien);
}

void MainWindow::on_btnAdd_clicked() {
    int row = ui->tableCTHD->rowCount();
    addChiTietRow(row);
}

void MainWindow::addChiTietRow(int row) {
    if (getDanhSachMaVT(dsvt).length() == ui->tableCTHD->rowCount()) return;

    ui->tableCTHD->insertRow(row);

    QComboBox *comboMAVT = new QComboBox(this);
    comboMAVT->addItems(getDanhSachMaVT(dsvt));
    ui->tableCTHD->setCellWidget(row, 0, comboMAVT);

    QSpinBox *spinSoLuong = new QSpinBox(this);
    spinSoLuong->setRange(1, 999999);
    ui->tableCTHD->setCellWidget(row, 1, spinSoLuong);

    QDoubleSpinBox *spinDonGia = new QDoubleSpinBox(this);
    spinDonGia->setRange(0, 1e9);
    spinDonGia->setDecimals(2);
    ui->tableCTHD->setCellWidget(row, 2, spinDonGia);

    QSpinBox *spinVAT = new QSpinBox(this);
    spinVAT->setRange(0, 100);
    ui->tableCTHD->setCellWidget(row, 3, spinVAT);
}

void MainWindow::on_btnDel_clicked() {
    int row = ui->tableCTHD->currentRow();
    if (row >= 0) {
        ui->tableCTHD->removeRow(row);
    }
}

void MainWindow::LoadComboBoxNhanVien() {
    ui->comboMaNV->clear(); // Xóa dữ liệu cũ nếu có

    for (int i = 0; i < dsnv.n; ++i) {
        QString maNV = QString::fromStdString(dsnv.nodes[i].MANV);
        ui->comboMaNV->addItem(maNV);
    }
}

void MainWindow::LoadComboBoxVatTu() {
    for (int i = 0; i < ui->tableCTHD->rowCount(); ++i) {
        QComboBox *comboMAVT = new QComboBox(this);
        comboMAVT->addItems(getDanhSachMaVT(dsvt));
        ui->tableCTHD->setCellWidget(i, 0, comboMAVT);
    }
}

void MainWindow::on_btnLuu_clicked() {
    if (!isValidHoaDon()) return;

    QString maNV = ui->comboMaNV->currentText().trimmed();
    QString soHD = ui->txtSoHD->text().trimmed().toUpper();
    QDate ngayLap = ui->dateNgayLap->date();
    char loaiHD = ui->radioNhap->isChecked() ? 'N' : 'X';

    int index = TimNhanVien(dsnv, maNV.toStdString().c_str());
    if (index == -1) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy nhân viên!");
        return;
    }

    // Tạo hóa đơn mới
    PTRHD newHD = new nodeHD;
    strncpy(newHD->hd.SOHD, soHD.toStdString().c_str(), sizeof(newHD->hd.SOHD));
    newHD->hd.NGAYLAP.NGAY = ngayLap.day();
    newHD->hd.NGAYLAP.THANG = ngayLap.month();
    newHD->hd.NGAYLAP.NAM = ngayLap.year();
    newHD->hd.LOAI = loaiHD;
    newHD->hd.FirstCTHD = nullptr;
    newHD->next = nullptr;

    // Tạo danh sách chi tiết
    PTRCTHD *pCT = &newHD->hd.FirstCTHD;
    for (int i = 0; i < ui->tableCTHD->rowCount(); ++i) {
        auto cbMAVT = qobject_cast<QComboBox *>(ui->tableCTHD->cellWidget(i, 0));
        auto spinSL = qobject_cast<QSpinBox *>(ui->tableCTHD->cellWidget(i, 1));
        auto spinDG = qobject_cast<QDoubleSpinBox *>(ui->tableCTHD->cellWidget(i, 2));
        auto spinVAT = qobject_cast<QSpinBox *>(ui->tableCTHD->cellWidget(i, 3));

        PTRCTHD ct = new nodeCTHD;
        strncpy(ct->cthd.MAVT, cbMAVT->currentText().toStdString().c_str(), sizeof(ct->cthd.MAVT));
        ct->cthd.SOLUONG = spinSL->value();
        ct->cthd.DONGIA = spinDG->value();
        ct->cthd.VAT = spinVAT->value();
        ct->next = nullptr;

        // Cập nhật tồn kho
        nodeVT* node = TimVatTu(dsvt, ct->cthd.MAVT);
        if (node != nullptr) {
            if (newHD->hd.LOAI == 'X') {
                node->vt.TONKHO -= ct->cthd.SOLUONG;
            } else if (newHD->hd.LOAI == 'N') {
                node->vt.TONKHO += ct->cthd.SOLUONG;
            }
        }

        // Thêm vào cuối danh sách chi tiết
        *pCT = ct;
        pCT = &ct->next;
    }

    // Thêm vào đầu danh sách hóa đơn của nhân viên
    newHD->next = dsnv.nodes[index].FirstHD;
    dsnv.nodes[index].FirstHD = newHD;

    FillTableVatTu(ui->tableVT, 1);
    LoadComboBoxNhanVien();
    LoadComboBoxVatTu();
    SaveVatTuToFile(dsvt, "../../vattu.txt");
    SaveNhanVienToFile(dsnv, "../../nhanvien.txt");
    QMessageBox::information(this, "Thành công", "Đã lưu hóa đơn!");
}

bool MainWindow::isValidHoaDon() {
    // Mã nhân viên
    QString maNV = ui->comboMaNV->currentText().trimmed();
    if (maNV.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Chưa chọn mã nhân viên!");
        return false;
    }
    if (TimNhanVien(dsnv, maNV.toStdString().c_str()) == -1) {
        QMessageBox::warning(this, "Lỗi", "Mã nhân viên không tồn tại!");
        return false;
    }

    // Số hóa đơn
    QString soHD = ui->txtSoHD->text().trimmed().toUpper();
    if (soHD.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Số hóa đơn không được để trống!");
        return false;
    }

    static const QRegularExpression re("^[A-Za-z0-9]+$");
    if (!re.match(soHD).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Số hóa đơn chỉ được chứa chữ và số!");
        return false;
    }

    PTRHD hd = TimHoaDonTheoSoHD(dsnv, soHD.toStdString().c_str());
    if (hd != nullptr) {
        QMessageBox::warning(this, "Lỗi", QString("Hóa đơn %1 đã tồn tại!").arg(soHD));
        return false;
    }

    // Ngày lập
    QDate ngayLap = ui->dateNgayLap->date();
    if (ngayLap.isNull() || ngayLap > QDate::currentDate()) {
        QMessageBox::warning(this, "Lỗi", "Ngày lập không hợp lệ!");
        return false;
    }

    // Loại hóa đơn
    char loaiHD = ui->radioNhap->isChecked() ? 'N' :
                    ui->radioXuat->isChecked() ? 'X' : '\0';
    if (loaiHD == '\0') {
        QMessageBox::warning(this, "Lỗi", "Chưa chọn loại hóa đơn!");
        return false;
    }

    // Chi tiết hóa đơn
    int rowCount = ui->tableCTHD->rowCount();
    if (rowCount == 0) {
        QMessageBox::warning(this, "Lỗi", "Chi tiết hóa đơn không được rỗng!");
        return false;
    }

    QSet<QString> seenMAVT;

    for (int row = 0; row < rowCount; ++row) {
        // Lấy dữ liệu
        QComboBox *comboMAVT = qobject_cast<QComboBox *>(ui->tableCTHD->cellWidget(row, 0));
        if (!comboMAVT) continue;
        QString maVT = comboMAVT->currentText().trimmed();

        QSpinBox *spinSoLuong = qobject_cast<QSpinBox *>(ui->tableCTHD->cellWidget(row, 1));
        if (!spinSoLuong) continue;
        int soLuong = spinSoLuong->value();

        QDoubleSpinBox *spinDonGia = qobject_cast<QDoubleSpinBox *>(ui->tableCTHD->cellWidget(row, 2));
        if (!spinDonGia) continue;
        float donGia = spinDonGia->value();

        QSpinBox *spinVAT = qobject_cast<QSpinBox *>(ui->tableCTHD->cellWidget(row, 3));
        if (!spinVAT) continue;
        int VAT = spinVAT->value();

        // Kiểm tra
        if (!comboMAVT || !spinSoLuong || !spinDonGia || !spinVAT) {
            QMessageBox::warning(this, "Lỗi", QString("Hàng %1 bị thiếu thông tin!").arg(row + 1));
            return false;
        }

        if (maVT.isEmpty()) {
            QMessageBox::warning(this, "Lỗi", QString("Mã vật tư ở hàng %1 không được để trống!").arg(row + 1));
            return false;
        }

        Tree_VT vt = TimVatTu(dsvt, maVT.toStdString().c_str());
        if (!vt) {
            QMessageBox::warning(this, "Lỗi", QString("Vật tư '%1' không tồn tại!").arg(maVT));
            return false;
        }

        if (seenMAVT.contains(maVT)) {
            QMessageBox::warning(this, "Lỗi", QString("Dòng %1: Mã vật tư bị trùng!").arg(row + 1));
            return false;
        }

        if (soLuong <= 0) {
            QMessageBox::warning(this, "Lỗi", QString("Số lượng ở hàng %1 phải lớn hơn 0!").arg(row + 1));
            return false;
        }

        if (loaiHD == 'X' && soLuong > vt->vt.TONKHO) {
            QMessageBox::warning(this, "Lỗi", QString("Vật tư '%1' chỉ còn %2 trong kho, không thể xuất %3!")
                                                  .arg(maVT).arg(vt->vt.TONKHO).arg(soLuong));
            return false;
        }

        if (donGia <= 0.0) {
            QMessageBox::warning(this, "Lỗi", QString("Đơn giá ở hàng %1 phải lớn hơn 0!").arg(row + 1));
            return false;
        }

        if (VAT < 0 || VAT > 100) {
            QMessageBox::warning(this, "Lỗi", QString("VAT ở hàng %1 phải từ 0 đến 100!").arg(row + 1));
            return false;
        }

        seenMAVT.insert(maVT);
    }
    return true;
}

void MainWindow::on_btnInHD_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btnThongKe_clicked() {
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_btnDoanhThu_clicked() {
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_btnInDoanhThu_clicked() {
    QDate fromDate = ui->dateFrom->date();
    QDate toDate = ui->dateTo->date();

    if (fromDate > toDate) {
        QMessageBox::warning(this, "Lỗi", "Ngày bắt đầu không được lớn hơn ngày kết thúc!");
        return;
    }

    ThoiGian from = { fromDate.day(), fromDate.month(), fromDate.year() };
    ThoiGian to   = { toDate.day(),   toDate.month(),   toDate.year() };

    // Tính doanh thu
    PTRDT dsdt = nullptr;
    TinhDoanhThu(dsnv, from, to, dsdt);

    // Sắp xếp và lấy top 10
    SapXepDoanhThu(dsdt);
    dsdt = LayTopN(dsdt, 10);

    // Đổ dữ liệu ngày vào label
    QString ngayText = QString("Từ ngày %1/%2/%3 đến %4/%5/%6")
                           .arg(from.NGAY).arg(from.THANG).arg(from.NAM)
                           .arg(to.NGAY).arg(to.THANG).arg(to.NAM);
    ui->lblNgayIn->setText(ngayText);

    // Đổ dữ liệu vào bảng
    ui->tableDoanhThu->setRowCount(0);
    int row = 0;
    PTRDT p = dsdt;

    while (p) {
        nodeVT* vt = TimVatTu(dsvt, p->dt.MAVT);
        if (!vt) continue;

        QLocale locale = QLocale(QLocale::Vietnamese);
        QString doanhThuStr = locale.toString(p->dt.DOANHTHU, 'f', 2);

        ui->tableDoanhThu->insertRow(row);
        ui->tableDoanhThu->setItem(row, 0, new QTableWidgetItem(vt->vt.MAVT));
        ui->tableDoanhThu->setItem(row, 1, new QTableWidgetItem(vt->vt.TENVT));
        ui->tableDoanhThu->setItem(row, 2, new QTableWidgetItem(vt->vt.DVT));
        ui->tableDoanhThu->setItem(row, 3, new QTableWidgetItem(doanhThuStr));

        row++;
        p = p->next;
    }
}
