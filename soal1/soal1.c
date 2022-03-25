#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>


char dir[40], folderPath[50], gachaRes[100], txtPath[100];
char fileChar[50][50], fileWeap[135][50], nameChar[50][50], nameWeap[135][50], rarityChar[50][10], rarityWeap[135][10];
int charCount = 0, weapCount = 0;
int primo = 0, numgacha = 0;
char out[100];
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
void resetResult(){
	strcpy(gachaRes, "");
}
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

	clock_t begin = clock();
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
