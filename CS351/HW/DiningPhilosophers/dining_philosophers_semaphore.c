// Safe Gergis
// Dining Philosophers using semaphores

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t chopsticks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
int philosopher_ids[NUM_PHILOSOPHERS];

// Philosopher thread function
void *philosopher(void *num)
{
    int id = *(int *)num;
    while (1)
    {
        printf("P#%d THINKING.\n", id);
        // Simulate thinking
        usleep(rand() % 100000);

        if (id % 2 == 0)
        {
            // Even philosophers pick up left then right
            sem_wait(&chopsticks[id]); // Left chopstick
            printf("P#%d picked up left chopstick.\n", id);

            sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]); // Right chopstick
            printf("P#%d picked up right chopstick.\n", id);
        }
        else
        {
            // Odd philosophers pick up right then left
            sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]); // Right chopstick
            printf("P#%d picked up right chopstick.\n", id);
            sem_wait(&chopsticks[id]); // Left chopstick
            printf("P#%d picked up left chopstick.\n", id);
        }

        // Eat
        printf("P#%d EATING.\n", id);
        usleep(rand() % 100000);

        // Put down chopsticks
        sem_post(&chopsticks[id]);
        printf("P#%d put down left chopstick.\n", id);

        sem_post(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);
        printf("P#%d put down right chopstick.\n", id);

        printf("P#%d finished eating and is thinking again.\n", id);
    }
}

int main()
{
    // Initialize semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_init(&chopsticks[i], 0, 1);
        philosopher_ids[i] = i;
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join threads (this example runs indefinitely)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores (unreachable in this example)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}