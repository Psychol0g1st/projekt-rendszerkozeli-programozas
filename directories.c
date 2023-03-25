#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    DIR *d;
    struct dirent * entry;
    d = opendir(".");
    while((entry = readdir(d)) != NULL){
        if((*entry).d_name[0] != '.'){ //Ha nem rejtett fajl/mappa
            puts((*entry).d_name);
        }
    }
    closedir(d);

    printf("\nCheck:");
    system("ls");
    return 0;
}