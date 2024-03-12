#include <stdio.h>
#include<dlfcn.h>

int main() {

    void *handler=dlopen("./libcollatz_library.so",RTLD_LAZY);
    if(!handler){printf("error with library opening\n"); return 0;}

    int (*collatz)(int, int);
    collatz = dlsym(handler,"test_collatz_convergence");
    if(dlerror()!=0){printf("error of function\n"); return 0;}

    int testnumbers[5] = {5,9,13,22,54};
    printf("Using static or shared library implementation. Limit of interations is 20\n");
    for(int i=0;i<5;i++){
        printf("Number %d, iterations: %d\n", testnumbers[i], collatz(testnumbers[i],20));
    }
    dlclose(handler);

    return 0;
}