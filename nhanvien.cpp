#include "nhanvien.h"
#include <QMessageBox>
#include <QString>
#include <cstring>

// Hàm tìm nhân viên theo mã
int TimNhanVien(List_NV &dsnv, const char *maNV) {
    for (int i = 0; i < dsnv.n; ++i) {
        if (strcmp(dsnv.nodes[i].MANV, maNV) == 0) {
            return i;
        }
    }
    return -1;
}

// Thêm nhân viên vào danh sách tuyến tính
bool ThemNhanVien(List_NV &dsnv, const NhanVien &nv, QWidget *parent) {
    if (dsnv.n >= MAX_NV) {
        QMessageBox::warning(parent, "Lỗi", QString("Danh sách nhân viên đã đầy!"));
        return false;
    }

    if (TimNhanVien(dsnv, nv.MANV) != -1) {
        QMessageBox::warning(parent, "Lỗi", QString("Mã nhân viên '%1' đã tồn tại!").arg(nv.MANV));
        return false;
    }
    dsnv.nodes[dsnv.n++] = nv;
    QMessageBox::information(parent, "Thông báo", QString("Đã thêm nhân viên '%1' thành công!").arg(nv.MANV));
    return true;
}

// Hàm xóa hóa đơn của nhân viên
void XoaToanBoHoaDon(NhanVien &nv) {
    PTRHD p = nv.FirstHD;
    while (p) {
        PTRCTHD q = p->hd.FirstCTHD;
        while (q) {
            PTRCTHD tmp = q;
            q = q->next;
            delete tmp;
        }
        PTRHD tmpHD = p;
        p = p->next;
        delete tmpHD;
    }
    nv.FirstHD = nullptr;
}

// Sắp xếp nhân viên theo mã
void SortNhanVienTheoMa(List_NV &dsnv) {
    for (int i = 0; i < dsnv.n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < dsnv.n; ++j) {
            if (strcmp(dsnv.nodes[j].MANV, dsnv.nodes[minIndex].MANV) < 0) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(dsnv.nodes[i], dsnv.nodes[minIndex]);
        }
    }
}

// Sắp xếp nhân viên theo tên
void SortNhanVienTheoTen(List_NV &dsnv) {
    for (int i = 1; i < dsnv.n; ++i) {
        NhanVien key = dsnv.nodes[i];
        int j = i - 1;

        while (j >= 0) {
            int cmpTen = strcmp(key.TEN, dsnv.nodes[j].TEN);
            int cmpHo = strcmp(key.HO, dsnv.nodes[j].HO);
            if (cmpTen < 0 || (cmpTen == 0 && cmpHo < 0)) {
                dsnv.nodes[j + 1] = dsnv.nodes[j];
                --j;
            } else {
                break;
            }
        }

        dsnv.nodes[j + 1] = key;
    }
}

// Tìm hóa đơn theo số hóa đơn
PTRHD TimHoaDonTheoSoHD(const List_NV &dsnv, const char *soHD) {
    for (int i = 0; i < dsnv.n; ++i) {
        PTRHD pHD = dsnv.nodes[i].FirstHD;
        while (pHD != nullptr) {
            if (strcmp(pHD->hd.SOHD, soHD) == 0) {
                return pHD;
            }
            pHD = pHD->next;
        }
    }
    return nullptr;
}

// Kiểm tra vật tư có trong hóa đơn nào không
bool KiemTraVatTuTrongHoaDon(List_NV &dsnv, const char* maVT) {
    for (int i = 0; i < dsnv.n; ++i) {
        PTRHD pHD = dsnv.nodes[i].FirstHD;
        while (pHD) {
            PTRCTHD pCT = pHD->hd.FirstCTHD;
            while (pCT) {
                if (strcmp(pCT->cthd.MAVT, maVT) == 0) {
                    return true; // Vật tư này có trong hóa đơn
                }
                pCT = pCT->next;
            }
            pHD = pHD->next;
        }
    }
    return false;
}

double TinhTriGiaHD(const HoaDon &hd) {
    double tong = 0;
    PTRCTHD pCT = hd.FirstCTHD;
    while (pCT) {
        double triGia = pCT->cthd.SOLUONG * pCT->cthd.DONGIA * (1 + pCT->cthd.VAT / 100.0);
        tong += triGia;
        pCT = pCT->next;
    }
    return tong;
}
