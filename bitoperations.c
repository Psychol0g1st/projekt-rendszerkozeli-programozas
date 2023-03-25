#include <stdio.h>

// void kiirbiteket(int num){
//     for(int i=0; i < sizeof(int)*8; ++i){
//         printf("%i", num & 1);
//         num = num >> 1;
//     }
//     puts("");
//  }
// void kiirbiteket(unsigned int num){
//     for(int i=sizeof(int)*8-1; i >= 0; --i){
//         printf("%i", num & 1 << i ? 1 : 0);
//     }
//     puts("");
//  }
void kiirbiteket(int num){
    unsigned int M = 1 << (sizeof(unsigned int)*8-1);
    while(M){
        printf("%d", num & M ? 1 : 0);
        M >>= 1;
    }
    puts("");
}

int increment (int num){
    unsigned int M = 1 ;
    while(num & M){
        num &= ~M;
        M <<= 1;
    }
    return num | M;
}

// int ellentetes (int num){
//     return ~(num)+1; 
// }
int ellentetes (int num){
    return increment(~num);
}
int sum (int a, int b){
    unsigned int M = 1;
    int carry_bit = 0;
    int output = 0;
    while(M){
        output = ((a & M) ^ (b & M)) ^ carry_bit;
        carry_bit = ((a & M) & (b & M)) | (((a & M) ^ (b & M)) & carry_bit);
        M <<= 1;
    }
    return output;
}

int main()
{
    // kiirbiteket(1);
    // kiirbiteket(2);
    // kiirbiteket(0x80000000);
    // kiirbiteket(0xFFFFFFFF);
    // kiirbiteket(4);
    // printf("%d\n", ellentetes(1));
    printf("%d\n", sum(1,1));
    
    return 0;
}
