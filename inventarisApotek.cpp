#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 100;

struct Tanggal {
    int hari, bulan, tahun;
};

struct Obat {
    string id, nama, kategori, jenis;
    int    stok;
    double harga;
    Tanggal expired;
};

Obat daftar[MAX];
int  jmlObat = 0;

Tanggal hariIni;


int selisihHari(Tanggal exp) {
    int d1 = hariIni.tahun*365 + hariIni.bulan*30 + hariIni.hari;
    int d2 = exp.tahun*365 + exp.bulan*30 + exp.hari;
    return d2 - d1;
}

string statusExp(Tanggal exp) {
    int d = selisihHari(exp);
    if (d < 0) return "Kadaluarsa";
    else if (d < 30) return "Akan Expired";
    else return "Aman";
}

string statusStok(int stok) {
    if (stok == 0) return "Habis";
    if (stok < 5) return "Menipis";
    return "Tersedia";
}

// tampilan

void garis() {
    for (int i = 0; i < 110; i++) cout << '-';
    cout << '\n';
}

void header(string judul) {
    cout << "\n " << judul << " \n";
}

void tampilTabel(int idx[], int jml) {
    if (jml == 0) { cout << "tidak ada data.\n"; return; }

    garis();
    cout << left
         << setw(10) << "ID" << setw(22) << "Nama" << setw(16) << "Kategori" << setw(10) << "Jenis" << setw(7) << "Stok" << setw(12) << "Harga" << setw(14) << "Expired"  << setw(13) << "Stok Sts" << "Exp Sts" << '\n';
    garis();

    for (int i = 0; i < jml; i++) {
        Obat& o = daftar[idx[i]];
        cout << left
             << setw(10) << o.id
             << setw(22) << o.nama
             << setw(16) << o.kategori
             << setw(10) << o.jenis
             << setw(7) << o.stok
             << setw(12) << fixed << setprecision(0) << o.harga;

        cout << setfill('0')
             << setw(2) << o.expired.hari << "/"
             << setw(2) << o.expired.bulan << "/"
             << setw(4) << o.expired.tahun
             << setfill(' ');
        cout << "  " << setw(13) << statusStok(o.stok)
             << statusExp(o.expired) << '\n';
    }
    garis();
}

void tampilSemua() {
    int idx[MAX];
    for (int i = 0; i < jmlObat; i++) idx[i] = i;
    tampilTabel(idx, jmlObat);
}


int cariIdx(string id) {
    for (int i = 0; i < jmlObat; i++)
        if (daftar[i].id == id) return i;
    return -1;
}

// input

void inputData() {
    header("input data obat");
    if (jmlObat >= MAX) { cout << "data penuh.\n"; return; }

    Obat o;

    cin.ignore();
    cout << "  nama       : "; getline(cin, o.nama);
    cout << "  kategori   : "; getline(cin, o.kategori);
    cout << "  jenis      : "; getline(cin, o.jenis);

    while (1) {
        cout << "  harga      : "; cin >> o.harga;
        if (o.harga >= 0) break;
        cout << "  harga tidak boleh negatif.\n";
    }

    cout << "  expired (dd mm yyyy): ";
    cin >> o.expired.hari >> o.expired.bulan >> o.expired.tahun;

    daftar[jmlObat++] = o;
    cout << "  data ditambahkan.\n";
}

// lihat

void lihatData() {
    header("lihat data obat");
    tampilSemua();
}

//sorting

double nilaiKolom(int i, int kolom) {
    if (kolom == 4) return daftar[i].stok;
    if (kolom == 5) return daftar[i].harga;
    if (kolom == 6) return daftar[i].expired.tahun*10000
                         + daftar[i].expired.bulan*100
                         + daftar[i].expired.hari;
    return 0;
}

string strKolom(int i, int kolom) {
    if (kolom == 1) return daftar[i].id;
    if (kolom == 2) return daftar[i].nama;
    if (kolom == 3) return daftar[i].kategori;
    return "";
}

void sortingData() {
    header("sorting data");
    cout << "  urutkan berdasarkan:\n"
         << "  1. id\n  2. nama\n  3. kategori\n"
         << "  4. stok\n  5. harga\n  6. expired\n";
    int kolom; cout << "  pilih: "; cin >> kolom;

    int urutan; cout << "  1. ascending\n  2. descending\n  pilih: "; cin >> urutan;
    bool asc = (urutan == 1);

    for (int i = 0; i < jmlObat - 1; i++) {
        for (int j = 0; j < jmlObat - i - 1; j++) {
            bool tukar = false;

            if (kolom <= 3) {
                bool lebih = strKolom(j, kolom) > strKolom(j+1, kolom);
                tukar = asc ? lebih : !lebih;
            } else {
                bool lebih = nilaiKolom(j, kolom) > nilaiKolom(j+1, kolom);
                tukar = asc ? lebih : !lebih; }

            if (tukar) {
                Obat tmp = daftar[j];
                daftar[j] = daftar[j+1];
                daftar[j+1] = tmp;}
        }
    }

    cout << "  data diurutkan.\n";
    tampilSemua();
}

