#ifndef FILEIO_H
#define FILEIO_H

#include <QString>
#include "vattu.h"
#include "nhanvien.h"

void SaveVatTuToFile(Tree_VT &root, const QString &filename = "vattu.txt");
void LoadVatTuFromFile(Tree_VT &root, const QString &filename = "vattu.txt");
void SaveNhanVienToFile(List_NV &dsnv, const QString &filename = "nhanvien.txt");
void LoadNhanVienFromFile(List_NV &dsnv, const QString &filename = "nhanvien.txt");

#endif // FILEIO_H
