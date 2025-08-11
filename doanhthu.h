#ifndef DOANHTHU_H
#define DOANHTHU_H

#include "nhanvien.h"

struct DoanhThu {
    char MAVT[11];
    double DOANHTHU;
};

struct nodeDT {
    DoanhThu dt;
    nodeDT *next;
};

typedef nodeDT* PTRDT;

void CapNhatDoanhThu(PTRDT &head, const char *maVT, double doanhThu);
bool NgayTrongKhoang(const ThoiGian &date, const ThoiGian &from, const ThoiGian &to);
void TinhDoanhThu(List_NV &dsnv, const ThoiGian &from, const ThoiGian &to, PTRDT &dsdt);
void SapXepDoanhThu(PTRDT &head);
PTRDT LayTopN(PTRDT head, int n);

#endif // DOANHTHU_H
