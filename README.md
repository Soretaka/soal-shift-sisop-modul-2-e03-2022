# Soal-shift-sisop-modul-2-E03-2022
Anggota:

1. Daniel Hermawan (5025201087)
2. Florentino Benedictus (5025201222)
3. Anak Agung Yatestha Parwata (5025201234)

## Soal No. 1
Dalam soal ini, kita diminta membuat program dalam 1 file `soal1.c` yang akan menjalankan beberapa fungsi.

### 1A
```c
void extractFile(){
	int status;
	if(fork()==0){
		char temp[100];
		strcpy(temp, dir);
		strcat(temp, "/gacha_gacha");
		
		char *argv[] = {"unzip", "char.zip", "-d", temp, NULL};
		execv("/bin/unzip", argv);
	}
	else{
		while(wait(&status) > 0);
		char temp[100];
		strcpy(temp, dir);
		strcat(temp, "/gacha_gacha");
		
		char *argv2[] = {"unzip", "weap.zip", "-d", temp, NULL};
		execv("/bin/unzip", argv2);
	}
}
void downloadFile(){
	int status;
	if(fork()==0){
		char temp[50];
		strcpy(temp, dir);
		//strcat(temp, "/gacha_gacha");
		strcat(temp, "/char.zip");
		char *argv[] = {"wget", "-q", "drive.google.com/uc?export=download&id=1xYYmsslb-9s8-4BDvosym7R4EmPi6BHp", "-O", temp};
		execv("/bin/wget", argv);
	}
	else{
		while(wait(&status) > 0);
		char temp[50];
		strcpy(temp, dir);
		//strcat(temp, "/gacha_gacha");
		strcat(temp, "/weap.zip");
		char *argv2[] = {"wget", "-q", "drive.google.com/uc?export=download&id=1XSkAqqjkNmzZ0AdIZQt_eWGOZ0eJyNlT", "-O", temp};
        	execv("/bin/wget", argv2);
	}
}
void createDir(){
	pid_t child_id;
	child_id = fork();
	int status;
	if (child_id < 0) {
		exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  	}
        if(child_id == 0){
		char temp[50];
		strcpy(temp, dir);
		strcat(temp, "/gacha_gacha");
		char *argv[] = {"mkdir", "-p", temp, NULL};
		execv("/bin/mkdir", argv);
	}
	else{
		while(wait(&status) > 0);
		pid_t child_id2;
		//status = NULL;
		child_id2 = fork();
		if (child_id2 < 0) {
		exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
		}
		if(child_id2 == 0){
			downloadFile();
		}
		else{
			while(wait(&status) > 0);
			extractFile();
		}
	}	
}
```
Pertama program akan menjalankan fungsi `createDir()`, lalu dengan menggunakan fork dan exec akan membuat directory `gacha_gacha` di path /home/user. Kemudian setelah directory tersebut dibuat, program akan melakukan fork lagi sehingga child akan menjalankan `downloadFile()` yang akan mendownload kedua file .zip dan parentnya akan menjalankan `extractFile()` yang melakukan extract kedua file .zip ke dalam folder `gacha_gacha`