// search

void searchData() {
    header("cari data obat");
    cout << "  cari berdasarkan:\n  1. nama\n  2. kategori\n";
    int pilih; cout << "  pilih: "; cin >> pilih;

    string kata; cin.ignore();
    cout << "  kata kunci: "; getline(cin, kata);

    int idx[MAX], jml = 0;

    for (int i = 0; i < jmlObat; i++) {
        string src = (pilih == 1) ? daftar[i].nama : daftar[i].kategori;
        if (src.find(kata) != string::npos) idx[jml++] = i;
    }

    cout << "  ditemukan " << jml << " obat.\n";
    tampilTabel(idx, jml);
}

//hapuss

void deleteData() {
    header("hapus data obat");

    string id; cout << "  id obat: "; cin >> id;

    int idx = cariIdx(id);
    if (idx == -1) { cout << "  obat tidak ditemukan.\n"; return; }

    cout << "  obat: " << daftar[idx].nama << "\n";
    char conf; cout << "  yakin hapus? (y/n): "; cin >> conf;

    if (conf == 'y' || conf == 'Y') {
        for (int i = idx; i < jmlObat - 1; i++)
            daftar[i] = daftar[i+1];
        jmlObat--;
        cout << "  data dihapus.\n";
    } else {
        cout << "  batal.\n";
    }
}

// edit

void editData() {
    header("edit data obat");

    string id; cout << "  id obat: "; cin >> id;

    int idx = cariIdx(id);
    if (idx == -1) { cout << "  obat tidak ditemukan.\n"; return; }

    Obat& o = daftar[idx];
    cout << "  (enter untuk lewati)\n\n";

    string tmp;
    cin.ignore();

    cout << "  nama [" << o.nama << "]: ";
    getline(cin, tmp); if (tmp != "") o.nama = tmp;

    cout << "  kategori [" << o.kategori << "]: ";
    getline(cin, tmp); if (tmp != "") o.kategori = tmp;

    cout << "  jenis [" << o.jenis << "]: ";
    getline(cin, tmp); if (tmp != "") o.jenis = tmp;

    int  stokBaru  = -1;
    double hargaBaru = -1;

    cout << "  stok [" << o.stok << "] (-1 skip): "; cin >> stokBaru;
    if (stokBaru >= 0) o.stok = stokBaru;

    cout << "  harga [" << fixed << setprecision(0) << o.harga << "] (-1 skip): "; cin >> hargaBaru;
    if (hargaBaru >= 0) o.harga = hargaBaru;

    cout << "  expired [" << o.expired.hari << "/" << o.expired.bulan << "/" << o.expired.tahun
         << "] (dd mm yyyy, 0 0 0 skip): ";
    int h, b, y; cin >> h >> b >> y;
    if (h != 0) { o.expired.hari = h; o.expired.bulan = b; o.expired.tahun = y; }

    cout << "  data diperbarui.\n";
}

bool login() {
    cout << "\n sistem manajemen inventori apotek \n" << endl;

    string user, pass;
    for (int i = 1; i <= 3; i++) {
        cout << "  username: "; cin >> user;
        cout << "  password: "; cin >> pass;

        if (user == "admin" && pass == "apotek123") {
            cout << "  login berhasil. selamat datang, " << user << ".\n";
            return true;
        }
        cout << "  username atau password salah. (" << i << "/3)\n\n";
    }

    cout << "  terlalu banyak percobaan. akses ditolak.\n";
    return false;
}


int main() {
    if (!login()) return 0;

    cout << "masukkan tanggal hari ini (dd mm yyyy): ";
    cin >> hariIni.hari >> hariIni.bulan >> hariIni.tahun;

    int pilih;
    do {
        cout << "\nmenu \n"
             << "1. input data\n"
             << "2. lihat data\n"
             << "3. sorting\n"
             << "4. cari data\n"
             << "5. hapus data\n"
             << "6. edit data\n"
             << "7. exit\n";
        cout << "pilih: "; cin >> pilih;

        switch (pilih) {
            case 1: inputData(); break;
            case 2: lihatData(); break;
            case 3: sortingData(); break;
            case 4: searchData(); break;
            case 5: deleteData();break;
            case 6: editData(); break;
            case 7: cout << "sampai jumpa.\n"; break;
            default: cout << "pilihan tidak valid.\n";
        }
    } while (pilih != 7);

    return 0;
}
