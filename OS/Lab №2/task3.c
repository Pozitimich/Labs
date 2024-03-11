#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define phil_number 5
sem_t waiter;

typedef struct
{
    int id;
    pthread_mutex_t* left_fork;
    pthread_mutex_t* right_fork;
    
} philosopher;

void* philosopher_act(void* current_philosopher)
{
    philosopher* me = (philosopher*) current_philosopher;
    while(1)
    {
        sem_wait(&waiter);
        pthread_mutex_lock(me->left_fork);
        printf("Философ %d взял левую вилку\n", (*me).id);
        pthread_mutex_lock(me->right_fork);
        printf("Философ %d взял правую вилку\n", (*me).id);
        sem_post(&waiter);
        printf("Философ %d ест\n", (*me).id);
        printf("Философ %d положил левую вилку\n", (*me).id);
        pthread_mutex_unlock(me->left_fork);
        printf("Философ %d положил правую вилку\n", (*me).id);
        pthread_mutex_unlock(me->right_fork);
        sleep(0.5);
    }
    return NULL;
}

int main() 
{
    philosopher philosophers[phil_number];
    pthread_t phil_threads[phil_number];

    sem_init(&waiter, 0, phil_number-1);


    for(int i = 0; i < phil_number; ++i)
    {
        philosophers[i].id = i;
        philosophers[i].left_fork = malloc(sizeof(pthread_mutex_t));
        philosophers[(i+1)%phil_number].right_fork = philosophers[i].left_fork;
        pthread_mutex_init(philosophers[i].left_fork, NULL);
    }
    for(int i = 0; i < phil_number; ++i)
    {
        pthread_create(&(phil_threads[i]), NULL, philosopher_act, (void*) &(philosophers[i]));
    }
    for(int i = 0; i < phil_number; ++i)
    {
        pthread_join(phil_threads[i], NULL);
    }
    for(int i = 0; i < phil_number; ++i)
    {
        pthread_mutex_destroy(philosophers[i].left_fork);
        free(philosophers[i].left_fork);
    }
    sem_destroy(&waiter);
    return 0;
}