### 1B, 1C, 1D
```c
void saveFileName(){
		// char file names
		DIR *d;
		struct dirent *dirs;
		char temp[100];
		strcpy(temp, dir);
		strcat(temp, "/gacha_gacha");
		strcat(temp, "/characters");
		d = opendir(temp);
		if (d) {
			while ((dirs = readdir(d)) != NULL) {
				if(strcmp(dirs->d_name, ".")==0 ||
				   strcmp(dirs->d_name, "..")==0)continue;
				strcpy(fileChar[charCount],dirs->d_name);
				charCount++;
			}
		closedir(d);
		}
		// weap file names
		strcpy(temp, dir);
		strcat(temp, "/gacha_gacha");
		strcat(temp, "/weapons");
		d = opendir(temp);
		if (d) {
			while ((dirs = readdir(d)) != NULL) {
				if(strcmp(dirs->d_name, ".")==0 ||
				   strcmp(dirs->d_name, "..")==0)continue;
				strcpy(fileWeap[weapCount],dirs->d_name);
				weapCount++;
			}
		closedir(d);
		}
		printf("---------------\nJumlah Char: %d\nJumlah Weap: %d\nJika salah satu jumlah char/weap 0 file corrupt ketika download!\n---------------\n", charCount, weapCount);
}
void saveDataValues(){
	char buffer[4096];
	if(charCount){
		// save char values to arr
		FILE *fp;
		struct json_object *parsed_json;
		struct json_object *name;
		struct json_object *rarity;
		for(int i=0; i<charCount; i++){
			char temp[100];
			strcpy(temp, "");
			strcat(temp, dir);
			strcat(temp, "/gacha_gacha/characters/");
			strcat(temp, fileChar[i]);
			fp = fopen(temp, "r");
			fread(buffer, 4096, 1, fp);
			fclose(fp);
			parsed_json = json_tokener_parse(buffer);

			json_object_object_get_ex(parsed_json, "name", &name);
			json_object_object_get_ex(parsed_json, "rarity", &rarity);

			strcpy(nameChar[i], json_object_get_string(name));
			strcpy(rarityChar[i], json_object_get_string(rarity));
		}
	}
	if(weapCount){
		// save weap values to arr
		FILE *fp;
		struct json_object *parsed_json;
		struct json_object *name;
		struct json_object *rarity;
		for(int i=0; i<weapCount; i++){
			char temp[100];
			strcpy(temp, "");
			strcat(temp, dir);
			strcat(temp, "/gacha_gacha/weapons/");
			strcat(temp, fileWeap[i]);

			fp = fopen(temp, "r");
			fread(buffer, 4096, 1, fp);
			fclose(fp);

			parsed_json = json_tokener_parse(buffer);

			json_object_object_get_ex(parsed_json, "name", &name);
			json_object_object_get_ex(parsed_json, "rarity", &rarity);
			
			strcpy(nameWeap[i], json_object_get_string(name));
			strcpy(rarityWeap[i], json_object_get_string(rarity));
		}
	}
printf("PARSING DONE\n");
}
void dataInit(){
	saveFileName();
	saveDataValues();
}
```
Selanjutnya setelah `createDir()` selesai dilakukan, program akan menjalankan `dataInit()`. Fungsi ini terdiri dari `saveFileName()` yang akan mencatat nama dari masing-masing file .json yang telah terextract dari .zip. Selanjutnya dijalankan `saveDataValues()` yang akan melakukan parsing .json lalu menyimpan atribut `name` dan `rarity` dari tiap file .json weapons dan characters.
```c
void changeFolder(){
	char folderName[30];
	int status;
	printf("Ganti folder!\n");
	strcpy(folderPath, "");
	strcat(folderPath, dir);
	strcat(folderPath, "/gacha_gacha");
		
	strcpy(folderName, "");
	strcat(folderName, "/total_gacha_");
	tostring(numgacha);
	strcat(folderName, out);
		
	strcat(folderPath, folderName);
	if(fork()==0){
		char *argv[] = {"mkdir", "-p", folderPath, NULL};
		execv("/bin/mkdir", argv);
	}
	else {
		while(wait(&status) > 0);
		printf("folder terbuat di: %s\n", folderPath);
		//strcat(folderPath, folderName);
	}
}
void changeTxt(){
	char txtName[40];
	int status;
	strcpy(txtPath, "");
	strcat(txtPath, folderPath);
	time_t seconds;
	struct tm *timeStruct;
	seconds = time(NULL);
	// change localtime
	timeStruct = localtime(&seconds);
	
	strcpy(txtName, "");
	strcat(txtName, "/");
	tostring(timeStruct->tm_hour);
	if(timeStruct->tm_hour > 9)strcat(txtName, out);
	else {
		strcat(txtName, "0");
		tostring(timeStruct->tm_hour);
		strcat(txtName, out);
	}
	strcat(txtName, ":");
	tostring(timeStruct->tm_min);
	if(timeStruct->tm_min > 9)strcat(txtName, out);
	else {
		strcat(txtName, "0");
		tostring(timeStruct->tm_min);
		strcat(txtName, out);
	}
	strcat(txtName, ":");
	tostring(timeStruct->tm_sec);
	if(timeStruct->tm_sec > 9)strcat(txtName, out);
	else{
		strcat(txtName, "0");
		tostring(timeStruct->tm_sec);
		strcat(txtName, out);
	}

	strcat(txtName, "_gacha_");
	tostring(numgacha);
	strcat(txtName, out);
	strcat(txtName, ".txt");
	char buff[256];
	strcat(txtPath, txtName);
	FILE * fp;
	fp = fopen (txtPath, "a+");
	fprintf(fp, "Path Text: %s\n\n", txtPath);
	fclose(fp);

printf("txt terbuat di: %s\n", txtPath);
	// wait 1 sec
	sleep(1);
}
void charToTxt(int RNGchar){
	int status;
	char charGet[50];
	strcpy(charGet, "");
	tostring(numgacha);
	strcat(charGet, out);
	strcat(charGet, "_characters_");
	strcat(charGet, rarityChar[RNGchar]);
	strcat(charGet, "_");
	strcat(charGet, nameChar[RNGchar]);
	strcat(charGet, "_");
	tostring(primo);
	strcat(charGet, out);
	strcat(charGet, "\n");
		
        FILE *out=fopen(txtPath,"a+");
	fprintf(out, "%s\n", charGet);
        fclose(out);
}
void weapToTxt(int RNGweap){
	int status;
	char weapGet[50];
	strcpy(weapGet, "");
	tostring(numgacha);
	strcat(weapGet, out);
	strcat(weapGet, "_weapons_");
	strcat(weapGet, rarityWeap[RNGweap]);
	strcat(weapGet, "_");
	strcat(weapGet, nameWeap[RNGweap]);
	strcat(weapGet, "_");
	tostring(primo);
	strcat(weapGet, out);
	strcat(weapGet, "\n");
	
	FILE *out=fopen(txtPath,"a+");
	fprintf(out, "%s\n", weapGet);
	fclose(out);
}
```
```c
primo = 79000, numgacha = 0;
		while(primo >= 160){
			printf("Current primo: %d\n", primo);
			primo-=160;
			if((numgacha % 90) == 0)changeFolder();
			if((numgacha % 10) == 0)changeTxt();
			numgacha++;
			if(numgacha % 2){
				charToTxt(rand() % charCount);
			}
			else{
				weapToTxt(rand() % weapCount);
			}
		}
```
Setelah `dataInit()` selesai, selanjutnya akan dilakukan gacha item dengan primogems yang di set 79000 dari soal dan gacha akan dilakukan sampai primogems habis dengan ketentuan sebagai berikut:
1. Tiap gacha memerlukan 160 primogems
2. Setiap jumlah gacha mod 90 akan dibuat folder baru dengan format nama `total_gacha_{jumlah-gacha}`
3. Setiap jumlah gacha mod 10 akan dibuat file .txt baru dengan format nama `{Hh:Mm:Ss}_gacha_{jumlah-gacha}`, tiap pergantian akan memiliki selisih 1 detik
4. Hasil gacha akan dimasukkan dalam file .txt dengan format `{jumlah-gacha}_[tipe-item]_{rarity}_{name}_{sisa-primogems}`, gacha ganjil akan mendapatkan tipe item characters dan gacha genap akan mendapatkan tipe item weapons
5. Seluruh operasi dilakukan dalam folder `gacha_gacha`
<br>
Untuk menyelesaikan poin 1 dapat menggunakan looping yang akan mengurangi primogems sebanyak 160 tiap loop, poin 2 menggunakan fungsi `changeFolder()` yang membuat folder baru sekaligus menyimpan path dari folder yang sekarang digunakan. Poin 3 menggunakan fungsi `changeTxt()` yang membuat .txt baru dalam folder sekarang dan menyimpan path dari .txt yang sekarang digunakan dan tiap pembuatan .txt akan dilakukan sleep selama 1 detik.<br>
Lalu `charToTxt()` dan `weapToTxt()` masing-masing akan digunakan untuk mengambil value dari data yang ada dengan bantuan fungsi `rand()` dan jumlah masing-masing item yang sebelumnya telah dicatat ketika menjalankan `saveFileName()`, lalu akan melakukan append pada .txt sekarang sesuai format.

