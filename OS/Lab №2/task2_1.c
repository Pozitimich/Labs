#include <stdio.h>
#include <unistd.h>

int main() 
{
    int pid =1;
    pid=fork();
    printf("%d\n", pid);
    return 0;
    /*
    Переменная pid для родительского процесса принимает значение id дочернего,
    для дочернего процесса она принимает значение 0.
    */
}