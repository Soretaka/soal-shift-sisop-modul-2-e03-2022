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
           // printf("ini copyfile %s %s\n\n",namefile,genre);
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
               // printf("ini ganti nama %s %s genre: %s directory: %s\n\n", namefile,truenamefile,genre,buf);
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
void sort_data(){
    if((chdir("/home/soreta/shift2/drakor"))<0){
        exit(EXIT_FAILURE);
    }else{
        char allgenre[255][255]={""};
        int indexgenre=0;
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d) {
            while ((dir = readdir(d))) {
                int directory=1;
                for(int i = 0 ; i < strlen(dir->d_name);i++){
                    if(dir->d_name[i] == '.'){
                        directory=0;
                        break;
                    }
                }
                if(directory){
                //    printf("%s\n",dir->d_name);
                    strcpy(allgenre[indexgenre],dir->d_name);
                    indexgenre++;
                }
            }
        closedir(d);
        }
       char *buf;
       buf=(char *)malloc(100*sizeof(char));
       getcwd(buf,100);
        for(int i = 0 ; i < indexgenre ; i++){
            char allnama[255][255];
            int alltahun[255];
 //           char alltahun[255][255];
            int indexitem=0;
            memset(allnama,0,sizeof(allnama));
            memset(alltahun,0,sizeof(alltahun));
            int indexline=1;
            char *path;
            path=(char *)malloc(255*sizeof(char));
            memset(path,0,sizeof(path));
            strcpy(path,buf);
            strcat(path,"/");
             strcat(path,allgenre[i]);
             strcat(path,"/data.txt");
             printf("%s\n",allgenre[i]);
            //printf("%s\n",path);
            FILE* fp;
            char * line=NULL;
            size_t len=0;
            ssize_t read;
            fp=fopen(path,"r");
            if(fp==NULL)exit(EXIT_FAILURE);
            while((read=getline(&line,&len,fp))!= -1){
                if(indexline==1) {
                    indexline++;
                    continue;
                }
           //     printf("Retrieved line of length %zu:\n", read);
                char* nama;
                nama = (char *)malloc(255*sizeof(char));
                char* tahun;
                tahun = (char *)malloc(255*sizeof(char));
                printf("%s\n", line);
                memset(nama,0,sizeof(nama));
                memset(tahun,0,sizeof(tahun));
                if(indexline%3==0){
                    int idx=0;
                    for(int j = 7 ; j < strlen(line);j++){
                        strncat(nama,&line[j],1);
                    }
                 //   printf("actual: %s\n",line);
                //    printf("nama : %s\n",nama);
                    strcpy(allnama[indexitem],nama);
                    memset(nama,0,sizeof(nama));
                   printf("%s\n",allnama[indexitem]);
                }
                if(indexline%3==1){
                    int idx=0;
                    for(int j = 14 ; j < strlen(line);j++){
                        strncat(tahun,&line[j],1);
                    }
                    int tahunInt=0;
                    for(int z=0 ; z <strlen(tahun)-1;z++){
                        int pengali=1;
                        for(int m=0; m< strlen(tahun)-2-z;m++){
                            pengali*=10;
                        }
                       // printf("%d ",tahun[z]-'0');
                        tahunInt+=(tahun[z]-'0')*pengali;
                    }
                 //   strcpy(alltahun[indexitem],tahun);
                 //   memset(tahun,0,sizeof(tahun));
                    alltahun[indexitem]=tahunInt;
                    printf("%d\n",alltahun[indexitem]);
                    indexitem++;
                }
                indexline++;
           }
            fclose(fp);
            // for(int j = 0 ; j < indexitem;j++){
            // printf("%s\n",allnama[j]);
            // }   
            if (line) free(line);

            for(int i = 0 ; i < indexitem;i++){
                for(int j = 0 ; j < indexitem-i-1; j++){
                    if(alltahun[j] > alltahun[j+1]){
                        int tmp = alltahun[j];
                        alltahun[j] = alltahun[j+1];
                        alltahun[j+1]=tmp;
                        char* namatmp;
                        namatmp=(char *)malloc(255*sizeof(char));
                        strcpy(namatmp,allnama[j]);
                        memset(allnama[j],0,sizeof(allnama[j]));
                        strcpy(allnama[j],allnama[j+1]);
                        memset(allnama[j+1],0,sizeof(allnama[j+1]));
                        strcpy(allnama[j+1],namatmp);
                    }
                }
            }
            for(int i = 0 ; i < indexitem ; i++){
                printf("nama: %s\ntahun: %d\n",allnama[i],alltahun[i]);
            }
            fp = fopen(path,"w");
            fprintf(fp,"kategori: %s\n\n",allgenre[i]);
            fclose(fp);
            fp=fopen(path,"a");
            for(int i = 0 ; i < indexitem;i++){
                fprintf(fp,"nama : %s",allnama[i]);
                fprintf(fp,"rilis : tahun %d\n\n", alltahun[i]);
            }
            fclose(fp);
        } //akhir dari looping per genre
        // printf("%s\n",buf);
        
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
        child_id=fork();
        if(child_id<0){
            exit(EXIT_FAILURE);
        }
        if(child_id==0){
           openfolder();
        }else{
            while(wait(&status)>0);
            sort_data();
        }
    }
}