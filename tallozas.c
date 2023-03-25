#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>


#define HOME_ENV_VARIABLE "HOME"
#define FLAG_INITIALIZED 0x01
#define FLAG_FILE_SELECTED 0x02


int main(){

    unsigned int state = FLAG_INITIALIZED;
    const char * home_path = getenv(HOME_ENV_VARIABLE);
    DIR *d;
    struct dirent *entry;
    char cwd[2048];
    char selected_file[1024];
    struct stat inode;

    chdir(home_path);
    while((state & FLAG_FILE_SELECTED) == 0){
        system("clear");
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf("Current directory:\n\t%s\n", cwd);
            d = opendir(".");
            printf("List files:\n");
            while((entry = readdir(d)) != NULL){
                if((*entry).d_name[0] != '.'){ //Ha nem rejtett fajl/mappa
                    printf("\t%s\n",(*entry).d_name);
                }
            }
            printf("Select file: ");
            fgets(selected_file, sizeof(selected_file), stdin);
            selected_file[strlen(selected_file)-1] = '\0';
            int tmp = stat(selected_file, &inode);
            if(tmp == 0){
                if(inode.st_mode & S_IFDIR){
                    chdir(selected_file);
                }
                if(inode.st_mode & S_IFREG){
                    state |= FLAG_FILE_SELECTED;
                }
            }
        }
    }
    printf("You selected: %s", selected_file);

//     DIR *d;
//     struct dirent * entry;
//     d = opendir(".");
    
//     closedir(d);

//     printf("\nCheck:");
//     system("ls");
    return 0;
}