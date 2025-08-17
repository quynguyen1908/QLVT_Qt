#include "vattu.h"
#include <QMessageBox>
#include <QString>
#include <cstring>

// Hàm tìm kiếm vật tư theo mã
nodeVT* TimVatTu(Tree_VT root, const char* maVT) {
    if (!root || strcmp(root->vt.MAVT, maVT) == 0)
        return root;
    if (strcmp(maVT, root->vt.MAVT) < 0)
        return TimVatTu(root->left, maVT);
    return TimVatTu(root->right, maVT);
}

// Hàm thêm vật tư vào cây nhị phân tìm kiếm
bool ThemVatTu(Tree_VT &root, VatTu vt, QWidget *parent) {
    // Kiểm tra trùng lặp mã vật tư
    if (TimVatTu(root, vt.MAVT) != nullptr) {
        QMessageBox::warning(parent, "Lỗi", QString("Mã vật tư '%1' đã tồn tại!").arg(vt.MAVT));
        return false;
    }

    // Thêm vật tư vào cây
    if (!root) {
        root = new nodeVT;
        root->vt = vt;
        root->left = nullptr;
        root->right = nullptr;
        return true;
    }

    if (strcmp(vt.MAVT, root->vt.MAVT) < 0)
        return ThemVatTu(root->left, vt, parent);
    return ThemVatTu(root->right, vt, parent);
}

// Hàm chuẩn hóa tên
QString normalizeName(const QString &raw) {
    QString s = raw.simplified().toLower();
    if (s.isEmpty()) return s;

    QStringList parts = s.split(' ', Qt::SkipEmptyParts);
    for (QString &word : parts) {
        if (!word.isEmpty()) word[0] = word[0].toUpper();
    }
    return parts.join(' ');
}

// Hàm lấy danh sách mã vật tư
QStringList getDanhSachMaVT(Tree_VT root) {
    QStringList list;
    std::function<void(Tree_VT)> inOrder = [&](Tree_VT node) {
        if (!node) return;
        inOrder(node->left);
        list << node->vt.MAVT;
        inOrder(node->right);
    };
    inOrder(root);
    return list;
}

nodeVT* XoaVatTu(nodeVT* root, const char* maVT) {
    nodeVT *parent = nullptr;
    nodeVT *curr = root;

    // 1. Tìm node cần xóa
    while (curr && strcmp(maVT, curr->vt.MAVT) != 0) {
        parent = curr;
        if (strcmp(maVT, curr->vt.MAVT) < 0)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if (!curr) return root; // Không tìm thấy, return cây gốc

    // 2. Nếu node có cả 2 con
    if (curr->left && curr->right) {
        // Tìm node nhỏ nhất bên phải
        nodeVT *succParent = curr;
        nodeVT *succ = curr->right;
        while (succ->left) {
            succParent = succ;
            succ = succ->left;
        }
        // Copy dữ liệu của succ vào curr
        curr->vt = succ->vt;

        // Đổi con trỏ curr -> succ để xử lý xóa node succ
        parent = succParent;
        curr = succ;
    }

    // 3. Lúc này curr chỉ còn 0 hoặc 1 con
    nodeVT *child = (curr->left) ? curr->left : curr->right;

    if (!parent) {
        // Xóa root
        delete curr;
        return child; // Cập nhật root mới
    }

    if (parent->left == curr) parent->left = child;
    else parent->right = child;

    delete curr;
    return root;
}
