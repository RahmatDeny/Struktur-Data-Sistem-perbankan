#include <iostream>
using namespace std;

struct KartuKredit;
struct Transaksi;

struct Rekening
{
    string nomor;
    int saldo;
    Rekening *next;
};

struct Nasabah
{
    int nik;
    string nama;
    int nomorHp;
    string email;
    string alamat;
    string username;
    string passworduser;
    Rekening *rekeningHead;
    KartuKredit *kartuKreditHead;
    Transaksi *transaksiHead;
    Nasabah *next;
};

struct KartuKredit
{
    string jenis;
    string nama;
    string nomorKartu;
    KartuKredit *next;
};

struct Transaksi
{
    string jenis;
    int jumlah;
    Transaksi *next;
};

struct Pinjaman
{
    string username;
    int jumlah;
    Pinjaman *next;
};

const int TABLE_SIZE = 1000;
Nasabah *nasabahTable[TABLE_SIZE];
Nasabah *currentUser = nullptr;
KartuKredit *kartuKreditHead = nullptr;
Transaksi *transaksiHead = nullptr;
Pinjaman *pinjamanFront = nullptr;
Pinjaman *pinjamanRear = nullptr;

int rekeningCounter = 100000;

// Struktur untuk adjacency list
struct AdjListNode {
    int dest;
    float amount;
    AdjListNode* next;
};

struct AdjList {
    AdjListNode *head;
};

struct Graph {
    int V;
    AdjList* array;
};

Graph* transferGraph = nullptr;

// Struktur Tree untuk Nasabah
struct TreeNode {
    Nasabah *nasabah;
    TreeNode *left;
    TreeNode *right;
};

TreeNode *root = nullptr;

// Struktur untuk Stack
struct StackNode
{
    Transaksi *transaksi;
    StackNode *next;
};

StackNode *stackTop = nullptr;

void pushStack(Transaksi *transaksi)
{
    StackNode *newNode = new StackNode{transaksi, stackTop};
    stackTop = newNode;
}

Transaksi* popStack()
{
    if (!stackTop)
        return nullptr;
    StackNode *temp = stackTop;
    Transaksi *transaksi = stackTop->transaksi;
    stackTop = stackTop->next;
    delete temp;
    return transaksi;
}

