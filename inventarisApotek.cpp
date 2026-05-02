#include <iostream>
#include <fstream>
#include <stdlib.h> // Ditambahkan untuk system("cls")
#include <iomanip> // ditambahkan untuk setw
using namespace std;

//admin
struct admin
{
    string username, password;
};

//username dan password
admin akun[2] = {
    {"Rafif", "079"},
    {"Vania", "071"}
};

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
    else if (stok < 10) return "Menipis";
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
    system("cls");
    int input;
    
    cout << "\n=== INPUT DATA ===\n";

    cout << "Ingin input berapa data? : ";
    //error handling jumlah data (input harus angka)
    while (!(cin >> input)) {
        cout << "Input harus berupa angka : ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
        
    for (int i = 0; i < input; i++)
    {
        Obat obatBaru;

        cout << "\nData ke-" << i + 1 << endl;

        //error handling ID (ID harus berbeda)
        cout << "ID obat       : ";
        while (!(cin >> obatBaru.id) || idSudahAda(obatBaru.id))
        {
            cout << "Error: ID sudah ada.\n";
            cout << "ID obat       : ";
            cin.clear();
            cin.ignore(1000,'\n');
        }

        cin.ignore();

        cout << "Nama obat     : ";
        getline(cin, obatBaru.nama);

        cout << "Jenis obat    : ";
        getline(cin, obatBaru.jenis);

        // Error handling stok (input harus angka)
        cout << "Stok          : ";
        while (!(cin >> obatBaru.stok) || obatBaru.stok < 0) {
            cout << "Input tidak valid (harus angka & tidak negatif).\n";
            cout << "Stok          : ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        // Error handling harga (input harus angka)
        cout << "Harga         : ";
        while (!(cin >> obatBaru.harga) || obatBaru.harga < 0) {
            cout << "Input tidak valid (harus angka & tidak negatif).\n";
            cout << "Harga         : ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        // Error handling expired (input harus angka)
        cout << "Expired YYYYMMDD : ";
        while (!(cin >> obatBaru.expired)) {
            cout << "Input tidak valid (harus format YYYYMMDD).\n";
            cout << "Expired YYYYMMDD : ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        tambahNode(obatBaru);
    }

    cout << "Data berhasil ditambahkan.\n";
    system("pause");
}

//menampilkan header tabel
void headertabel()
{
    
    cout << setfill('-') << setw(115) << " " << endl;
    cout << setfill(' ');
    cout << left << setw(5) << "NO" << " | "
        << setw(5) << "ID Obat" << " | "
        << setw(18) << "Nama Obat" << " | "
        << setw(12) << "Jenis Obat" << " | "
        << setw(8) << "Stok" << " | "
        << setw(8) << "Harga" << " | "
        << setw(10) << "Expired" << " | "
        << setw(12) << "Status Stok" << " | "
        << setw(12) << "Status Expaired" << "| ";
    cout << setfill('-') << setw(115) << " " << endl;
    
}

//lihat satu data
void tampilSatu(Node* bantu, int nomor) {
    
    cout << setfill(' ');
    cout << left << setw(5) << nomor << " | " 
        << setw(7) << bantu->data.id << " | "
        << setw(18) << bantu->data.nama << " | "
        << setw(12) <<bantu->data.jenis << " | "
        << setw(8) << bantu->data.stok << " | "
        << setw(8) << bantu->data.harga << " | "
        << setw(10) << bantu->data.expired << " | "
        << setw(12) << statusStok(bantu->data.stok) << " | " 
        << setw(12) << statusExpired(bantu->data.expired) << " | " << endl;
}

//lihat data
void lihatData() {
    system("cls");
    cout << "\n=== LIHAT DATA ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        system("pause");
        return;
    }

    headertabel();

    Node* bantu = head;
    int nomor = 1;

    do {
        
        tampilSatu(bantu, nomor);

        bantu = bantu->next;
        nomor++;
    } while (bantu != head);
    cout << setfill('-') << setw(115) << " " << endl;
    system("pause");
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
    system("cls");
    int pilih;

    cout << "\n=== SORTING STOK ===\n";
    cout << "1. Ascending\n";
    cout << "2. Descending\n";
    cout << "Pilih: ";
    
    if(!(cin >> pilih)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input tidak valid.\n";
        system("pause");
        return;
    }

    if (pilih == 1) sortingStok(true);
    else if (pilih == 2) sortingStok(false);
    else cout << "Pilihan tidak valid.\n";
    system("pause");
}

//search jenis
void searchJenis() {
    system("cls");
    cout << "\n=== SEARCH JENIS ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        system("pause");
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
            if (!ditemukan)
            {
                cout << "\nData ditemukan!" << endl;
                headertabel();

            }
            tampilSatu(bantu, nomor);
            ditemukan = true;
        }

        bantu = bantu->next;
        nomor++;
    } while (bantu != head);

    if (!ditemukan) {
        cout << "Data tidak ditemukan.\n";
    }
    system("pause");
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
    system("cls");
    cout << "\n=== DELETE DATA ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        system("pause");
        return;
    }

    string id;
    char yakin;

    cout << "Masukkan ID obat: ";
    cin >> id;

    Node* hapus = cariID(id);

    if (hapus == NULL) {
        cout << "Data tidak ditemukan.\n";
        system("pause");
        return;
    }

    cout << "\nData yang akan dihapus:\n";
    headertabel();
    tampilSatu(hapus, 1);
    cout << setfill('-') << setw(130) << " " << endl;

    cout << "Yakin hapus? (y/t): ";
    cin >> yakin;

    if (yakin != 'y' && yakin != 'Y') {
        cout << "Delete dibatalkan.\n";
        system("pause");
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
    system("pause");
}