```c
void tostring(int x){
	int len = 0;
	len++;
	int temp = x;
	while(temp>10){
	    temp /= 10;
	    len++;
	}
	temp = len;
	// fodder
	for(int i=0; i<len; i++)out[i] = 'a';
	while(temp--){
	    if(x)out[temp] = '0' + (x%10);
	    else out[temp] = '0' + x;
	    x/=10;
	}
	out[len] = '\0';
}
```

Selain itu, diperlukan adanya fungsi tambahan `tostring` yang berfungsi mengubah integer menjadi string, karena beberapa data seperti jumlah gacha dan primogems bertipe integer tetapi diperlukan dalam penamaan file dan folder.

### 1E
Terdapat kriteria tambahan yaitu gacha hanya akan dimulai pada anniversary yaitu 30 Maret jam 04:44 dan 3 jam setelah gacha dimulai semua isi di folder `gacha_gacha` akan di zip dengan nama `not_safe_for_wibu` dan dipassword `satuduatiga` lalu folder `gacha_gacha` akan didelete, sehingga untuk menyelesaikan 2E akan ditambahkan beberapa hal di luar fungsi utama gacha di `main()`
```c
void zipFile(){
	int status;
	if(fork()==0){
		char *argv[] = {"zip","-r","not_safe_for_wibu.zip","-P", "satuduatiga", "gacha_gacha", NULL};
		execv("/bin/zip", argv);
	}
	else while(wait(&status) > 0); 
}
void deleteFile(){
	int status;
	if(fork()==0){
		char *argv[] = {"rm", "-r", "gacha_gacha", NULL};
		execv("/bin/rm", argv);
	}
	else while(wait(&status) > 0);
}
int main(int argc, char* argv[]){

	time_t seconds;
	struct tm *timeStruct;
	seconds = time(NULL);
	// change localtime
	timeStruct = localtime(&seconds);
	while(1){
		printf("Time now: %d %d %d %d:%d:%d, wait anniversary!\n", timeStruct->tm_mday,
            timeStruct->tm_mon + 1, timeStruct->tm_year + 1900,
            timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
		if(timeStruct->tm_mday==30 && timeStruct->tm_mon + 1 == 3 && timeStruct->tm_hour == 4 && timeStruct->tm_min == 44)break;
	}

//FUNGSI UTAMA GACHA

	pid_t child_id;

	strcpy(dir, "");
	strcpy(dir, "/home/florentino"); //ganti sesuai nama user
	// default
	strcpy(folderPath, "");
	int status;
	child_id = fork();
	if (child_id < 0) {
		exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
	}
	if(child_id == 0){
		createDir();
	}
	else{
		while(wait(&status) > 0);
		dataInit();
		primo = 79000, numgacha = 0;
        clock_t begin = clock();
		while(primo >= 160){
			printf("Current primo: %d\n", primo);
			primo-=160;
			if((numgacha % 90) == 0)changeFolder();
			if((numgacha % 10) == 0)changeTxt();
			numgacha++;
			if(numgacha % 2){
				charToTxt(rand() % charCount);
			}
			else{
				weapToTxt(rand() % weapCount);
			}
		}
		printf("Primo habis, top up lagi ngab\n");
		printf("Total laksek: %d", numgacha);
		// gacha done

//FUNGSI UTAMA GACHA

		clock_t end = clock();
		if((end-begin)/CLOCKS_PER_SEC < 10800){
			printf("Wait %ld seconds..\n", 10800 - ((end-begin)/CLOCKS_PER_SEC));
			sleep(10800 - ((end-begin)/CLOCKS_PER_SEC));
		}
		pid_t child_id3;
		//status = NULL;
		child_id3 = fork();
		if (child_id3 < 0) {
			exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
		}
		if(child_id3 == 0){
			zipFile();
		}
		else{
			while(wait(&status) > 0);
			printf("Aduh ga dapet SSR :(\n");
			deleteFile();
		}			
	}
	return 0;
}
```
Beberapa diantaranya adalah program baru akan dijalankan pada tanggal anniversary dan selama belum maka program akan melakukan looping yang akan menunggu tanggal anniversary. Selanjutnya, ketika proses gacha dimulai timer `begin` akan mencatat waktu mulai dan ketika gacha selesai timer `end` akan mencatat waktu selesai sehingga ketika selisih begin dan end belum mencapai 3 jam maka program akan sleep sejumlah waktu yang kurang. Lalu setelah lewat 3 jam program akan menjalankan `zipFile()` yang akan melakukan zip pada folder `gacha_gacha` dengan nama .zip `not_safe_for_wibu` dan password `satuduatiga`. Selanjutnya akan dijalankan `deleteFile()` yang akan menghapus folder `gacha_gacha` dan isinya.

