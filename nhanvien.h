#ifndef NHANVIEN_H
#define NHANVIEN_H

#include "hoadon.h"
#include <QWidget>

const int MAX_NV = 500;

// Khai báo cấu trúc nhân viên
struct NhanVien {
    char MANV[11];    // Mã nhân viên
    char HO[51];      // Họ
    char TEN[11];     // Tên
    char PHAI[4];     // Giới tính
    PTRHD FirstHD;    // Con trỏ đến danh sách hóa đơn
};

// Khai báo danh sách nhân viên
struct List_NV {
    int n;            // Số lượng nhân viên
    NhanVien nodes[MAX_NV]; // Mảng nhân viên
};

// Khai báo các hàm
int TimNhanVien(List_NV &dsnv, const char *maNV);
bool ThemNhanVien(List_NV &dsnv, const NhanVien &nv, QWidget *parent = nullptr);
void XoaToanBoHoaDon(NhanVien &nv);
void SortNhanVienTheoMa(List_NV &dsnv);
void SortNhanVienTheoTen(List_NV &dsnv);
PTRHD TimHoaDonTheoSoHD(const List_NV &dsnv, const char *soHD);
bool KiemTraVatTuTrongHoaDon(List_NV &dsnv, const char* maVT);
double TinhTriGiaHD(const HoaDon &hd);

#endif // NHANVIEN_H
