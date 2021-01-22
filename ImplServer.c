#include "ServerHeader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//definisi fungsi untuk membuat tree dengan data inisial pMediaInfo
BinaryTree* MakeBinaryTree(MediaData pMediaInfo){
	BinaryTree* bt;     //membuat pointer binarytree
	bt = (BinaryTree*) malloc (sizeof(BinaryTree) * 1);     //mengalokasikan memori sebesar binarytree
	bt->mediaInfo = pMediaInfo;
	bt->ptrLeftNode = NULL;
	bt->ptrRightNode = NULL;
	return bt;      //return binarytree yang sudah dibentuk
}

//definisi fungsi untuk mengecek keadaan server apakah kosong atau tidak
int IsEmptyTree(BinaryTree* pBinaryTree){
	return (pBinaryTree == NULL) ? 1 : 0;
}

//definisi fungsi untuk memasukkan data ke dalam server
BinaryTree* InsertNode(BinaryTree* pBinaryTree, MediaData pMediaInfo){
	if(IsEmptyTree(pBinaryTree) != 1){		//cari node leafnya :
		if(pMediaInfo.DeletionKey >= pBinaryTree->mediaInfo.DeletionKey){	//jika node skrg ini lebih rendah nilainya
			pBinaryTree->ptrRightNode = InsertNode(pBinaryTree->ptrRightNode, pMediaInfo);	//lanjut ke node kanan
		}else pBinaryTree->ptrLeftNode = InsertNode(pBinaryTree->ptrLeftNode, pMediaInfo);	//else, lanjut ke node kiri
	} else {	//jika sudah sampai leaf node :
		pBinaryTree = MakeBinaryTree(pMediaInfo);		//buat tree baru dengan data pMediaInfo
	}
	return pBinaryTree;		//kembalikan tree yang sudah di-insert node baru
}

//fungsi untuk mencari data dengan kriteria yang diberikan
Node* Search(BinaryTree* pBinaryTree, MediaData pMediaInfo){
	Queue* pQ;
	pQ = CreateEmptyQueue(pQ, MAX_MEDIA);	//buat queue kosong untuk menyimpan node secara sementara
	//ukurannya sebesar banyaknya media yang bs ditampung oleh server, yaitu MAX_MEDIA
	//perbaikan selanjutnya : ukuran queue hanya sebesar jumlah media yg saat ini ditampung oleh tree
	Enqueue(pQ, pBinaryTree);		//masukkan root node ke dalam queue
	return BFS(pMediaInfo, pQ);	//lakukan Breadth-First Search
}
//fungsi Breadth-First Search untuk membantu fungsi Search
Node* BFS(MediaData pMediaInfo, Queue* pQ){
	if(IsEmptyQueue(pQ) == 1){		//cek apakah queue kosong, yang artinya semua node sudah dijalani
		return NULL;		//Jika ya, maka  langsung return NULL, dan proses searching dihentikan
	} else {		//jika tidak, maka lakukan breadth-first search
		Node* thisNode = Dequeue(pQ);		//simpan "pointer ke node" yang ada di queue untuk sementara
		if(checkEquality(thisNode->mediaInfo, pMediaInfo) == 1){	//cek apakah node ini mengandung mediaInfo yg sama dengan yang kita cari
			return thisNode;		//jika iya, berarti ini adalah node yang kita cari
		} else {	//jika tidak...
			Enqueue(pQ, thisNode->ptrLeftNode);		//masukkan data left node ke dalam queue
			Enqueue(pQ, thisNode->ptrRightNode);	//masukkan data right node ke dalam queue
			return BFS(pMediaInfo, pQ);
		}
	}
}
//fungsi untuk mengecek apakah kedua media adalah sama
int checkEquality(MediaData mediaInfo1, MediaData mediaInfo2){
	//dua media dikatakan sama jika namanya sama
	return (strcmp(mediaInfo1.mediaFilename, mediaInfo2.mediaFilename) == 0) ? 1 : 0;
}

//mencari data terlama dari server
Node* FindOldest(BinaryTree* pBinaryTree){
	if(IsEmptyTree(pBinaryTree) == 1){
		return NULL;
	} else {
		return IsEmptyTree(pBinaryTree->ptrLeftNode) == 1 ? pBinaryTree : FindOldest(pBinaryTree->ptrLeftNode);
	}
}

