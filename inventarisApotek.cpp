#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//data obat
struct Obat {
    string id;
    string nama;
    string jenis;
    int stok;
    int harga;
    int expired;
};

//node
struct Node {
    Obat data;
    Node* next;
    Node* prev;
};

Node* head = NULL;
int tanggalSekarang;

//status stok
string statusStok(int stok) {
    if (stok == 0) return "Habis";
    else if (stok < 5) return "Menipis";
    else return "Tersedia";
}

//ubah tanggal
int ubahHari(int tanggal) {
    int tahun = tanggal / 10000;
    int bulan = (tanggal / 100) % 100;
    int hari = tanggal % 100;

    return (tahun * 365) + (bulan * 30) + hari;
}

//status expired
string statusExpired(int expired) {
    int selisih = ubahHari(expired) - ubahHari(tanggalSekarang);

    if (selisih < 0) return "Kadaluarsa";
    else if (selisih <= 30) return "Akan expired";
    else return "Aman";
}

//cek id
bool idSudahAda(string id) {
    if (head == NULL) return false;

    Node* bantu = head;

    do {
        if (bantu->data.id == id) {
            return true;
        }

        bantu = bantu->next;
    } while (bantu != head);

    return false;
}

//tambah node
void tambahNode(Obat obatBaru) {
    Node* baru = new Node;
    baru->data = obatBaru;

    //linked list kosong
    if (head == NULL) {
        head = baru;
        head->next = head;
        head->prev = head;
    } 
    else {
        Node* tail = head->prev;

        tail->next = baru;
        baru->prev = tail;
        baru->next = head;
        head->prev = baru;
    }
}

//input data
void inputData() {
    Obat obatBaru;

    cout << "\n=== INPUT DATA ===\n";

    cout << "ID obat       : ";
    cin >> obatBaru.id;

    if (idSudahAda(obatBaru.id)) {
        cout << "ID sudah ada.\n";
        return;
    }

    cin.ignore();

    cout << "Nama obat     : ";
    getline(cin, obatBaru.nama);

    cout << "Jenis obat    : ";
    getline(cin, obatBaru.jenis);

    cout << "Stok          : ";
    cin >> obatBaru.stok;

    while (obatBaru.stok < 0) {
        cout << "Stok tidak boleh negatif.\n";
        cout << "Stok          : ";
        cin >> obatBaru.stok;
    }

    cout << "Harga         : ";
    cin >> obatBaru.harga;

    while (obatBaru.harga < 0) {
        cout << "Harga tidak boleh negatif.\n";
        cout << "Harga         : ";
        cin >> obatBaru.harga;
    }

    cout << "Expired YYYYMMDD : ";
    cin >> obatBaru.expired;

    tambahNode(obatBaru);

    cout << "Data berhasil ditambahkan.\n";
}

//lihat satu data
void tampilSatu(Node* bantu) {
    cout << "ID obat        : " << bantu->data.id << endl;
    cout << "Nama obat      : " << bantu->data.nama << endl;
    cout << "Jenis obat     : " << bantu->data.jenis << endl;
    cout << "Stok           : " << bantu->data.stok << endl;
    cout << "Harga          : " << bantu->data.harga << endl;
    cout << "Expired        : " << bantu->data.expired << endl;
    cout << "Status stok    : " << statusStok(bantu->data.stok) << endl;
    cout << "Status expired : " << statusExpired(bantu->data.expired) << endl;
}

//lihat data
void lihatData() {
    cout << "\n=== LIHAT DATA ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        return;
    }

    Node* bantu = head;
    int nomor = 1;

    do {
        cout << "\nData ke-" << nomor << endl;
        tampilSatu(bantu);

        bantu = bantu->next;
        nomor++;
    } while (bantu != head);
}

