#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "vattu.h"
#include "nhanvien.h"
#include "doanhthu.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnVatTu_clicked();

    void on_btnNhanVien_clicked();

    void on_btnLapHD_clicked();

    void on_btnThemVT_clicked();

    void on_tableNhanVien_cellClicked(int row, int column);

    void on_btnThemNV_clicked();

    void on_btnSuaNV_clicked();

    void on_btnXoaNV_clicked();

    void on_btnSapXepTheoTen_clicked();

    void on_btnSapXepTheoMa_clicked();

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void on_btnLuu_clicked();

    void on_btnInHD_clicked();

    void on_btnThongKe_clicked();

    void on_btnDoanhThu_clicked();

    void on_btnInDoanhThu_clicked();

private:
    Ui::MainWindow *ui;
    Tree_VT dsvt = nullptr;
    List_NV dsnv;
    void FillTableVatTu(QTableWidget* table, int choice);
    void FillTableNhanVien(QTableWidget* table);
    bool isValidHoaDon();
    void LoadComboBoxNhanVien();
    void LoadComboBoxVatTu();
    void addChiTietRow(int row);
};
#endif // MAINWINDOW_H
