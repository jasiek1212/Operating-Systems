#include<stdio.h>

int collatz_conjecture(int input){
    if(input%2==0){
        return input/2;
    }
    return 3*input+1;
}

int test_collatz_convergence(int input, int max_iter){
    int number = input;
    int index = 0;
    while(number > 1){
        if(index==max_iter){
            return -1;
        }
        number = collatz_conjecture(number);
        index += 1;
    }
    return index;
}