#include <stdio.h>

extern int collatz_conjecture(int);
extern int test_collatz_convergence(int, int);

int main() {
    int testnumbers[5] = {5,9,13,22,54};
    printf("Using static or shared library implementation. Limit of interations is 20\n");
    for(int i=0;i<5;i++){
        printf("Number %d, iterations: %d\n", testnumbers[i], test_collatz_convergence(testnumbers[i],20));
    }
    return 0;
}
