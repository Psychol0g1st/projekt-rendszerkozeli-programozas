#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>


int main(){
    srand(1);
    
    const float max = 1000000.0;
    float numbers[1000];
    for(int i=0; i<1000; ++i){
        numbers[i] = ((float)rand()/(float)(RAND_MAX)) * max;
        printf("%lf\n",numbers[i]);
    }

    int f = open("nums.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR | S_IRUSR);
    if(f < 0){
        write(2, "File Error!", 11);
        return -1;
    }

    write(f, numbers, 1000*sizeof(float));

    close(f);

    f = open("nums.txt", O_RDONLY);
    float read_numbers[1000];
    int n = read(f, read_numbers, sizeof(read_numbers));
    printf("%d\n", n);
    printf("%lf\n", read_numbers[0]);
    close(f);

    return 0;
}