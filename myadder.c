#include <stdio.h>
#include <stdlib.h>

int main(int argc , char *argv[]) {
    int num1,num2;
    if(argc != 3) {
        printf("invalid number of arguments\n");
        exit(1);
    }
    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);

    printf("Sum = %d\n",num1+num2);
    return 0;
}