//mencari data terbaru dari server
Node* FindTrending(BinaryTree* pBinaryTree){
	if(IsEmptyTree(pBinaryTree) == 1){
		return NULL;
	} else {
		return IsEmptyTree(pBinaryTree->ptrRightNode) == 1 ? pBinaryTree : FindTrending(pBinaryTree->ptrRightNode);
	}
}

//fungsi untuk menghapus data dari server. Fungsi ini digunakan pada saat menekan 
BinaryTree* DeleteNode(BinaryTree* pBinaryTree, MediaData pMediaInfo){
	if(IsEmptyTree(pBinaryTree) == 1) return NULL;	//jika hasil search adalah NULL, berarti node tidak ditemukan
	else if(pMediaInfo.DeletionKey < pBinaryTree->mediaInfo.DeletionKey) pBinaryTree->ptrLeftNode = DeleteNode(pBinaryTree->ptrLeftNode, pMediaInfo);
	else if(pMediaInfo.DeletionKey > pBinaryTree->mediaInfo.DeletionKey) pBinaryTree->ptrRightNode = DeleteNode(pBinaryTree->ptrRightNode, pMediaInfo);
	else{		//jika node ditemukan
		if(IsEmptyTree(pBinaryTree->ptrLeftNode) == 0 && IsEmptyTree(pBinaryTree->ptrRightNode) == 0){	//cek apakah node kiri dan kanannya kosong. jika ya,
			Node* tmp = FindTrending(pBinaryTree->ptrLeftNode);		//maka simpan sementara node paling trending di node kiri
			pBinaryTree->mediaInfo = tmp->mediaInfo;		//ubah mediaInfo dari node ini menjadi mediaInfo dari node paling trending
			pBinaryTree->ptrLeftNode = DeleteNode(pBinaryTree->ptrLeftNode, pBinaryTree->mediaInfo);	//hapus data paling trending tersebut
			free(tmp);	//hapus node yang sudah kita simpan sementara tadi
		} else {
			if(IsEmptyTree(pBinaryTree->ptrLeftNode) == 1) pBinaryTree = pBinaryTree->ptrRightNode;
			else if(IsEmptyTree(pBinaryTree->ptrRightNode) == 1) pBinaryTree = pBinaryTree->ptrLeftNode;
		}
	}
	return pBinaryTree;
}

//fungsi untuk menampilkan semua data media di server, mulai dari yang paling trending
void ShowAll(BinaryTree* pBinaryTree){
	printf("Nama File\t\t\t|Tanggal dibuat\t\t|Jumlah Akses\n");
	printf("--------------------------------|-----------------------|---------------------\n");
	ShowList(pBinaryTree);
}
void ShowList(BinaryTree* pBinaryTree){		//fungsi pembantu fungsi showAll untuk menampilkan semua data
	if(IsEmptyTree(pBinaryTree) != 1){
		ShowList(pBinaryTree->ptrRightNode);
		printf("%s\t\t", pBinaryTree->mediaInfo.mediaFilename);
		int* decodedDate = DecodeDate(pBinaryTree->mediaInfo.dateCreated);
		//dd = decodedDate[0]
		//mm = decodedDate[1]
		//yyyy = decodedDate[2]
		printf("| %d/%d/%d\t\t", decodedDate[0], decodedDate[1], decodedDate[2]);
		printf("| %d\n", pBinaryTree->mediaInfo.AccessedFrequency);
		ShowList(pBinaryTree->ptrLeftNode);
	}
}

//fungsi untuk mengubah bentuk tanggal dd/mm/yyyy menjadi angka
int EncodeDate(int dd, int mm, int yyyy){
	int date = 0;
	date |= (dd&0xFF);
	date |= (mm&0xFF)<<8;
	date |= (yyyy&0xFFFF)<<16;
	return date;
}
//fungsi untuk mengubah bentuk tanggal integer menjadi dd/mm/yyyy
int* DecodeDate(int date){
	int dd, mm, yyyy;
	int tes = date;
	dd = (tes & 0xff);
    mm = ((tes>>8) & 0xff);
    yyyy = ((tes>>16) & 0xffff);
	int* decodedDate = (int*) malloc (sizeof(int) * 3);		//buat array untuk menampung dd, mm, yyyy
	decodedDate[0] = dd;
	decodedDate[1] = mm;
	decodedDate[2] = yyyy;
	return decodedDate;
}