//sorting stok
void sortingStok(bool ascending) {
    if (head == NULL || head->next == head) {
        cout << "Data belum cukup untuk sorting.\n";
        return;
    }

    bool tukar;
    Node* bantu;

    do {
        tukar = false;
        bantu = head;

        do {
            Node* setelah = bantu->next;

            if (setelah != head) {
                bool kondisi;

                if (ascending) {
                    kondisi = bantu->data.stok > setelah->data.stok;
                } 
                else {
                    kondisi = bantu->data.stok < setelah->data.stok;
                }

                if (kondisi) {
                    Obat temp = bantu->data;
                    bantu->data = setelah->data;
                    setelah->data = temp;
                    tukar = true;
                }
            }

            bantu = bantu->next;
        } while (bantu->next != head);

    } while (tukar);

    cout << "Data berhasil diurutkan.\n";
}

//menu sorting
void menuSorting() {
    int pilih;

    cout << "\n=== SORTING STOK ===\n";
    cout << "1. Ascending\n";
    cout << "2. Descending\n";
    cout << "Pilih: ";
    cin >> pilih;

    if (pilih == 1) sortingStok(true);
    else if (pilih == 2) sortingStok(false);
    else cout << "Pilihan tidak valid.\n";
}

//search jenis
void searchJenis() {
    cout << "\n=== SEARCH JENIS ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        return;
    }

    string cari;
    bool ditemukan = false;

    cin.ignore();

    cout << "Masukkan jenis obat: ";
    getline(cin, cari);

    Node* bantu = head;
    int nomor = 1;

    do {
        if (bantu->data.jenis == cari) {
            cout << "\nData ditemukan ke-" << nomor << endl;
            tampilSatu(bantu);
            ditemukan = true;
        }

        bantu = bantu->next;
        nomor++;
    } while (bantu != head);

    if (!ditemukan) {
        cout << "Data tidak ditemukan.\n";
    }
}

//cari id
Node* cariID(string id) {
    if (head == NULL) return NULL;

    Node* bantu = head;

    do {
        if (bantu->data.id == id) {
            return bantu;
        }

        bantu = bantu->next;
    } while (bantu != head);

    return NULL;
}

//delete data
void deleteData() {
    cout << "\n=== DELETE DATA ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        return;
    }

    string id;
    char yakin;

    cout << "Masukkan ID obat: ";
    cin >> id;

    Node* hapus = cariID(id);

    if (hapus == NULL) {
        cout << "Data tidak ditemukan.\n";
        return;
    }

    cout << "\nData yang akan dihapus:\n";
    tampilSatu(hapus);

    cout << "Yakin hapus? (y/t): ";
    cin >> yakin;

    if (yakin != 'y' && yakin != 'Y') {
        cout << "Delete dibatalkan.\n";
        return;
    }

    //hapus satu data
    if (hapus->next == hapus) {
        delete hapus;
        head = NULL;
    } 
    else {
        hapus->prev->next = hapus->next;
        hapus->next->prev = hapus->prev;

        if (hapus == head) {
            head = hapus->next;
        }

        delete hapus;
    }

    cout << "Data berhasil dihapus.\n";
}