### Revisi
- fopen tidak perlu menggunakan fork
- isi .json terlalu besar sehingga buffer awal yang digunakan tidak cukup
- file harus diextract satu per satu karena jika tidak akan error



## Soal no 3  
### Conan adalah seorang detektif terkenal. Suatu hari, Conan menerima beberapa laporan tentang hewan di kebun binatang yang tiba-tiba hilang. Karena jenis-jenis hewan yang hilang banyak, maka perlu melakukan klasifikasi hewan apa saja yang hilang.  
  
**a) membuat program untuk membuat 2 directory di `/home/[USER]/modul2/` dengan nama “darat” lalu 3 detik kemudian membuat directory ke 2 dengan nama “air”**  
  
```c
void darat(){
    char *argv[] = {"mkdir", "-p", "/home/daniel/modul2/darat", NULL};
    execv("/bin/mkdir", argv);
}

void air(){
    char *argv1[] = {"mkdir", "/home/daniel/modul2/air", NULL};
    execv("/bin/mkdir", argv1);
}

void pertama() { // create air and darat folder

  pid_t child_id;
  int status;
   
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    darat();
  }
  else {
    // this is parent
    while ((wait(&status)) > 0);
    sleep(3);
    air();
  }
}
```  
Kode diatas menggunakan fork untuk mengeksekusi pembuatan `folder darat`**` terlebih dahulu disusul dengan **delay 3 detik** yang kemudian akan menciptakan `folder air`. 

**b) melakukan extract “animal.zip” di “/home/[USER]/modul2/”**

```c
void kedua(){ // extract animal.zip
    char *argv3[] = {"unzip","animal.zip","-d","/home/daniel/modul2", NULL};
    execv("/bin/unzip", argv3);
}

