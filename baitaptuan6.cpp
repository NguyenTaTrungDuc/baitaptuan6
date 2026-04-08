#include <iostream>
#include <string>

using namespace std;

// KHAI BÁO CẤU TRÚC

// Thông tin của một File
struct FileInfo {
    string Name;
    int TimeOrder;   // (1, 2, 3,...) Số càng nhỏ thì file càng cũ
    double Size; // Tính bằng GB
};

// Cấu trúc Node
struct Node {
    FileInfo data;
    Node* next;
};

// Cấu trúc List
struct List {
    Node* firstNode;
};

// Hàm khởi tạo danh sách
void List_Init(List* list) {
    list->firstNode = nullptr;
}


// COPY PASTE (CHÈN THEO TRẬT TỰ THỜI GIAN)
void InsertFile(List* list, FileInfo newFile) {
    Node* newNode = new Node();
    newNode->data = newFile;
    newNode->next = nullptr;

    // TH1: Nếu danh sách rỗng, hoặc file mới cũ hơn cả file đầu tiên -> Chèn vào đầu
    if (list->firstNode == nullptr || list->firstNode->data.TimeOrder >= newFile.TimeOrder) {
        newNode->next = list->firstNode;
        list->firstNode = newNode;
        return;
    }

    // TH2: Tìm vị trí thích hợp ở giữa hoặc cuối để chèn
    Node* temp = list->firstNode;
    
    while (temp->next != nullptr && temp->next->data.TimeOrder < newFile.TimeOrder) {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}


// TÍNH TỔNG KÍCH THƯỚC CÁC FILE
double TotalSize(List list) {
    double tong = 0;
    Node* temp = list.firstNode;
    while (temp != nullptr) {
        tong += temp->data.Size;
        temp = temp->next;
    }
    return tong;
}


// SAO LƯU (LOẠI BỎ FILE NHỎ NHẤT NẾU VƯỢT QUÁ 32GB)
void RemoveSmallestFile(List* list) {
    if (list->firstNode == nullptr) return; // Không có gì để xóa

    Node* minNode = list->firstNode; // Lưu node nhỏ nhất
    Node* minPrev = nullptr;         // Lưu node đứng NGAY TRƯỚC node nhỏ nhất

    Node* temp = list->firstNode;
    Node* prev = nullptr;

    // Duyệt toàn bộ danh sách để tìm file nhỏ nhất
    while (temp != nullptr) {
        if (temp->data.Size < minNode->data.Size) {
            minNode = temp;
            minPrev = prev;
        }
        prev = temp;
        temp = temp->next;
    }

    // Tiến hành xóa minNode khỏi danh sách
    if (minPrev == nullptr) {
        // Nếu file nhỏ nhất lại chính là file đứng đầu danh sách
        list->firstNode = list->firstNode->next;
    } else {
        minPrev->next = minNode->next;
    }

    cout << "   [-] Da xoa file: " << minNode->data.Name 
         << " (" << minNode->data.Size << "GB) de giai phong dung luong." << endl;
    delete minNode;
}

void BackupToUSB(List* list) {
    double usbCapacity = 32.0;
    
    cout << "\n--- TIEN HANH BACKUP VAO USB 32GB ---" << endl;

    while (TotalSize(*list) > usbCapacity) {
        cout << "Tong dung luong hien tai la " << TotalSize(*list) 
             << "GB. Vuot muc cho phep! Dang tim file nho nhat de xoa..." << endl;
        RemoveSmallestFile(list);
    }
    
    cout << "=> HOAN TAT! Tong dung luong con lai la: " << TotalSize(*list) 
         << "GB. Du dieu kien de backup vao USB." << endl;
}

// Hàm in danh sách để kiểm tra
void PrintList(List list) {
    Node* temp = list.firstNode;
    cout << "\n--- DANH SACH FILE TRONG THU MUC ---" << endl;
    while (temp != nullptr) {
        cout << "File: " << temp->data.Name 
             << " | Thoi gian: " << temp->data.TimeOrder 
             << " | Kich thuoc: " << temp->data.Size << "GB" << endl;
        temp = temp->next;
    }
    cout << "Tong dung luong: " << TotalSize(list) << "GB\n";
}

int main() {
    List folder;
    List_Init(&folder);

    // Thử nghiệm chèn file (thời gian lộn xộn để test tự động sắp xếp)
    InsertFile(&folder, {"Tai_lieu.pdf", 1, 10.5});
    InsertFile(&folder, {"Videos.mp4", 4, 10.0});
    InsertFile(&folder, {"Am_nhac.mp3", 3, 2.5});
    InsertFile(&folder, {"Games.zip", 2, 20.0}); 
    InsertFile(&folder, {"Memes.jpg", 5, 2.0}); // Tổng lúc này là 45GB > 32GB

    // In ra xem file đã tự động xếp theo thời gian tăng dần chưa (1 -> 2 -> 3 -> 4 -> 5)
    PrintList(folder);

    // Thực hiện Backup (Xóa dần file nhỏ nhất cho đến khi < 32GB)
    BackupToUSB(&folder);

    // In lại danh sách xem còn lại những file nào
    PrintList(folder);

    return 0;
}