//edit data
void editData() {
    cout << "\n=== EDIT DATA ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        return;
    }

    string id;
    int pilih;

    cout << "Masukkan ID obat: ";
    cin >> id;

    Node* edit = cariID(id);

    if (edit == NULL) {
        cout << "Data tidak ditemukan.\n";
        return;
    }

    cout << "\nData lama:\n";
    tampilSatu(edit);

    cout << "\n//menu edit\n";
    cout << "1. Edit nama\n";
    cout << "2. Edit jenis\n";
    cout << "3. Edit stok\n";
    cout << "4. Edit harga\n";
    cout << "5. Edit expired\n";
    cout << "6. Edit semua\n";
    cout << "Pilih: ";
    cin >> pilih;
    cout << "//akhir menu edit\n";

    cin.ignore();

    if (pilih == 1) {
        cout << "Nama baru: ";
        getline(cin, edit->data.nama);
    } 
    else if (pilih == 2) {
        cout << "Jenis baru: ";
        getline(cin, edit->data.jenis);
    } 
    else if (pilih == 3) {
        cout << "Stok baru: ";
        cin >> edit->data.stok;

        while (edit->data.stok < 0) {
            cout << "Stok tidak boleh negatif.\n";
            cout << "Stok baru: ";
            cin >> edit->data.stok;
        }
    } 
    else if (pilih == 4) {
        cout << "Harga baru: ";
        cin >> edit->data.harga;

        while (edit->data.harga < 0) {
            cout << "Harga tidak boleh negatif.\n";
            cout << "Harga baru: ";
            cin >> edit->data.harga;
        }
    } 
    else if (pilih == 5) {
        cout << "Expired baru YYYYMMDD: ";
        cin >> edit->data.expired;
    } 
    else if (pilih == 6) {
        cout << "Nama baru: ";
        getline(cin, edit->data.nama);

        cout << "Jenis baru: ";
        getline(cin, edit->data.jenis);

        cout << "Stok baru: ";
        cin >> edit->data.stok;

        while (edit->data.stok < 0) {
            cout << "Stok tidak boleh negatif.\n";
            cout << "Stok baru: ";
            cin >> edit->data.stok;
        }

        cout << "Harga baru: ";
        cin >> edit->data.harga;

        while (edit->data.harga < 0) {
            cout << "Harga tidak boleh negatif.\n";
            cout << "Harga baru: ";
            cin >> edit->data.harga;
        }

        cout << "Expired baru YYYYMMDD: ";
        cin >> edit->data.expired;
    } 
    else {
        cout << "Pilihan tidak valid.\n";
        return;
    }

    cout << "Data berhasil diedit.\n";
}

//simpan file
void simpanFile() {
    ofstream file("inventaris_apotek.txt");

    if (head == NULL) {
        file.close();
        return;
    }

    Node* bantu = head;

    do {
        file << bantu->data.id << endl;
        file << bantu->data.nama << endl;
        file << bantu->data.jenis << endl;
        file << bantu->data.stok << endl;
        file << bantu->data.harga << endl;
        file << bantu->data.expired << endl;

        bantu = bantu->next;
    } while (bantu != head);

    file.close();
}

//baca file
void bacaFile() {
    ifstream file("inventaris_apotek.txt");

    if (!file) {
        return;
    }

    Obat obat;
    string stokText, hargaText, expiredText;

    while (getline(file, obat.id)) {
        getline(file, obat.nama);
        getline(file, obat.jenis);
        getline(file, stokText);
        getline(file, hargaText);
        getline(file, expiredText);

        obat.stok = stoi(stokText);
        obat.harga = stoi(hargaText);
        obat.expired = stoi(expiredText);

        tambahNode(obat);
    }

    file.close();
}

//hapus memori
void hapusMemori() {
    if (head == NULL) return;

    Node* bantu = head->next;

    while (bantu != head) {
        Node* hapus = bantu;
        bantu = bantu->next;
        delete hapus;
    }

    delete head;
    head = NULL;
}

//menu
int main() {
    int pilih;

    cout << "Masukkan tanggal hari ini YYYYMMDD: ";
    cin >> tanggalSekarang;

    bacaFile();

    do {
        cout << "\n=== INVENTARIS APOTEK ===\n";
        cout << "1. Input Data\n";
        cout << "2. Lihat Data\n";
        cout << "3. Sorting\n";
        cout << "4. Search\n";
        cout << "5. Delete\n";
        cout << "6. Edit\n";
        cout << "7. Exit\n";
        cout << "Pilih menu: ";
        cin >> pilih;

        if (pilih == 1) {
            inputData();
            simpanFile();
        } 
        else if (pilih == 2) {
            lihatData();
        } 
        else if (pilih == 3) {
            menuSorting();
            simpanFile();
        } 
        else if (pilih == 4) {
            searchJenis();
        } 
        else if (pilih == 5) {
            deleteData();
            simpanFile();
        } 
        else if (pilih == 6) {
            editData();
            simpanFile();
        } 
        else if (pilih == 7) {
            simpanFile();
            hapusMemori();
            cout << "Program selesai.\n";
        } 
        else {
            cout << "Pilihan tidak valid.\n";
        }

    } while (pilih != 7);

    return 0;
}
//akhir menu