```  
Kemudian, kita diperlukan untuk melakukan ekstrak file `animal.zip` di directory yang sama dengan folder darat dan air yang telah kita buat.  
  
**c) Hasil extract dipisah menjadi hewan darat dan hewan air sesuai dengan nama filenya. Untuk hewan darat dimasukkan ke folder `/home/[USER]/modul2/darat` dan untuk hewan air dimasukkan ke folder `/home/[USER]/modul2/air`. Untuk hewan yang tidak ada keterangan air atau darat harus dihapus.**  
  
```c
int move_file(char *file_name, char *target_dir) {
        int ch;
        FILE *fp1, *fp2;
	printf("the file is %s move to %s\n", file_name, target_dir);

        /* open the source file in read mode */
        fp1 = fopen(file_name, "r");
        /* open the destination file in write mode */
        fp2 = fopen(target_dir, "w");

        /* error handling */
        if (!fp1) {
                printf("Unable to open source file to read!!\n");
                fclose(fp2);
                return 0;
        }

        if (!fp2) {
                printf("Unable to open target file to write\n");
                return 0;
        }

        /* copying contents of source file to target file */
        while ((ch = fgetc(fp1)) != EOF) {
                fputc(ch, fp2);
        }

        /* closing the opened files */
        fclose(fp1);
        fclose(fp2);

        /* removing the source file */
        remove(file_name);
  }

int ketiga() // separate air and darat
{
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul2/animal");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
            char move_darat[MAX] = "/home/daniel/modul2/darat/";
	        char move_air[MAX] = "/home/daniel/modul2/air/";
            char path[MAX] = "/home/daniel/modul2/animal/";
            strcat(path, dir->d_name);
	
			if (strstr(dir->d_name,"darat")) // ini darat
			{
			printf("file %s adalah DARAT!\n", dir->d_name);
			strcat(move_darat,dir->d_name);
			move_file(path,move_darat);
			printf("MOVE TO %s\n\n", move_darat);
			}
			else if (strstr(dir->d_name,"air")) // ini air
			{
			printf("file %s adalah AIR!\n", dir->d_name);
			strcat(move_air,dir->d_name);
			move_file(path,move_air);
			printf("MOVE TO %s\n\n", move_air);
			}
	
			else 
			{
			char path[MAX] = "/home/daniel/modul2/animal/";
			printf("FILE %s AKAN DIHAPUS!\n", dir->d_name);
			strcat(path,dir->d_name);
			printf("PATH PENGHAPUSAN ADALAH %s\n", path);
			remove(path);
			}
	
            }
        }
        closedir(d);
    }
    return(0);
}