Graph* createGraph(int V) {
    Graph* graph = new Graph;
    graph->V = V;
    graph->array = new AdjList[V];
    for (int i = 0; i < V; ++i) {
        graph->array[i].head = nullptr;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest, float amount) {
    AdjListNode* newNode = new AdjListNode{dest, amount, graph->array[src].head};
    graph->array[src].head = newNode;
}

int hashFunction(string key)
{
    int hash = 0;
    for (char ch : key)
    {
        hash = (hash + ch) % TABLE_SIZE;
    }
    return hash;
}

void tambahNasabah(Nasabah *nasabah) {
    int hashIndex = hashFunction(nasabah->username);
    nasabah->next = nasabahTable[hashIndex];
    nasabahTable[hashIndex] = nasabah;

    TreeNode *newNode = new TreeNode{nasabah, nullptr, nullptr};
    if (!root) {
        root = newNode;
    } else {
        TreeNode *current = root;
        TreeNode *parent = nullptr;
        while (current) {
            parent = current;
            if (nasabah->username < current->nasabah->username) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        if (nasabah->username < parent->nasabah->username) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }
}

void tampilkanTreeNasabah(TreeNode *node) {
    if (node) {
        tampilkanTreeNasabah(node->left);
        cout << "========================================\n";
        cout << "Nama: " << node->nasabah->nama << endl;
        cout << "NIK: " << node->nasabah->nik << endl;
        cout << "Nomor HP: " << node->nasabah->nomorHp << endl;
        cout << "Email: " << node->nasabah->email << endl;
        cout << "Alamat: " << node->nasabah->alamat << endl;
        cout << "Username: " << node->nasabah->username << endl;

        Rekening *rekeningTemp = node->nasabah->rekeningHead;
        while (rekeningTemp) {
            cout << "Nomor Rekening: " << rekeningTemp->nomor<< endl;
            rekeningTemp = rekeningTemp->next;
        }

        KartuKredit *kartuKreditTemp = node->nasabah->kartuKreditHead;
        while (kartuKreditTemp) {
            cout << "Jenis Kartu: " << kartuKreditTemp->jenis<< endl;
            cout << "Nomor Kartu: " << kartuKreditTemp->nomorKartu << endl;
            cout << "========================================\n";
            kartuKreditTemp = kartuKreditTemp->next;
        }

        cout << endl;
        tampilkanTreeNasabah(node->right);
    }
}



Nasabah *cariNasabah(string username)
{
    int hashIndex = hashFunction(username);
    Nasabah *temp = nasabahTable[hashIndex];
    while (temp)
    {
        if (temp->username == username)
        {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

void tambahTransaksi(string jenis, int jumlah)
{
    if (!currentUser)
        return;
    Transaksi *newTransaksi = new Transaksi{jenis, jumlah, currentUser->transaksiHead};
    currentUser->transaksiHead = newTransaksi;
}


string generateRekeningNumber(string dataDiri)
{
    int length = dataDiri.length();
    int seed = 0;
    for (int i = 0; i < length; ++i)
    {
        seed += static_cast<int>(dataDiri[i]);
    }
    string randomRekeningNumber = "1";
    for (int i = 1; i <= 9; ++i)
    {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        int randomNumber = (seed % 10);
        randomRekeningNumber += to_string(randomNumber);
    }

    return randomRekeningNumber;
}

string generateNomorKartu(string nomorRekening)
{
    string nomorKartu = nomorRekening;
    int seed = 0;
    for (char ch : nomorRekening)
    {
        seed += ch;
    }

    for (int i = 0; i < 6; ++i)
    {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        nomorKartu += to_string(seed % 10);
    }
    return nomorKartu;
}

void buatAkun()
{
    string nama, email, alamat, username, passworduser;
    int nik, nomorHp;
    cout << "========================================================\n";
    cout << "|Masukkan NIK: ";
    cin >> nik;
    cout << "|Masukkan Nama: ";
    cin >> nama;
    cout << "|Masukkan Nomor HP: ";
    cin >> nomorHp;
    cout << "|Masukkan Email: ";
    cin >> email;
    cout << "|Masukkan Alamat: ";
    cin >> alamat;
    cout << "|Masukkan Username: ";
    cin >> username;
    cout << "|Masukkan password: ";
    cin >> passworduser;
    cout << "========================================================\n";

    string jenisKartu[3] = {"Gold", "Premium", "Standard"};
    cout << "Pilih jenis kartu kredit:\n";
    cout << "1. Gold\n";
    cout << "2. Premium\n";
    cout << "3. Standard\n";
    cout << "========================================================\n";
    int pilihan;
    cout << "pilih : ";
    cin >> pilihan;
    cout << "========================================================\n";
    if (pilihan < 1 || pilihan > 3)
    {
        cout << "Pilihan tidak valid.\n";
        return;
    }
    string jenis = jenisKartu[pilihan - 1];

    string dataDiri = to_string(nik) + nama + to_string(nomorHp);
    string nomorRekening = generateRekeningNumber(dataDiri);
    Rekening *newRekening = new Rekening{nomorRekening, 0, nullptr};

    string nomorKartu = generateNomorKartu(nomorRekening);
    KartuKredit *newKartuKredit = new KartuKredit{jenis, nama, nomorKartu, nullptr};

    Nasabah *newNasabah = new Nasabah{nik, nama, nomorHp, email, alamat, username, passworduser, newRekening, newKartuKredit, nullptr}; // Menetapkan newKartuKredit ke kartuKreditHead
    tambahNasabah(newNasabah);
    cout << "Akun berhasil dibuat dengan nomor rekening: " << nomorRekening << endl;
    cout << "========================================================\n";
}

void cariDataNasabah()
{
    string nama;
    cout << "Masukkan nama nasabah: ";
    cin >> nama;

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Nasabah *temp = nasabahTable[i];
        while (temp)
        {
            if (temp->nama == nama)
            {
                cout << "========================================================\n";
                cout << "|Nasabah ditemukan: " << temp->nama << "\n";
                cout << "|NIK: " << temp->nik << "\n";
                cout << "|Nomor HP: " << temp->nomorHp << "\n";
                cout << "|Email: " << temp->email << "\n";
                cout << "|Alamat: " << temp->alamat << "\n";
                cout << "|Username: " << temp->username << "\n";
                cout << "========================================================\n";
                return;
            }
            temp = temp->next;
        }
    }
    cout << "Nasabah tidak ditemukan.\n";
}

void lihatRiwayatTransaksiNasabah()
{
    string username;
    cout << "Masukkan username nasabah: ";
    cin >> username;

    Nasabah *nasabah = cariNasabah(username);
    if (nasabah)
    {
        cout << "========================================================\n";
        cout << "Riwayat Transaksi Nasabah " << nasabah->nama << ":\n";
        if (nasabah->transaksiHead)
        {
            Transaksi *transaksi = nasabah->transaksiHead;
            while (transaksi)
            {
                cout << "========================================================\n";
                cout << "Jenis: " << transaksi->jenis << "     | Jumlah: " << transaksi->jumlah << endl;
                cout << "========================================================\n";
                transaksi = transaksi->next;
            }
        }
        else
        {
            cout << "Nasabah ini tidak memiliki riwayat transaksi.\n";
        }
    }
    else
    {
        cout << "Nasabah tidak ditemukan.\n";
    }
}

int hitungTotalUangMasuk(Transaksi *transaksi)
{
    if (transaksi == nullptr)
        return 0;

    if (transaksi->jenis == "Setor Tunai" || transaksi->jenis == "Transfer Masuk"|| transaksi->jenis == "Pinjam Dana")
        return transaksi->jumlah + hitungTotalUangMasuk(transaksi->next);

    return hitungTotalUangMasuk(transaksi->next);
}

int hitungTotalUangKeluar(Transaksi *transaksi)
{
    if (transaksi == nullptr)
        return 0;

    if (transaksi->jenis == "Tarik Tunai" || transaksi->jenis == "Transfer"|| transaksi->jenis == "Kembalikan Dana")
        return transaksi->jumlah + hitungTotalUangKeluar(transaksi->next);

    return hitungTotalUangKeluar(transaksi->next);
}

void urutkanTransaksi(Transaksi **head, bool ascending)
{
    if (!*head)
        return;

    bool swapped;
    Transaksi *ptr1;
    Transaksi *lptr = nullptr;

    do
    {
        swapped = false;
        ptr1 = *head;

        while (ptr1->next != lptr)
        {
            if (ascending ? (ptr1->jumlah > ptr1->next->jumlah) : (ptr1->jumlah < ptr1->next->jumlah))
            {
                swap(ptr1->jumlah, ptr1->next->jumlah);
                swap(ptr1->jenis, ptr1->next->jenis);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void lihatRiwayatTransaksi()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }

    Transaksi *transaksi = currentUser->transaksiHead;
    if (!transaksi)
    {
        cout << "Tidak ada riwayat transaksi.\n";
        return;
    }

    while (transaksi)
    {
        cout << "========================================================\n";
        cout << "Jenis: " << transaksi->jenis << "      | Jumlah: " << transaksi->jumlah << endl;
        cout << "========================================================\n";
        transaksi = transaksi->next;
    }

    int totalUangMasuk = hitungTotalUangMasuk(currentUser->transaksiHead);
    int totalUangKeluar = hitungTotalUangKeluar(currentUser->transaksiHead);

    cout << "Total Uang Masuk: " << totalUangMasuk << endl;
    cout << "Total Uang Keluar: " << totalUangKeluar << endl;
    cout << "Saldo Akhir: " << totalUangMasuk - totalUangKeluar << endl;
    cout << "========================================================\n";

    cout << "Apakah Anda ingin mengurutkan riwayat transaksi berdasarkan jumlah?\n";
    cout << "1. Ya\n";
    cout << "2. Tidak\n";
    int pilihan;
    cout << "pilih : ";
    cin >> pilihan;

    if (pilihan == 1)
    {
        urutkanTransaksi(&currentUser->transaksiHead, true);
        cout << "========================================================\n";
        cout << "Riwayat transaksi setelah diurutkan(dari yang terkecil):\n";
        transaksi = currentUser->transaksiHead;
        while (transaksi)
        {
            cout << "========================================================\n";
            cout << "Jenis: " << transaksi->jenis << "     | Jumlah: " << transaksi->jumlah << endl;
            cout << "========================================================\n";
            transaksi = transaksi->next;
        }
    } 
}



void detailRekening()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }
    cout << "===========================================\n";
    cout << "|Nama: " << currentUser->nama << "        \n";
    cout << "|Nomor Rekening: " << currentUser->rekeningHead->nomor << "\n";
    cout << "|Nomor HP: " << currentUser->nomorHp << " \n";
    cout << "|Email: " << currentUser->email << "\n";
    cout << "|Alamat: " << currentUser->alamat << "\n";
    Rekening *temp = currentUser->rekeningHead;
    while (temp)
    {
        cout << "|Nomor Rekening: " << temp->nomor << "\n";
        cout << "|Saldo: " << temp->saldo << "\n";
        cout << "===========================================\n";
        temp = temp->next;
    }
}

void detailKartuKredit()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }

    KartuKredit *temp = currentUser->kartuKreditHead;
    while (temp)
    {
        cout << "===========================================\n";
        cout << "|Jenis: " << temp->jenis << "\n";
        cout << "|Nama: " << temp->nama << "\n";
        cout << "|Nomor Kartu: " << temp->nomorKartu << "\n";
        cout << "===========================================\n";
        temp = temp->next;
    }
}

void konfirmasiTransaksi()
{
    Transaksi *transaksi = popStack();
    if (!transaksi)
    {
        cout << "Tidak ada transaksi yang menunggu konfirmasi.\n";
        return;
    }

    char konfirmasi;
    cout << "Jenis Transaksi: " << transaksi->jenis << ", Jumlah: " << transaksi->jumlah << endl;
    cout << "Apakah Anda ingin melanjutkan transaksi ini? (y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y')
    {
        if (transaksi->jenis == "Tarik Tunai")
        {
            if (currentUser->rekeningHead->saldo >= transaksi->jumlah)
            {
                currentUser->rekeningHead->saldo -= transaksi->jumlah;
                cout << "Tarik tunai sejumlah " << transaksi->jumlah << " berhasil. Sisa saldo: "<< currentUser->rekeningHead->saldo << endl;
            }
            else
            {
                cout << "Saldo tidak mencukupi untuk tarik tunai.\n";
            }
        }
        else if (transaksi->jenis == "Setor Tunai")
        {
            currentUser->rekeningHead->saldo += transaksi->jumlah;
            cout << "Setor tunai sejumlah " << transaksi->jumlah << " berhasil. Saldo saat ini: " << currentUser->rekeningHead->saldo << endl;
        }
        else if (transaksi->jenis == "Transfer")
        {
            if (currentUser->rekeningHead->saldo >= transaksi->jumlah)
            {
                Nasabah *nasabahTujuan = cariNasabah(transaksi->jenis);
                if (nasabahTujuan && nasabahTujuan->rekeningHead)
                {
                    currentUser->rekeningHead->saldo -= transaksi->jumlah;
                    nasabahTujuan->rekeningHead->saldo += transaksi->jumlah;
                    cout << "Transfer sejumlah " << transaksi->jumlah << " ke " << transaksi->jenis << " berhasil.\n";
                }
                else
                {
                    cout << "Nasabah tujuan tidak ditemukan.\n";
                }
            }
            else
            {
                cout << "Saldo tidak mencukupi untuk transfer.\n";
            }
        }
        delete transaksi;
    }
    else
    {
        cout << "Transaksi dibatalkan.\n";
        delete transaksi;
    }
}

void tarikTunai()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }

    int jumlah;
    cout << "Masukkan jumlah yang ingin ditarik: ";
    cin >> jumlah;

    if (currentUser->rekeningHead->saldo >= jumlah)
    {
        tambahTransaksi("Tarik Tunai", jumlah);

        Transaksi *newTransaksi = new Transaksi{"Tarik Tunai", jumlah, nullptr};
        pushStack(newTransaksi);
        konfirmasiTransaksi();

        // Tampilkan sisa saldo setelah tarik tunai
        cout << "Tarik tunai sejumlah " << jumlah << " berhasil. Sisa saldo: " << currentUser->rekeningHead->saldo << endl;
    }
    else
    {
        cout << "Saldo tidak mencukupi.\n";
    }
}

