#include "fileio.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

// Ghi thông tin vào file
void SaveVatTuToFile(Tree_VT &root, const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Lỗi", "Không mở được file để ghi!");
        return;
    }

    QTextStream out(&file);

    // Hàm đệ quy duyệt cây in-order và ghi từng vật tư vào file
    std::function<void(Tree_VT)> writeInOrder = [&](Tree_VT node) {
        if (!node) return;
        writeInOrder(node->left);
        out << node->vt.MAVT << "|" << node->vt.TENVT << "|" << node->vt.DVT << "|" << node->vt.TONKHO << "\n";
        writeInOrder(node->right);
    };

    writeInOrder(root);
    file.close();
}

void SaveNhanVienToFile(List_NV &dsnv, const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Lỗi", "Không mở được file để ghi!");
        return;
    }

    QTextStream out(&file);

    // Ghi số lượng nhân viên
    out << dsnv.n << "\n";

    // Duyệt từng nhân viên trong danh sách
    for (int i = 0; i < dsnv.n; ++i) {
        const NhanVien &nv = dsnv.nodes[i];

        // Đếm số hóa đơn của nhân viên
        int soHD = 0;
        PTRHD pHD = nv.FirstHD;
        while (pHD != nullptr) {
            soHD++;
            pHD = pHD->next;
        }

        // Ghi thông tin nhân viên + số hóa đơn
        out << nv.MANV << "|" << nv.HO << "|" << nv.TEN << "|" << nv.PHAI << "|" << soHD << "\n";

        // Ghi từng hóa đơn nếu có
        pHD = nv.FirstHD;
        int sttHD = 0;
        while (pHD != nullptr) {
            // Đếm số chi tiết hóa đơn
            int soCT = 0;
            PTRCTHD pCT = pHD->hd.FirstCTHD;
            while (pCT != nullptr) {
                soCT++;
                pCT = pCT->next;
            }

            // Ghi thông tin hóa đơn: số HĐ, ngày lập, loại, số chi tiết
            out << "    " << pHD->hd.SOHD << "|"
                << QString("%1/%2/%3")
                       .arg(pHD->hd.NGAYLAP.NGAY, 2, 10, QChar('0'))
                       .arg(pHD->hd.NGAYLAP.THANG, 2, 10, QChar('0'))
                       .arg(pHD->hd.NGAYLAP.NAM)
                << "|" << pHD->hd.LOAI
                << "|" << soCT << "\n";

            // Ghi từng chi tiết hóa đơn
            pCT = pHD->hd.FirstCTHD;
            while (pCT != nullptr) {
                out << "        " << pCT->cthd.MAVT << "|"
                    << pCT->cthd.SOLUONG << "|"
                    << QString::number(pCT->cthd.DONGIA, 'f', 2) << "|"
                    << pCT->cthd.VAT << "\n";
                pCT = pCT->next;
            }

            pHD = pHD->next;
        }
    }

    file.close();
}

// Đọc thông tin từ file
void LoadVatTuFromFile(Tree_VT &root, const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Lỗi", "Không mở được file để đọc!");
        return;
    }

    QTextStream in(&file);

    // Đọc từng dòng, mỗi dòng chứa một vật tư: MAVT|TENVT|DVT|TONKHO
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split("|");

        // Nếu đủ 4 trường thì tạo và thêm vào cây
        if (fields.size() >= 4) {
            VatTu vt;
            strncpy(vt.MAVT,  fields[0].toStdString().c_str(), sizeof(vt.MAVT));
            strncpy(vt.TENVT, fields[1].toStdString().c_str(), sizeof(vt.TENVT));
            strncpy(vt.DVT,   fields[2].toStdString().c_str(), sizeof(vt.DVT));
            vt.TONKHO = fields[3].toInt();

            ThemVatTu(root, vt);
        }
    }
}

void LoadNhanVienFromFile(List_NV &dsnv, const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Lỗi", "Không mở được file để đọc!");
        return;
    }

    QTextStream in(&file);
    QString line;
    dsnv.n = 0;

    // Đọc số lượng nhân viên từ dòng đầu tiên
    if (!in.atEnd()) {
        line = in.readLine().trimmed();
        dsnv.n = line.toInt();
    }

    // Lặp lại với mỗi nhân viên
    for (int i = 0; i < dsnv.n; ++i) {
        if (in.atEnd()) break;

        // Đọc dòng thông tin nhân viên
        line = in.readLine().trimmed();
        QStringList nvParts = line.split("|");
        if (nvParts.size() < 5) continue;

        NhanVien &nv = dsnv.nodes[i];
        strncpy(nv.MANV, nvParts[0].toStdString().c_str(), sizeof(nv.MANV));
        strncpy(nv.HO,   nvParts[1].toStdString().c_str(), sizeof(nv.HO));
        strncpy(nv.TEN,  nvParts[2].toStdString().c_str(), sizeof(nv.TEN));
        strncpy(nv.PHAI, nvParts[3].toStdString().c_str(), sizeof(nv.PHAI));
        int soHD = nvParts[4].toInt();
        nv.FirstHD = nullptr;

        PTRHD *pHDNext = &nv.FirstHD;

        // Đọc từng hóa đơn của nhân viên
        for (int j = 0; j < soHD && !in.atEnd(); ++j) {
            line = in.readLine().trimmed();
            QStringList hdParts = line.split("|");
            if (hdParts.size() < 4) continue;

            PTRHD newHD = new nodeHD;
            strncpy(newHD->hd.SOHD, hdParts[0].trimmed().toStdString().c_str(), sizeof(newHD->hd.SOHD));

            QStringList dateParts = hdParts[1].split("/");
            newHD->hd.NGAYLAP.NGAY  = dateParts[0].toInt();
            newHD->hd.NGAYLAP.THANG = dateParts[1].toInt();
            newHD->hd.NGAYLAP.NAM   = dateParts[2].toInt();

            newHD->hd.LOAI = hdParts[2].trimmed().toStdString()[0];
            int soCT = hdParts[3].toInt();

            newHD->hd.FirstCTHD = nullptr;
            newHD->next = nullptr;

            *pHDNext = newHD;
            pHDNext = &newHD->next;

            PTRCTHD *pCTNext = &newHD->hd.FirstCTHD;

            // Đọc từng chi tiết hóa đơn
            for (int k = 0; k < soCT && !in.atEnd(); ++k) {
                line = in.readLine().trimmed();
                QStringList ctParts = line.split("|");
                if (ctParts.size() < 4) continue;

                PTRCTHD newCT = new nodeCTHD;
                strncpy(newCT->cthd.MAVT, ctParts[0].toStdString().c_str(), sizeof(newCT->cthd.MAVT));
                newCT->cthd.SOLUONG = ctParts[1].toInt();
                newCT->cthd.DONGIA  = ctParts[2].toFloat();
                newCT->cthd.VAT     = ctParts[3].toInt();
                newCT->next = nullptr;

                *pCTNext = newCT;
                pCTNext = &newCT->next;
            }
        }
    }
}