//fungsi untuk mengupdate nilai DeletionKey dari sebuah mediaInfo
MediaData UpdateDelKey(MediaData pMediaInfo){
	int accFreq = pMediaInfo.AccessedFrequency;
	int createdDate = pMediaInfo.dateCreated;
	int lastAcc = pMediaInfo.lastAccessed;
	pMediaInfo.DeletionKey = accFreq + createdDate + lastAcc;	//nilai "tua" atau tidaknya sebuah media bergantung pada jumlah
	return pMediaInfo;											//dateCreated, lastAccessed, dan AccessedFrequency
}

//fungsi untuk menghitung banyak data yang disimpan dalam server
int CountData(BinaryTree* pBinaryTree){		//penghitungan jumlah data dalam server dilakukan
	Queue* pQ;								//dengan cara breadth-first search, sehingga
	pQ = CreateEmptyQueue(pQ, MAX_MEDIA);	//membutuhkan queue, dll.
	Enqueue(pQ, pBinaryTree);
	return Count(pQ, 0);
}
int Count(Queue* pQ, int counter){		//fungsi pembantu countdata
	if(IsEmptyQueue(pQ) == 1) return counter;
	Node* thisNode = Dequeue(pQ);
	counter++;								//setiap kali menemukan sebuah node, counter ditambah 1
	Enqueue(pQ, thisNode->ptrLeftNode);
	Enqueue(pQ, thisNode->ptrRightNode);
	return Count(pQ, counter);
}

//fungsi untuk membuat media data yang baru
MediaData CreateNewMedia(char* pMediaFilename, int dateCreated, int lastAccessed, int AccessedFrequency, int id){
	MediaData* newMediaData = (MediaData*) malloc (sizeof(MediaData));
	strcpy(newMediaData->mediaFilename, pMediaFilename);		//berikan nama sesuai dengan yg diberikan
	newMediaData->id = id;		//berikan ID yg sesuai (newest ID + 1)
	newMediaData->dateCreated = dateCreated;		//masukkan tanggal media tsb dibuat
	newMediaData->lastAccessed = lastAccessed;		//karena baru saja dibuat, tentu saja terakhir kali diakses adalah pada hari ini juga
	newMediaData->AccessedFrequency = AccessedFrequency;		//media yang baru saja dibuat tentu saja blm prnh diakses
	*newMediaData = UpdateDelKey(*newMediaData);		//update deletion key
	return *newMediaData;		//kembalikan media yang sudah dibuat
}