void setorTunai()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }

    int jumlah;
    cout << "Masukkan jumlah yang ingin disetor: ";
    cin >> jumlah;

    currentUser->rekeningHead->saldo += jumlah;
    tambahTransaksi("Setor Tunai", jumlah);

    currentUser->rekeningHead->saldo -= jumlah;

    Transaksi *newTransaksi = new Transaksi{"Setor Tunai", jumlah, nullptr};
    pushStack(newTransaksi);
    konfirmasiTransaksi();

    cout << "Setor tunai sejumlah " << jumlah << " berhasil. Saldo saat ini: " << currentUser->rekeningHead->saldo << endl;
}


void transfer() {
    string usernameTujuan;
    int jumlah;
    cout << "Masukkan username tujuan: ";
    cin >> usernameTujuan;
    cout << "Masukkan jumlah transfer: ";
    cin >> jumlah;

    if (jumlah <= 0) {
        cout << "Jumlah transfer tidak valid.\n";
        return;
    }

    Nasabah *nasabahTujuan = cariNasabah(usernameTujuan);
    if (!nasabahTujuan) {
        cout << "Nasabah tujuan tidak ditemukan.\n";
        return;
    }

    Rekening *rekeningPengirim = currentUser->rekeningHead;
    Rekening *rekeningPenerima = nasabahTujuan->rekeningHead;

    if (rekeningPengirim && rekeningPenerima) {
        if (rekeningPengirim->saldo >= jumlah) {
            rekeningPengirim->saldo -= jumlah;
            rekeningPenerima->saldo += jumlah;
            tambahTransaksi("Transfer Keluar", jumlah);

            int src = hashFunction(currentUser->username);
            int dest = hashFunction(usernameTujuan);
            addEdge(transferGraph, src, dest, jumlah);

            Transaksi *newTransaksi = new Transaksi{"Transfer Masuk", jumlah, nullptr};
            pushStack(newTransaksi);
            konfirmasiTransaksi();

            Transaksi *newTransaksiMasuk = new Transaksi{"Transfer Masuk", jumlah, nasabahTujuan->transaksiHead};
            nasabahTujuan->transaksiHead = newTransaksiMasuk;
        }
        else {
            cout << "Saldo tidak mencukupi.\n";
        }
    }
}

