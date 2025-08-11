#include "doanhthu.h"
#include <cstring>

void CapNhatDoanhThu(PTRDT &head, const char *maVT, double doanhThu) {
    PTRDT p = head;
    while (p) {
        if (strcmp(p->dt.MAVT, maVT) == 0) {
            p->dt.DOANHTHU += doanhThu;
            return;
        }
        p = p->next;
    }

    // Chưa có -> thêm mới
    PTRDT newNode = new nodeDT;
    strncpy(newNode->dt.MAVT, maVT, sizeof(newNode->dt.MAVT));
    newNode->dt.MAVT[sizeof(newNode->dt.MAVT) - 1] = '\0';
    newNode->dt.DOANHTHU = doanhThu;
    newNode->next = head;
    head = newNode;
}

bool NgayTrongKhoang(const ThoiGian &date, const ThoiGian &from, const ThoiGian &to) {
    // So sánh ngày bắt đầu
    if (date.NAM < from.NAM) return false;
    if (date.NAM == from.NAM && date.THANG < from.THANG) return false;
    if (date.NAM == from.NAM && date.THANG == from.THANG && date.NGAY < from.NGAY) return false;

    // So sánh ngày kết thúc
    if (date.NAM > to.NAM) return false;
    if (date.NAM == to.NAM && date.THANG > to.THANG) return false;
    if (date.NAM == to.NAM && date.THANG == to.THANG && date.NGAY > to.NGAY) return false;

    return true;
}

void TinhDoanhThu(List_NV &dsnv, const ThoiGian &from, const ThoiGian &to, PTRDT &dsdt) {
    dsdt = nullptr;

    for (int i = 0; i < dsnv.n; i++) {
        PTRHD pHD = dsnv.nodes[i].FirstHD;
        while (pHD) {
            // Chỉ tính hóa đơn xuất
            if (pHD->hd.LOAI == 'X' && NgayTrongKhoang(pHD->hd.NGAYLAP, from, to)) {
                PTRCTHD pCT = pHD->hd.FirstCTHD;
                while (pCT) {
                    double doanhThu = pCT->cthd.SOLUONG * pCT->cthd.DONGIA * (1 + pCT->cthd.VAT/100.0);
                    CapNhatDoanhThu(dsdt, pCT->cthd.MAVT, doanhThu);
                    pCT = pCT->next;
                }
            }
            pHD = pHD->next;
        }
    }
}

void SapXepDoanhThu(PTRDT &head) {
    for (PTRDT p = head; p && p->next; p = p->next) {
        PTRDT maxNode = p;
        for (PTRDT q = p->next; q; q = q->next) {
            if (q->dt.DOANHTHU > maxNode->dt.DOANHTHU) {
                maxNode = q;
            }
        }
        if (maxNode != p) {
            DoanhThu temp = p->dt;
            p->dt = maxNode->dt;
            maxNode->dt = temp;
        }
    }
}

PTRDT LayTopN(PTRDT head, int n) {
    if (n <= 0 || !head) return nullptr;

    PTRDT newHead = head;
    PTRDT p = head;
    int count = 1;

    while (p->next != nullptr && count < n) {
        p = p->next;
        count++;
    }

    if (p->next) {
        PTRDT temp = p->next;
        p->next = nullptr;
        while (temp) {
            PTRDT del = temp;
            temp = temp->next;
            delete del;
        }
    }

    return newHead;
}
