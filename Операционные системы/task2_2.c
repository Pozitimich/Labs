#include <stdio.h>
#include <unistd.h>

int main() {
    fork();
    //Процесс раздваивается
    printf("Hi, id = %d, pid = %d\n", getpid(), getppid());
    int id = fork();
    //Родительский и дочерний процессы раздваиваются
    if (id == 0)
    printf("Hi, id = %d, pid = %d\n", getpid(), getppid());
    return 0;

    /*
    
    */
}