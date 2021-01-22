#include "ServerHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char* argv[]){
    time_t t = time(NULL);
    struct tm Today = *localtime(&t);
    //UI utama program
    Server* server0 = (Server*) malloc(sizeof(Server));     //buat server baru
    server0->serverSize = 0;        //inisialisasikan ukuran
    server0->ServerTree = NULL;     //inisialisasikan tree-nya
    while(1 == 1){
        printf("Menu :\n");     //tampilkan menu
        printf("1. Akses media\n");
        printf("2. Tampilkan semua media yang ada di server\n");
        printf("3. Keluar dari program\n\n");
        printf("pilihan? (masukkan angka yang sesuai) ");
        int pilihan;
        scanf("%d", &pilihan);      //minta pilihan dari user
        getchar();      //getchar() selalu ditambahkan setelah scanf agar input user yang selanjutnya tidak ter-skip
        if(pilihan == 1){           //jika user ingin mengakses sebuah media
            printf("\nMasukkan nama media yang ingin dicari : ");     //minta nama media yang ingin diakses
            char mediaName[250];
            scanf(" %s", &mediaName);        //simpan dalam mediaName
            getchar();
            MediaData tmpMediaInfo = CreateNewMedia(mediaName, 0, 0, 0, 0);       //buat data media sementara, untuk proses pencarian data
            Node* theMediaNode = Search(server0->ServerTree, tmpMediaInfo);     //cari node yang menyimpan data media tersebut
            if(theMediaNode == NULL){       //jika node tersebut tidak ditemukan (media tidak ditemukan)
                printf("\nMedia tidak ditemukan!\n");
                if(server0->serverSize == MAX_MEDIA){       //cek apakah server sudah penuh
                    printf("\nServer penuh!! Menghapus media paling tua...\n");
                    Node* oldestNode = FindOldest(server0->ServerTree);
                    server0->ServerTree = DeleteNode(server0->ServerTree, oldestNode->mediaInfo);       //jika ya, hapus data yang tertua
                }
                printf("\nMasukkan nama media yang baru : ");     //lakukan prosedur pembuatan media yang baru
                scanf(" %s", &mediaName);
                getchar();
                printf("Masukkan tanggal pembuatan media (dd/mm/yyyy) : ");
                int dd, mm, yyyy;
                scanf("%d/%d/%d", &dd, &mm, &yyyy);
                getchar();
                Node* trendingNode = FindTrending(server0->ServerTree);
                int newID;
                if(trendingNode == NULL) newID = 0;     //untuk menghindari segmentation fault untuk media yang pertama sekali dimasukkan ke dalam server
                else{
                    int newID = trendingNode->mediaInfo.id + 1;     //ID dari media yang baru adalah id media kedua terbaru ditambah 1
                }
                int LastAccessed = EncodeDate(Today.tm_mday, Today.tm_mon+1, Today.tm_year+1900);
                MediaData newMedia = CreateNewMedia(mediaName, EncodeDate(dd, mm, yyyy), LastAccessed, 0, newID);
                server0->ServerTree = InsertNode(server0->ServerTree, newMedia);
                printf("\nMedia berhasil ditambahkan!!\n\n");
                printf("Press ENTER to go back to menu...\n");
                getchar();

            } else{
                printf("\nMedia ditemukan!\n");
                printf("\nNama media             : %s\n", theMediaNode->mediaInfo.mediaFilename);     //tampilkan nama media
                int* dateCreated = DecodeDate(theMediaNode->mediaInfo.dateCreated);     //ambil data tanggal dibentuknya media tersebut
                int* lastAccessed = DecodeDate(theMediaNode->mediaInfo.lastAccessed);
                printf("Tanggal media dibentuk : %d/%d/%d\n", dateCreated[0], dateCreated[1], dateCreated[2]);
                printf("Jumlah akses           : %d\n", theMediaNode->mediaInfo.AccessedFrequency);
                printf("Terakhir diakses       : %d/%d/%d\n", lastAccessed[0], lastAccessed[1], lastAccessed[2]);
                printf("Media ID               : %d\n", theMediaNode->mediaInfo.id);
                //re-insert media node yg sudah diakses
                server0->ServerTree = DeleteNode(server0->ServerTree, theMediaNode->mediaInfo);     //hapus media tersebut
                theMediaNode->mediaInfo.lastAccessed = EncodeDate(Today.tm_mday, Today.tm_mon+1, Today.tm_year+1900);       //update tanggal terakhir diakses
                theMediaNode->mediaInfo.AccessedFrequency += 1;     //karena media sudah diakses... tambahkan AccessedFrequency sebanyak 1
                theMediaNode->mediaInfo = UpdateDelKey(theMediaNode->mediaInfo);
                InsertNode(server0->ServerTree, theMediaNode->mediaInfo);
                printf("\nPress ENTER to go back to menu...\n");
                getchar();
            }
        } else if(pilihan == 2){        //tampilkan semua media yang tersimpan dalam server, serta jumlah media yang tersimpan
            server0->serverSize = CountData(server0->ServerTree);
            printf("Jumlah media dalam server : %d\n", server0->serverSize);
            ShowAll(server0->ServerTree);
            printf("\nPress ENTER to go back to menu...\n");
            getchar();
        } else if(pilihan == 3){
            break;
        } else if(pilihan == 99){       //hidden option "99", melakukan demo server ini dengan menggunakan data yang berada di mediaFile.txt
            server0 = Demo(server0);
        }
        system("cls");
    }
    return 0;
}