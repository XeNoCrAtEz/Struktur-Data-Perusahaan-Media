#ifndef SERVER_HEADER
#define SERVER_HEADER

#define MAX_MEDIA 1000

typedef struct SNode Node;
typedef struct SMediaData MediaData;

struct SMediaData{      //struct untuk menyimpan data dari sebuah media
    char mediaFilename[250];    //nama media
    int lastAccessed;   //kapan terakhir kali media diakses
    int dateCreated;    //kapan media dibentuk
    int AccessedFrequency;      //berapa kali media diakses
    int id;    //ID unik media tersebut
    int DeletionKey;    //angka unik yang mendefinisikan apakah media ini layak dihapus atau tidak
    //DeletionKey = lasAccessed + dateCreated + AccessedFrequency
};

struct SNode{       //Node dari tree yang menyimpan data media
    MediaData mediaInfo;    //node berisi struct media tersebut
    Node* ptrLeftNode;      //serta pointer ke node kiri
    Node* ptrRightNode;     //dan node kanan
};

typedef Node BinaryTree;    //membuat tipe data BinaryTree

typedef struct SServer Server;      //membuat tipe data untuk server itu sendiri
struct SServer{
    BinaryTree* ServerTree;     //terdiri atas tree yang dinamakaan ServerTree
    int serverSize;              //serta ukuran dari server tersebut
};

//Queue untuk Breadth-First Search
typedef struct SQueue Queue;
struct SQueue{      //Queue menyimpan array of "pointer to Node"
    Node** Q;
    int head, tail, size;   //head, tail, dan size disimpan dlm bntk integer
};
#define MIN -1      //definisi MIN untuk Queue

BinaryTree* MakeBinaryTree(MediaData pMediaInfo);   //fungsi untuk membuat tree baru
int IsEmptyTree(BinaryTree* pBinaryTree); //fungsi untuk mengecek keadaan tree
BinaryTree* InsertNode(BinaryTree* pBinaryTree, MediaData pMediaInfo);  //fungsi untuk memasukkan data baru ke dalam server
Node* Search(BinaryTree* pBinaryTree, MediaData pMediaInfo);  //fungsi untuk mencari data dalam server
Node* BFS(MediaData pMediaInfo, Queue* pQ);    //fungsi Breadth-first search yang membantu fungsi Search
int checkEquality(MediaData mediaInfo1, MediaData mediaInfo2);
Node* FindOldest(BinaryTree* pBinaryTree);     //fungsi untuk mencari media paling tua
Node* FindTrending(BinaryTree* pBinaryTree);        //fungsi untuk mencari media paling baru
BinaryTree* DeleteNode(BinaryTree* pBinaryTree, MediaData pMediaInfo);      //fungsi untuk menghapus data lama
void ShowAll(BinaryTree* pBinaryTree);    //fungsi untuk menampilkan semua media yang ada di server
void ShowList(BinaryTree* pBinaryTree);

int EncodeDate(int dd, int mm, int yyyy);       //fungsi untuk mengubah bentuk tanggal dd/mm/yyyy menjadi angka
int* DecodeDate(int date);      //fungsi untuk mengubah angka menjadi bentuk tanggal
MediaData UpdateDelKey(MediaData pMediaInfo);       //fungsi untuk mengupdate deletionKey dari sebuah mediaInfo
int CountData(BinaryTree* pBinaryTree);     //fungsi untuk menghitung banyak data dalam server
int Count(Queue* pQ, int counter);
MediaData CreateNewMedia(char* pMediaFilename, int dateCreated, int lastAccessed, int AccessedFrequency, int id);

//implementasi demo server ini
Server* Demo(Server* Server);

//prototype fungsi untuk Queue
Queue* CreateEmptyQueue(Queue* pQ, int size);   //fungsi untuk membuat Queue
void DestroyQueue(Queue* pQ);       //fungsi untuk menghapus Queue
int IsEmptyQueue(Queue* pQ);        //fungsi untuk mengecek apakah Queue kosong
int IsFullQueue(Queue* pQ);         //fungsi untuk mengecek apakah Queue penuh
void Enqueue(Queue* pQ, Node* pNode);    //fungsi untuk memasukkan "pointer ke Node" ke dalam Queue
Node* Dequeue(Queue* pQ);       //fungsi untuk mengeluarkan "pointer ke Node" dari Queue

#endif