void tambahPermintaanPinjaman(string username, int jumlah)
{
    Pinjaman *newPinjaman = new Pinjaman{username, jumlah, nullptr};
    if (!pinjamanFront)
    {
        pinjamanFront = pinjamanRear = newPinjaman;
    }
    else
    {
        pinjamanRear->next = newPinjaman;
        pinjamanRear = newPinjaman;
    }
    cout << "Permintaan pinjaman berhasil diajukan.\n";
}


void pinjamDana()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }

    float jumlahPinjaman;
    cout << "Masukkan jumlah pinjaman: ";
    cin >> jumlahPinjaman;
    cout << "Pinjaman berhasil sebesar: " << jumlahPinjaman << endl;

    tambahPermintaanPinjaman(currentUser->username, jumlahPinjaman);
}

void prosesPermintaanPinjaman()
{
    while (pinjamanFront)
    {
        Pinjaman *current = pinjamanFront;
        Nasabah *user = cariNasabah(current->username);
        if (user)
        {
            char persetujuan;
            cout << "Permintaan pinjaman dari " << current->username << " sebesar " << current->jumlah << ". Setujui? (y/n): ";
            cin >> persetujuan;
            if (persetujuan == 'y' || persetujuan == 'Y')
            {
                user->rekeningHead->saldo += current->jumlah;
                tambahTransaksi("Pinjam Dana", current->jumlah);
                cout << "Pinjaman disetujui.\n";
            }
            else
            {
                cout << "Pinjaman ditolak.\n";
            }
        }

        pinjamanFront = pinjamanFront->next;
        delete current;
    }
    pinjamanRear = nullptr;
}


