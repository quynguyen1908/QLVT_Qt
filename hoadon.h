#ifndef HOADON_H
#define HOADON_H

#include "chitiethoadon.h"

// Khai báo cấu trúc thời gian
struct ThoiGian {
    int NGAY;
    int THANG;
    int NAM;
};

// Khai báo cấu trúc hóa đơn
struct HoaDon {
    char SOHD[21];         // Số hóa đơn
    ThoiGian NGAYLAP;      // Ngày lập hóa đơn
    char LOAI;             // Loại hóa đơn (N: Nhập, X: Xuất)
    PTRCTHD FirstCTHD;     // Con trỏ đến danh sách chi tiết hóa đơn
};

// Khai báo node danh sách liên kết đơn cho hóa đơn
struct nodeHD {
    HoaDon hd;
    nodeHD *next;
};

// Định nghĩa kiểu con trỏ danh sách liên kết
typedef nodeHD* PTRHD;

#endif // HOADON_H
