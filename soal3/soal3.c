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
            //printf("\n%s\n", dir->d_name);
            char move_darat[MAX] = "/home/daniel/modul2/darat/";
	    char move_air[MAX] = "/home/daniel/modul2/air/";
            char path[MAX] = "/home/daniel/modul2/animal/";
            //strcat(move,dir->d_name);
            strcat(path, dir->d_name);
            //printf("from %s\n", path);
            //printf("move to %s\n\n", move);
            //move_file(path, move);
	
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

/*
void ketiga(){
    //bash command nya
    //find . -type f -name "*darat*" -exec mv '{}' /home/daniel/modul2/darat/ \;
}
*/

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
