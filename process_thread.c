#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

// Function for child process
void child_process_function()
{
    printf("Child Process:\n");
    printf("  Process ID: %d\n", getpid());
    printf("  Parent Process ID: %d\n", getppid());
}

// Function for thread
void *thread_function(void *arg)
{
    int thread_num = *(int *)arg;
    printf("Thread %d:\n", thread_num);
    printf("  Thread ID: %lu\n", pthread_self());

    // Simulate some work
    sleep(1);

    return NULL;
}

// Demonstration of fork() process creation
void demonstrate_process_creation()
{
    printf("\n--- Process Creation Demonstration ---\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        child_process_function();
        exit(0);
    }
    else
    {
        // Parent process
        printf("Parent Process:\n");
        printf("  Process ID: %d\n", getpid());
        printf("  Child Process ID: %d\n", pid);

        // Wait for child to complete
        wait(NULL);
        printf("Child process completed\n");
    }
}

// Demonstration of pthread thread creation
void demonstrate_thread_creation()
{
    printf("\n--- Thread Creation Demonstration ---\n");

// Number of threads to create
#define NUM_THREADS 3

    // Thread identifiers
    pthread_t threads[NUM_THREADS];

    // Thread arguments
    int thread_args[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_args[i] = i;

        // Create thread
        int result = pthread_create(&threads[i],     // Thread identifier
                                    NULL,            // Thread attributes
                                    thread_function, // Thread function
                                    &thread_args[i]  // Thread function argument
        );

        if (result != 0)
        {
            fprintf(stderr, "Thread creation failed\n");
            exit(1);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed\n");
}

// Multiple process creation example
void demonstrate_multiple_processes()
{
    printf("\n--- Multiple Process Creation Demonstration ---\n");

#define NUM_PROCESSES 3

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            // Fork failed
            fprintf(stderr, "Fork failed\n");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            printf("Child Process %d:\n", i);
            printf("  Process ID: %d\n", getpid());
            printf("  Parent Process ID: %d\n", getppid());
            exit(0);
        }
    }

    // Parent process waits for all children
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        wait(NULL);
    }
    printf("All child processes completed\n");
}

int main()
{
    // Demonstrate process creation
    demonstrate_process_creation();

    // Demonstrate thread creation
    demonstrate_thread_creation();

    // Demonstrate multiple process creation
    demonstrate_multiple_processes();

    return 0;
}