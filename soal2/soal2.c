#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void buatdir(char *name){
    char *argv[] = {"mkdir","-p", name,NULL};
    execv("/bin/mkdir",argv);
}

void buattxt (char *name){
    printf("stuck");
    char path[]="";
    strcpy(path,name);
    strcat(path,"/data.txt");
    char *argv[] = {"touch",path,NULL};
    execv("/bin/touch",argv);
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void unzip(){
    char *argv[] = {"unzip","drakor.zip","-d","/home/soreta/shift2/drakor","*.png", NULL};
    execv("/bin/unzip",argv); 
}

void copyfile(char* namefile, char* genre,char *truenamefile,int indextruename, char *tahun){
   pid_t child_id;
   int status;
   child_id = fork();
   if(child_id<0){
       exit(EXIT_FAILURE);
   }
   if(child_id==0){
        char filename[]="";
        strcpy(filename,genre);
        strcat(filename,"/data.txt");
      //  printf("filename: %s\n",filename);
        FILE *fp;
        if(exists(filename)){
            fp = fopen(filename,"a");
            if (fp == NULL){
                exit(EXIT_FAILURE);
            }
            fprintf(fp,"nama : %s\n",truenamefile);
            fprintf(fp,"rilis : tahun %s\n\n",tahun);
            fclose(fp);
        }else{
            fp = fopen(filename,"w");
            if(fp == NULL){
                exit(EXIT_FAILURE);
            }
            fprintf(fp,"kategori : %s\n\n",genre);
            fprintf(fp,"nama : %s\n",truenamefile);
            fprintf(fp,"rilis : tahun %s\n\n",tahun);
            fclose(fp);
            }
   }else{
        while(wait(&status)<0);
        child_id = fork();
        if(child_id<0){
            exit(EXIT_FAILURE);
        }
        if(child_id==0){
            printf("ini copyfile %s %s\n\n",namefile,genre);
            char *argv[] = {"cp",namefile,genre,NULL };
            execv("/bin/cp",argv);
        }else{
            while(wait(&status)<0);
            child_id=fork();
            if(child_id<0){
                exit(EXIT_FAILURE);
            }
            if(child_id==0){
                char *buf;
                buf=(char *)malloc(100*sizeof(char));
                getcwd(buf,100);
                if(chdir(genre)<0){
                    //  printf("genre");
                    exit(EXIT_FAILURE);
                }
                buf=(char *)malloc(100*sizeof(char));
                getcwd(buf,100);
                truenamefile[indextruename]='.';
                indextruename++;
                truenamefile[indextruename]='p';
                indextruename++;
                truenamefile[indextruename]='n';
                indextruename++;
                truenamefile[indextruename]='g';
                indextruename++;
                printf("ini ganti nama %s %s genre: %s directory: %s\n\n", namefile,truenamefile,genre,buf);
                char *argv[] = {"mv",namefile,truenamefile,NULL};
                execv("/bin/mv",argv);
            }
        }
    }
}

void openfolder(){
    pid_t child_id;
    int status;
    if((chdir("/home/soreta/shift2/drakor"))<0){
            exit(EXIT_FAILURE);
    }else{
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d) {
            while ((dir = readdir(d))) {
                if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
                    //printf("%s\n", dir->d_name);
                    //printf("length of string: %ld\n",strlen(dir->d_name));
                    int banyaktitikkoma=0;
                    char genre[255]="";
                    char nama[255]="";
                    char tahun[255]="";
                    int indexnama=0;
                    int indextahun=0;
                    for(int i = 0 ; i < strlen(dir->d_name);i++){
                        while(wait(&status)>0);
                        //printf("testing %c\n",dir->d_name[i]);
                        if(dir->d_name[i]==';'){
                            banyaktitikkoma++;
                            i++;
                        }
                        if(banyaktitikkoma==0){
                            nama[indexnama]=dir->d_name[i];
                            //printf("nama:%c  namafile:%c\n",nama[indexnama],dir->d_name[i]);
                            indexnama++;
                        }
                        if(banyaktitikkoma==1){
                        //   i++;
                            tahun[indextahun]=dir->d_name[i];
                            indextahun++;
                        }
                        if(banyaktitikkoma==2){
                            int indexgenre=0;
                            while(dir->d_name[i] != '_' && dir->d_name[i] != '.'){
                                genre[indexgenre]=dir->d_name[i];
                            // printf ("%d %d\n", i,index);
                            // strncat(name,&dir->d_name[i],1);
                            // printf("%c\n",dir->d_name[i]);
                            //  printf("genre:%c dir:%c\n",genre[indexgenre],dir->d_name[i]);
                                i++;
                                indexgenre++;
                            }
                            banyaktitikkoma=0;
                            //  int genrelen = strlen(name);
                            //  printf("nama : %s  tahun: %s genre : %s\n",nama,tahun,genre);
                            child_id=fork();
                            if(child_id < 0) {
                                exit(EXIT_FAILURE);
                            }
                            if(child_id==0){
                                //this is child
                                buatdir(genre);
                            }else{
                                while(wait(&status)<0);
                                copyfile(dir->d_name,genre,nama,indexnama,tahun);
                                
                            }
                            memset(nama,0,sizeof(nama));
                            memset(genre,0,sizeof(genre));
                            memset(tahun,0,sizeof(tahun));
                            indexnama=0;
                            indextahun=0;
                        }
                    }
                }
            }
            closedir(d);
        }
    }    
}
int main(){
    pid_t child_id;
    int status;
    child_id = fork();
    if(child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if(child_id==0){
        unzip();
    }else{
        while(wait(&status)>0);
        openfolder();
    }
}