//edit data
void editData() {
    system("cls");
    cout << "\n=== EDIT DATA ===\n";

    if (head == NULL) {
        cout << "Data masih kosong.\n";
        system("pause");
        return;
    }

    string id;
    int pilih;

    cout << "Masukkan ID obat: ";
    cin >> id;

    Node* edit = cariID(id);

    if (edit == NULL) {
        cout << "Data tidak ditemukan.\n";
        system("pause");
        return;
    }

    cout << "\nData lama:\n";
    headertabel();
    tampilSatu(edit, 1);
    cout << setfill('-') << setw(115) << " " << endl;

    cout << "\n//menu edit\n";
    cout << "1. Edit nama\n";
    cout << "2. Edit jenis\n";
    cout << "3. Edit stok\n";
    cout << "4. Edit harga\n";
    cout << "5. Edit expired\n";
    cout << "6. Edit semua\n";
    cout << "7. Batal Edit\n";
    cout << "Pilih: ";
    
    if(!(cin >> pilih)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Pilihan tidak valid.\n";
        system("pause");
        return;
    }

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
        while (!(cin >> edit->data.stok) || edit->data.stok < 0) {
            cout << "Input tidak valid.\nStok baru: ";
            cin.clear(); cin.ignore(1000, '\n');
        }
    } 
    else if (pilih == 4) {
        cout << "Harga baru: ";
        while (!(cin >> edit->data.harga) || edit->data.harga < 0) {
            cout << "Input tidak valid.\nHarga baru: ";
            cin.clear(); cin.ignore(1000, '\n');
        }
    } 
    else if (pilih == 5) {
        cout << "Expired baru YYYYMMDD: ";
        while (!(cin >> edit->data.expired)) {
            cout << "Input tidak valid.\nExpired baru: ";
            cin.clear(); cin.ignore(1000, '\n');
        }
    } 
    else if (pilih == 6) {
        cout << "Nama baru: ";
        getline(cin, edit->data.nama);

        cout << "Jenis baru: ";
        getline(cin, edit->data.jenis);

        cout << "Stok baru: ";
        while (!(cin >> edit->data.stok) || edit->data.stok < 0) {
            cout << "Salah input.\nStok baru: ";
            cin.clear(); cin.ignore(1000, '\n');
        }

        cout << "Harga baru: ";
        while (!(cin >> edit->data.harga) || edit->data.harga < 0) {
            cout << "Salah input.\nHarga baru: ";
            cin.clear(); cin.ignore(1000, '\n');
        }

        cout << "Expired baru YYYYMMDD: ";
        while (!(cin >> edit->data.expired)) {
            cout << "Salah input.\nExpired baru: ";
            cin.clear(); cin.ignore(1000, '\n');
        }
    }
    else if (pilih == 7)
    {
        cout << "Edit dibatalkan\n";
        system("pause");
        return;
    }
     
    else {
        cout << "Pilihan tidak valid.\n";
        system("pause");
        return;
    }

    cout << "Data berhasil diedit.\n";
    system("pause");
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
        if (obat.id == "") break; // Error handling file kosong
        getline(file, obat.nama);
        getline(file, obat.jenis);
        getline(file, stokText);
        getline(file, hargaText);
        getline(file, expiredText);

        // Konversi string ke int
        obat.stok = atoi(stokText.c_str());
        obat.harga = atoi(hargaText.c_str());
        obat.expired = atoi(expiredText.c_str());

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

//Login
void login()
{
    bool login = false;
    string username, password;
    int kesempatan = 3;

    system("cls");
    cout << "\n=== LOGIN ADMIN ===\n";

    do
    {
        cout << "Username : ";
        getline(cin, username);
        cout << "Password : ";
        getline(cin, password);

        for (int i = 0; i < 2; i++)
        {
            if (username == akun[i].username && password == akun[i].password)
            {
                login = true;
                break;
            }
        }
        if (!login)
        {
            kesempatan--;
            cout << "Username atau Password salah, Kesempatan tersisa " << kesempatan << endl;
        }
    } while ((!login) && (kesempatan > 0));
    
    if (!login)
    {
        cout << "Kesempatan login habis!\n";
        system("pause");
        exit(0);
    }
    cout << "\nLogin berhasil, selamat datanh " << username << endl;
    system("pause");
}

//menu
int main() {
    system("cls");
    int pilih;

    //login menggunakan akun yang disediakan
    login();

    system("cls");
    cout << "\n=== TANGGAL ===\n";
    cout << "Masukkan tanggal hari ini YYYYMMDD: ";
    while (!(cin >> tanggalSekarang)) {
        cout << "Input harus angka YYYYMMDD: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    bacaFile();

    do {
        system("cls");
        cout << "\n=== INVENTARIS APOTEK ===\n";
        cout << "1. Input Data\n";
        cout << "2. Lihat Data\n";
        cout << "3. Sorting\n";
        cout << "4. Search\n";
        cout << "5. Delete\n";
        cout << "6. Edit\n";
        cout << "7. Exit\n";
        cout << "Pilih menu: ";
        
        // Error handling input menu
        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(1000, '\n');
            pilih = 0; // Memaksa masuk ke pilihan tidak valid
        }

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
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            system("pause");
        }

    } while (pilih != 7);

    return 0;
}