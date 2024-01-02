#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("Process ID: %d\n", getpid());
    printf("Parent process ID: %d\n", getppid());
    printf("Group ID: %d\n", getpgrp());
    printf("Real user ID: %d\n", getuid());
    printf("Real user group ID: %d\n", getgid());
    printf("Effective user ID: %d\n", geteuid());
    printf("Effective user group ID: %d\n", getegid());
    return 0;
}