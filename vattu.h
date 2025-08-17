#ifndef VATTU_H
#define VATTU_H

#include <QWidget>
#include <QString>

// Khai báo cấu trúc vật tư
struct VatTu {
    char MAVT[11];  // Mã vật tư
    char TENVT[51]; // Tên vật tư
    char DVT[11];   // Đơn vị tính
    int TONKHO;     // Số lượng tồn kho
};

// Khai báo node cây nhị phân cho vật tư
struct nodeVT {
    VatTu vt;
    nodeVT *left;
    nodeVT *right;
};

// Định nghĩa kiểu con trỏ cây nhị phân
typedef nodeVT* Tree_VT;

// Khai báo các hàm
nodeVT* TimVatTu(Tree_VT root, const char* maVT);
bool ThemVatTu(Tree_VT &root, VatTu vt, QWidget *parent = nullptr);
QString normalizeName(const QString &raw);
QStringList getDanhSachMaVT(Tree_VT root);
nodeVT* XoaVatTu(nodeVT* root, const char* maVT);

#endif // VATTU_H
