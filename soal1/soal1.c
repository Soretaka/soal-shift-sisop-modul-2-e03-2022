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


char dir[12], folderPath[50], gachaRes[100], txtPath[100];
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
	    if(x)out[temp] = '0' + x%10;
	    else out[temp] = '0' + x;
	    x/=10;
	}
	out[len] = '\0';
}
void extractFile(){
	if(fork()==0){
		char *argv[] = {"unzip", "char.zip", "-d", dir, NULL};
		execv("/bin/unzip", argv);
	}
	else{
		char *argv2[] = {"unzip", "weap.zip", "-d", dir, NULL};
		execv("/bin/unzip", argv2);
	}
}
void downloadFile(){
	if(fork()==0){
		char *argv[] = {"wget", "-P", dir, "-q", "drive.google.com/uc?export=download&id=1xYYmsslb-9s8-4BDvosym7R4EmPi6BHp", "-O", "char.zip"};
		execv("/bin/wget", argv);
	}
	else{
		char *argv2[] = {"wget", "-P", dir, "-q", "drive.google.com/uc?export=download&id=1XSkAqqjkNmzZ0AdIZQt_eWGOZ0eJyNlT", "-O", "weap.zip"};
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
		char *argv[] = {"mkdir", "gacha_gacha", dir, NULL};
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
	if(fork()==0){
		// char file names
		DIR *d;
		struct dirent *dir;
		d = opendir("./characters/");
		if (d) {
			while ((dir = readdir(d)) != NULL) {
				strcpy(fileChar[charCount],dir->d_name);
				charCount++;
			}
		closedir(d);
		}
	}
	else{
		// weap file names
		DIR *d;
		struct dirent *dir;
		d = opendir("./weapons");
		if (d) {
			while ((dir = readdir(d)) != NULL) {
				strcpy(fileWeap[weapCount],dir->d_name);
				weapCount++;
			}
		closedir(d);
		}
	}
}
void saveDataValues(){
	if(fork()==0){
		// save char values to arr
		FILE *fp;
		char buffer[1024];
		struct json_object *parsed_json;
		struct json_object *name;
		struct json_object *rarity;
		for(int i=0; i<charCount; i++){
			fp = fopen(fileChar[i], "r");
			fread(buffer, 1024, 1, fp);
			fclose(fp);
			
			parsed_json = json_tokener_parse(buffer);
			
			json_object_object_get_ex(parsed_json, "name", &name);
			json_object_object_get_ex(parsed_json, "rarity", &rarity);
			
			strcpy(nameChar[i], json_object_get_string(name));
			strcpy(rarityChar[i], json_object_get_string(rarity));
		}
	}
	else{
		// save weap values to arr
		FILE *fp;
		char buffer[1024];
		struct json_object *parsed_json;
		struct json_object *name;
		struct json_object *rarity;
		for(int i=0; i<weapCount; i++){
			fp = fopen(fileWeap[i], "r");
			fread(buffer, 1024, 1, fp);
			fclose(fp);
			
			parsed_json = json_tokener_parse(buffer);
			
			json_object_object_get_ex(parsed_json, "name", &name);
			json_object_object_get_ex(parsed_json, "rarity", &rarity);
			
			strcpy(nameWeap[i], json_object_get_string(name));
			strcpy(rarityWeap[i], json_object_get_string(rarity));
		}
	}
}
void dataInit(){
	pid_t child_id;
	child_id = fork();
	int status;
	if (child_id < 0) {
		exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  	}
	if(child_id == 0){
		saveFileName();
	}
	else{
		saveDataValues();
	}
}
void resetResult(){
	strcpy(gachaRes, "");
}
void changeFolder(){
	char folderName[30];
	int status;
	if(fork()==0){
		strcpy(folderPath, "");
		strcat(folderPath, dir);
		strcat(folderPath, "/gacha_gacha");
		
		strcpy(folderName, "");
		strcat(folderName, "/total_gacha_");
		tostring(numgacha);
		strcat(folderName, out);
		
		strcat(folderPath, folderName);
		char *argv[] = {"mkdir", "-p", folderPath, NULL};
		execv("/bin/mkdir", argv);
	}
	else {
		while(wait(&status) > 0);
		//strcat(folderPath, folderName);
	}
}
void changeTxt(){
	char txtName[40];
	int status;
	if(fork()==0){
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
		
		strcat(txtPath, txtName);
		char *argv[] = {"mkdir", "-p", txtPath, NULL};
		execv("/bin/mkdir", argv);
	}
	else {
		while(wait(&status) > 0);
		// wait 1 sec
		sleep(1);
	}
}
void charToTxt(int RNGchar){
	int status;
	char charGet[50];
	if(fork()==0){
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
		
		FILE *out=fopen(txtPath,"w+");
		fputs(charGet,out);
		fclose(out);
	}
	else while(wait(&status) > 0);
}
void weapToTxt(int RNGweap){
	int status;
	char weapGet[50];
	if(fork()==0){
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
		
		FILE *out=fopen(txtPath,"w+");
		fputs(weapGet,out);
		fclose(out);
	}
	else while(wait(&status) > 0);
}

int main(int argc, char* argv[]){

	pid_t child_id;

	strcpy(dir, "");
	strcpy(dir, "/home/user");
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
		pid_t child_id2;
		//status = NULL;
		child_id2 = fork();
		if (child_id2 < 0) {
		exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
		}
		if(child_id2 == 0){
			dataInit();
		}
		else{
			while(wait(&status) > 0);
			primo = 79000, numgacha = 0;
			while(primo >= 160){
				primo-=160;
				resetResult();
				if(numgacha % 90 == 0)changeFolder();
				if(numgacha % 10 == 0)changeTxt();
				numgacha++;
				if(numgacha % 2){
					charToTxt(rand() % charCount);
				}
				else{
					weapToTxt(rand() % weapCount);
				}
			}
			// gacha done
		}
	}
	return 0;
}
