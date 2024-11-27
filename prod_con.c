#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

// Buffer configuration
#define BUFFER_SIZE 5

// Shared buffer and related variables
int buffer[BUFFER_SIZE];
int in = 0;         // Index for inserting items
int out = 0;        // Index for removing items
int item_count = 0; // Number of items in the buffer

// Semaphores
sem_t mutex;       // Ensures mutual exclusion
sem_t empty_slots; // Tracks empty slots in the buffer
sem_t full_slots;  // Tracks filled slots in the buffer

// Function prototypes
void *producer(void *arg);
void *consumer(void *arg);
void insert_item(int item);
int remove_item();

int main()
{
    // Thread identifiers
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores
    sem_init(&mutex, 0, 1);                 // Binary semaphore for mutual exclusion
    sem_init(&empty_slots, 0, BUFFER_SIZE); // Initially all slots are empty
    sem_init(&full_slots, 0, 0);            // Initially no slots are full

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);

    return 0;
}

void *producer(void *arg)
{
    int item;

    for (int i = 0; i < 20; i++)
    {
        // Generate a random item
        item = rand() % 100;

        // Wait for an empty slot
        sem_wait(&empty_slots);
        // Acquire mutex lock
        sem_wait(&mutex);

        // Insert item into buffer
        insert_item(item);
        printf("Produced item: %d\n", item);

        // Release mutex lock
        sem_post(&mutex);
        // Signal that a slot is now full
        sem_post(&full_slots);

        // Simulate some processing time
        sleep(rand() % 2);
    }

    return NULL;
}

void *consumer(void *arg)
{
    int item;

    for (int i = 0; i < 20; i++)
    {
        // Wait for a full slot
        sem_wait(&full_slots);
        // Acquire mutex lock
        sem_wait(&mutex);

        // Remove item from buffer
        item = remove_item();
        printf("Consumed item: %d\n", item);

        // Release mutex lock
        sem_post(&mutex);
        // Signal that an empty slot is now available
        sem_post(&empty_slots);

        // Simulate some processing time
        sleep(rand() % 2);
    }

    return NULL;
}

void insert_item(int item)
{
    // Insert item at the 'in' index and update
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    item_count++;
}

int remove_item()
{
    // Remove and return item from 'out' index
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    item_count--;
    return item;
}