void kembalikanDana()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }

    float jumlah;
    cout << "Masukkan jumlah yang ingin dikembalikan: ";
    cin >> jumlah;

    if (currentUser->rekeningHead->saldo >= jumlah)
    {
        currentUser->rekeningHead->saldo -= jumlah;
        tambahTransaksi("Kembalikan Dana", jumlah);
        cout << "Dana berhasil dikembalikan.\n";
    }
    else
    {
        cout << "Saldo tidak mencukupi.\n";
    }
}
void lihatSaldo()
{
    if (!currentUser)
    {
        cout << "Anda harus login terlebih dahulu.\n";
        return;
    }
    cout << "=================================\n";

    cout << "Saldo saat ini: " << currentUser->rekeningHead->saldo << endl;
    cout << "=================================\n";
}

void menuAdmin()
{
    int pilihan;
    do
    {
        cout << "=====================================\n";
        cout << "|            MENU ADMIN             |\n";         
        cout << "===================================== \n";
        cout << "|1. Cari Data Nasabah               |\n";
        cout << "|2. Lihat Riwayat Transaksi Nasabah |\n";
        cout << "|3. Proses Permintaan Pinjaman      |\n";
        cout << "|4. Tampilkan Semua Nasabah         |\n";
        cout << "|5. Keluar                          |\n";
        cout << "===================================== \n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            cariDataNasabah();
            break;
        case 2:
            lihatRiwayatTransaksiNasabah();
            break;
        case 3:
            prosesPermintaanPinjaman();
            break;
        case 4:
                cout << "Daftar Nasabah:\n";
                tampilkanTreeNasabah(root);
                break;
        }
    } while (pilihan != 5);
}

