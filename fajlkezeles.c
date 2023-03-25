#include <stdio.h>
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
    FILE *f = fopen("szamok.txt", "w");
    for(int i=0; i<1000; ++i){
        fprintf(f, "%lf ", numbers[i]);
    }
    fclose(f);

    //beolvasni a 821-ik szamot.
    return 0;
}