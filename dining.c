#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_PHILOSOPHERS 5
#define LEFT(x) ((x + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS)
#define RIGHT(x) ((x + 1) % NUM_PHILOSOPHERS)

// State of philosophers
enum
{
    THINKING,
    HUNGRY,
    EATING
} state[NUM_PHILOSOPHERS];

// Semaphores
sem_t mutex;                 // Mutual exclusion for critical sections
sem_t sem[NUM_PHILOSOPHERS]; // One semaphore per philosopher

// Function prototypes
void *philosopher(void *arg);
void take_forks(int philosopher_id);
void put_forks(int philosopher_id);
void test(int philosopher_id);

int main()
{
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_init(&sem[i], 0, 0);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_destroy(&sem[i]);
    }

    return 0;
}

void *philosopher(void *arg)
{
    int philosopher_id = *(int *)arg;

    while (1)
    {
        // Thinking
        printf("Philosopher %d is thinking.\n", philosopher_id);
        sleep(rand() % 3 + 1);

        // Get hungry
        take_forks(philosopher_id);

        // Eating
        printf("Philosopher %d is eating.\n", philosopher_id);
        sleep(rand() % 3 + 1);

        // Put down forks
        put_forks(philosopher_id);
    }

    return NULL;
}

void take_forks(int philosopher_id)
{
    // Enter critical section
    sem_wait(&mutex);

    // Set state to hungry
    state[philosopher_id] = HUNGRY;
    printf("Philosopher %d is hungry.\n", philosopher_id);

    // Try to acquire forks
    test(philosopher_id);

    // Exit critical section
    sem_post(&mutex);

    // Wait if forks not available
    sem_wait(&sem[philosopher_id]);
}

void put_forks(int philosopher_id)
{
    // Enter critical section
    sem_wait(&mutex);

    // Set state to thinking
    state[philosopher_id] = THINKING;
    printf("Philosopher %d put down forks.\n", philosopher_id);

    // Check if neighbors can eat
    test(LEFT(philosopher_id));
    test(RIGHT(philosopher_id));

    // Exit critical section
    sem_post(&mutex);
}

void test(int philosopher_id)
{
    // Check if philosopher can eat
    if (state[philosopher_id] == HUNGRY &&
        state[LEFT(philosopher_id)] != EATING &&
        state[RIGHT(philosopher_id)] != EATING)
    {

        state[philosopher_id] = EATING;
        sem_post(&sem[philosopher_id]);
    }
}