#ifndef CHITIETHOADON_H
#define CHITIETHOADON_H

// Khai báo cấu trúc chi tiết hóa đơn
struct ChiTietHoaDon {
    char MAVT[11];   // Mã vật tư
    int SOLUONG;     // Số lượng
    float DONGIA;    // Đơn giá
    int VAT;         // Thuế VAT
};

// Khai báo node danh sách liên kết đơn cho chi tiết hóa đơn
struct nodeCTHD {
    ChiTietHoaDon cthd;
    nodeCTHD *next;
};

// Định nghĩa kiểu con trỏ danh sách liên kết
typedef nodeCTHD* PTRCTHD;

#endif // CHITIETHOADON_H