void menuUser()
{
    int pilihan;
    transferGraph = createGraph(TABLE_SIZE);
    do
    {
        cout << "============================\n";
        cout << "|        MENU USER         |\n";
        cout << "============================\n";
        cout << "|1. Detail Rekening        |\n";
        cout << "|2. Detail Kartu Kredit    |\n";
        cout << "|3. lihat saldo            |\n";
        cout << "|4. Tarik Tunai            |\n";
        cout << "|5. Setor Tunai            |\n";
        cout << "|6. Transfer               |\n";
        cout << "|7. Lihat Riwayat Transaksi|\n";
        cout << "|8. Pinjam Dana            |\n";
        cout << "|9. Kembalikan Dana        |\n";
        cout << "|10. Logout                |\n";
        cout << "============================\n";
        cout << "Pilih: ";

        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            detailRekening();
            break;
        case 2:
            detailKartuKredit();
            break;
        case 3:
            lihatSaldo();
            break;
        case 4:
            tarikTunai();
            break;
        case 5:
            setorTunai();
            break;
        case 6:
            transfer();
            break;
        case 7:
            lihatRiwayatTransaksi();
            break;
        case 8:
            pinjamDana();
            break;
        case 9:
            kembalikanDana();
            break;
        case 10:
            cout << "Logout berhasil.\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            break;
        }
    } while (pilihan != 10);
}

bool loginAdmin(string nama, string password)
{

    struct Admin
    {
        string nama;
        string password;
    };

    const int MAX_ADMINS = 10;
    Admin admins[MAX_ADMINS] = {
        {"admin1", "password1"},
        {"admin2", "password2"},

    };
    int jumlahAdmin = sizeof(admins) / sizeof(admins[0]);

    for (int i = 0; i < jumlahAdmin; ++i)
    {
        if (admins[i].nama == nama && admins[i].password == password)
        {
            return true;
        }
    }
    return false;
}


bool loginUser(string username, string passworduser)
{

    Nasabah *user = cariNasabah(username);
    if (user && user->passworduser == passworduser)
    {
        currentUser = user;
        transaksiHead = user->transaksiHead;
        return true;
    }
    return false;
}

int main()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        nasabahTable[i] = nullptr;
    }

    int pilihan;
    string nama, password;
    string username, passworduser;
    do
    {
        cout << "==============\n";
        cout << "| Menu Utama |\n";
        cout << "==============\n";
        cout << "|1. Admin    |\n";
        cout << "|2. User     |\n";
        cout << "|3. Keluar   |\n";
        cout << "==============\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            cout << "Masukkan nama admin: ";
            cin >> nama;
            cout << "Masukkan password admin: ";
            cin >> password;

            if (loginAdmin(nama, password))
            {
                cout << "Login berhasil.\n";
                cout << "===================\n";
                menuAdmin();
            }
            else
            {
                cout << "Login gagal.\n";
            }
            break;
        case 2:
    int pilihanuser;
    cout << "==============\n";
    cout << "|1. Login    |\n";
    cout << "|2. Buat Akun|\n";
    cout << "==============\n";
    cout << "Pilih: ";
    cin >> pilihanuser;

    switch (pilihanuser)
    {
        case 1:
            cout << "Masukkan username: ";
            cin >> username;
            cout << "Masukkan password: ";
            cin >> passworduser;

            if(loginUser(username, passworduser))
            {
                cout << "Login berhasil.\n";
                cout << "====================\n";
                menuUser();
            }
            else{
                cout << "Login gagal.\n";
            }
            break;
        case 2:
            buatAkun();
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            break;
    }
    break;

        case 3:
            cout << "Keluar dari program.\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            break;
        }
    } while (pilihan != 3);

    return 0;
}