```  
Pada kode diatas, kita akan menggunakan **directory listing** untuk melakukan iterasi file yang ada di directory tersebut. Kemudian jika ditemukan pola kalimat **darat** maupun **air** , file tersebut akan dipindahkan ke **folder darat atau air** yang telah dibuat tadi. Jika file tersebut tidak mengandung kata **darat** maupun **air**, maka file tersebut akan dihapus.  

**d) Menghapus semua burung yang ada di directory “/home/[USER]/modul2/darat”**  
  
```c
void keempat() // remove bird
	{
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul2/darat");
	if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
	    printf("file name : %s\n",dir->d_name);
	    if(strstr(dir->d_name,"bird")) {
	       printf("remove file %s.\n", dir->d_name);
	       char bird_path[MAX] = "/home/daniel/modul2/darat/";
	       strcat(bird_path,dir->d_name);
	       remove(bird_path);
	       }
	    }
        }
    }
    closedir(d);
    }
```  
Seperti kode sebelumnya, kode diatas melakukan iterasi file di **folder darat** untuk mencari file yang mengandung kata **bird** yang nantinya file tersebut akan dihapus.  
  
**e) Membuat list nama semua hewan yang ada di directory “/home/[USER]/modul2/air” ke “list.txt” dengan format UID_[UID file permission]_Nama File.[jpg/png]**  
  
```c
void check_permission(char *filename, char *nama_file, FILE* fptr)
{
    char format[MAX] = "";
    memset(format,0,strlen(format));
    struct stat info;
    int r1;

    r1 = stat(filename, &info);
    if( r1==-1 )
    {
        fprintf(stderr,"File error\n");
        exit(1);
    }

    struct passwd *pw = getpwuid(info.st_uid);
    struct group  *gr = getgrgid(info.st_gid);

    strcat(format, pw->pw_name);
    strcat(format,"_");

    struct stat fs;
    int r;

    r = stat(filename,&fs);
    if( r==-1 )
    {
        fprintf(stderr,"File error\n");
        exit(1);
    }

    /* file permissions are kept in the st_mode member */
    if( fs.st_mode & S_IRUSR ) strcat(format,"r");
    if( fs.st_mode & S_IWUSR ) strcat(format,"w");
    if( fs.st_mode & S_IXUSR ) strcat(format,"x");

    strcat(format,"_");
    strcat(format,nama_file);
    strcat(format,"\n");
    printf("format akhir : %s", format); 
    fprintf(fptr, "%s", format);
}

void kelima() // create list.txt in the air folder
{
    FILE* file_ptr = fopen("/home/daniel/modul2/air/list.txt", "w");

    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul2/air");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
            char path[MAX] = "/home/daniel/modul2/air/";
            strcat(path, dir->d_name);
            check_permission(path,dir->d_name, file_ptr);
            }
        }
        closedir(d);
        fclose(file_ptr);
    }
}
```  
Pada kode diatas akan dilakukan iterasi kepada seluruh file di folder bird untuk menentukan format penamaan berdasarkan `UID` yang dapat berupa `r` , `w`, maupun `x` yang dieksekusi di fungsi `check_permission`. Selain itu juga perlu didapatkan nama user dan nama file tersebut.  
  
Berdasarkan uraian kode diatas, dihasilkan kode utuh seperti berikut.  
  
```c
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <stdio.h>
#include<dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#define MAX 256

void check_permission(char *filename, char *nama_file, FILE* fptr)
{
    char format[MAX] = "";
    memset(format,0,strlen(format));
    struct stat info;
    int r1;

    r1 = stat(filename, &info);
    if( r1==-1 )
    {
        fprintf(stderr,"File error\n");
        exit(1);
    }

    struct passwd *pw = getpwuid(info.st_uid);
    struct group  *gr = getgrgid(info.st_gid);

    strcat(format, pw->pw_name);
    strcat(format,"_");

    struct stat fs;
    int r;

    r = stat(filename,&fs);
    if( r==-1 )
    {
        fprintf(stderr,"File error\n");
        exit(1);
    }

    /* file permissions are kept in the st_mode member */
    if( fs.st_mode & S_IRUSR ) strcat(format,"r");
    if( fs.st_mode & S_IWUSR ) strcat(format,"w");
    if( fs.st_mode & S_IXUSR ) strcat(format,"x");

    strcat(format,"_");
    strcat(format,nama_file);
    strcat(format,"\n");
    printf("format akhir : %s", format); 
    fprintf(fptr, "%s", format);
}