//fungsi demo untuk video
Server* Demo(Server* server){
	FILE* mediaDataFile;
	mediaDataFile = fopen("MediaFile.txt", "r");		//buka file txt berisi data 1000 media yang tersimpan dalam server

	MediaData newMedia;		//buat variable untuk menampung media yang baru
	char stringPerLine[300];
	char newMediaFilename[250];
	int newMediaLastAcc;
	int newMediaDateCreated;
	int newMediaAccFreq;
	int newMediaID;

	while(1){		//looping hingga End-Of-File dari mediaDataFile
		if( feof(mediaDataFile) ) break;
		fgets(stringPerLine, 250, mediaDataFile);		//ambil satu baris string dari file mediaDataFile
		char* identifier = strtok(stringPerLine, "=\n");		//potong string di bagian "=" dan "\n"
		while(strncmp(stringPerLine, ";", 1) != 0){		//tanda akhir dari data sebuah media adalah ";"
			identifier = strtok(NULL, "=\n");		//ambil string yang berada setelah "=", string ini berisi data seperti nama, tanggal akses, dll
			if(strcmp(stringPerLine, "mediaFilename") == 0) strcpy(newMediaFilename, identifier);		//masukkan data di identifier sesuai dengan
			if(strcmp(stringPerLine, "lastAccessed") == 0 || strcmp(stringPerLine, "dateCreated") == 0){	//yang tertulis sebelum tanda "="
				char* dd = strtok(identifier, "/");			//tanggal masih dalam format "dd/mm/yyyy"
				char* mm = strtok(NULL, "/");				//sehingga harus kita pisahkan menjadi 3 string yang berbeda
				char* yyyy = strtok(NULL, "/");
				int intdd = atoi(dd);			//lalu kita ubah menjadi integer, serta kita encode,
				int intmm = atoi(mm);			//dan kita masukkan ke dalam lastAccessed atau DateCreated, sesuai dengan yang tertulis
				int intyyyy = atoi(yyyy);
				if(strcmp(stringPerLine, "lastAccessed") == 0) newMediaLastAcc = EncodeDate(intdd, intmm, intyyyy);
				else newMediaDateCreated = EncodeDate(intdd, intmm, intyyyy);
			}
			if(strcmp(stringPerLine, "AccessedFrequency") == 0) newMediaAccFreq = atoi(identifier);		//jika idetifier adalah data untuk AccessedFreq
			if(strcmp(stringPerLine, "id") == 0) newMediaID = atoi(identifier);		//jika identifier berisi data untuk ID media tersebut
			fgets(stringPerLine, 250, mediaDataFile);		//isi ulang stringPerLine dengan baris yang baru
			identifier = strtok(stringPerLine, "=");		//serta identifier juga diisi dengan data yang baru
		}
		//jika semua data yang tertulis di text sudah dimasukkan ke variable sementara (ditandai dengan adanya ";")
		//buat mediaData baru, lalu masukkan ke dalam server (hanya jika tidak melewati kapasitas server)
		if(CountData(server->ServerTree) < MAX_MEDIA){
			newMedia = CreateNewMedia(newMediaFilename, newMediaDateCreated, newMediaLastAcc, newMediaAccFreq, newMediaID);
			if(server->ServerTree == NULL) server->ServerTree = MakeBinaryTree(newMedia);
			else InsertNode(server->ServerTree, newMedia);
		} else break;
		}
	//tampilkan semua data yang berhasil dimasukkan
	server->serverSize = CountData(server->ServerTree);
	printf("Jumlah media dalam server : %d\n", server->serverSize);
	ShowAll(server->ServerTree);
	return server;
}

//------------------------------------------------------------------------------------------
//implementasi Queue
//fungsi untuk membentuk Queue baru
Queue* CreateEmptyQueue(Queue* pQ, int size){
	pQ = (Queue*) malloc(sizeof(Queue));
	pQ->size = size;
	pQ->Q = (Node**) malloc(sizeof(Node*) * pQ->size);
	pQ->head = MIN;
	pQ->tail = MIN;
	return pQ;
}

//fungsi untuk menghapus queue
void DestroyQueue(Queue* pQ){
	free(pQ->Q);
	free(pQ);
}

//fungsi untuk mengecek apakah queue kosong atau tidak
int IsEmptyQueue(Queue* pQ){
	return (pQ->tail == MIN) ? 1 : 0;
}

//fungsi untuk mengecek apakah queue penuh atau tidak
int IsFullQueue(Queue* pQ){
	return (pQ->tail == pQ->size - 1) ? 1 : 0;
}

//fungsi untuk memasukkan "pointer ke node" ke dalam queue
void Enqueue(Queue* pQ, Node* pNode){
	if(pNode != NULL){
		if(pQ->head == MIN) pQ->head++;		//majukan head satu langkah jika queue masih kosong
		pQ->tail++;		//majukan tail satu langkah
		pQ->Q[pQ->tail] = pNode;		//masukkan "pointer ke node" ke index queue yang ditunjuk oleh tail
	}
}

//fungsi untuk mengambil "pointer ke node" dari dalam queue
Node* Dequeue(Queue* pQ){
	if(IsEmptyQueue(pQ) == 0){		//cek apakah queue kosong atau tidak
		Node* DQNode = pQ->Q[pQ->head];		//simpan "pointer ke Node" untuk sementara
		int i;
		for(i = 0; i < pQ->tail; i++) pQ->Q[i] = pQ->Q[i+1];	//pindahkan semua data satu langkah ke kiri
		pQ->Q[pQ->tail] = NULL;		//data yang ditunjuk oleh tail menjadi NULL
		pQ->tail--;		//mundurkan tail satu langkah
		return DQNode;		//kembalikan "pointer ke node" yang sudah kita simpan
	}
}