void kelima() // create list.txt in the air folder
{
    FILE* file_ptr = fopen("/home/daniel/modul2/air/list.txt", "w");

    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul2/air");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
            char path[MAX] = "/home/daniel/modul2/air/";
            strcat(path, dir->d_name);
            check_permission(path,dir->d_name, file_ptr);
            }
        }
        closedir(d);
        fclose(file_ptr);
    }
}

  void keempat() // remove bird
	{
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul2/darat");
	if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
	    printf("file name : %s\n",dir->d_name);
	    if(strstr(dir->d_name,"bird")) {
	       printf("remove file %s.\n", dir->d_name);
	       char bird_path[MAX] = "/home/daniel/modul2/darat/";
	       strcat(bird_path,dir->d_name);
	       remove(bird_path);
	       }
	    }
        }
    }
    closedir(d);
    }

  int move_file(char *file_name, char *target_dir) {
        int ch;
        FILE *fp1, *fp2;
        //char source[MAX], target[MAX];
	printf("the file is %s move to %s\n", file_name, target_dir);

        /* open the source file in read mode */
        fp1 = fopen(file_name, "r");
        /* open the destination file in write mode */
        fp2 = fopen(target_dir, "w");

        /* error handling */
        if (!fp1) {
                printf("Unable to open source file to read!!\n");
                fclose(fp2);
                return 0;
        }

        if (!fp2) {
                printf("Unable to open target file to write\n");
                return 0;
        }

        /* copying contents of source file to target file */
        while ((ch = fgetc(fp1)) != EOF) {
                fputc(ch, fp2);
        }

        /* closing the opened files */
        fclose(fp1);
        fclose(fp2);

        /* removing the source file */
        remove(file_name);
  }


int ketiga() // separate air and darat
{
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul2/animal");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
            char move_darat[MAX] = "/home/daniel/modul2/darat/";
	    char move_air[MAX] = "/home/daniel/modul2/air/";
            char path[MAX] = "/home/daniel/modul2/animal/";
            strcat(path, dir->d_name);
	
	    if (strstr(dir->d_name,"darat")) // ini darat
	       {
	       printf("file %s adalah DARAT!\n", dir->d_name);
	       strcat(move_darat,dir->d_name);
	       move_file(path,move_darat);
	       printf("MOVE TO %s\n\n", move_darat);
	       }
	    else if (strstr(dir->d_name,"air")) // ini air
	       {
	       printf("file %s adalah AIR!\n", dir->d_name);
	       strcat(move_air,dir->d_name);
	       move_file(path,move_air);
	       printf("MOVE TO %s\n\n", move_air);
	       }
	
	    else 
	       {
	       char path[MAX] = "/home/daniel/modul2/animal/";
	       printf("FILE %s AKAN DIHAPUS!\n", dir->d_name);
	       strcat(path,dir->d_name);
	       printf("PATH PENGHAPUSAN ADALAH %s\n", path);
	       remove(path);
	       }
	
            }
        }
        closedir(d);
    }
    return(0);
}

void darat(){
    char *argv[] = {"mkdir", "-p", "/home/daniel/modul2/darat", NULL};
    execv("/bin/mkdir", argv);
}

void air(){
    char *argv1[] = {"mkdir", "/home/daniel/modul2/air", NULL};
    execv("/bin/mkdir", argv1);
}

void kedua(){ // extract animal.zip
    char *argv3[] = {"unzip","animal.zip","-d","/home/daniel/modul2", NULL};
    execv("/bin/unzip", argv3);
}

void pertama() { // create air and darat folder

  pid_t child_id;
  int status;
   
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    darat();
  }
  else {
    // this is parent
    while ((wait(&status)) > 0);
    sleep(3);
    air();
  }
}

void ab(){ // a,b process
  pid_t child_id;
  int status;
   
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); 
  }

  if (child_id == 0) {
    // this is child
    pertama();
  }
  else {
    while ((wait(&status)) > 0);
    kedua();
  }
}

void abc(){ // a,b,c process
  pid_t child_id;
  int status;
   
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); 
  }

  if (child_id == 0) {
    // this is child
    ab();
  }
  else {
    while ((wait(&status)) > 0);
    ketiga();
  }
}

void abcd(){ //a,b,c,d process
  pid_t child_id;
  int status;
   
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    abc();
  }
  else {
    while ((wait(&status)) > 0);
    keempat();
  }
}


int main(){
  pid_t child_id;
  int status;
   
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    abcd();
  }
  else {
    while ((wait(&status)) > 0);
    kelima();
  }
  return 0;
}
```
  
### Kendala  
Tidak ada.  
