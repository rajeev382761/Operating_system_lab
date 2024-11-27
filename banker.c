#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Global variables for Banker's Algorithm
int num_processes, num_resources;
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

// Function to input system resources and allocation
void input_resource_data()
{
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter the number of resources: ");
    scanf("%d", &num_resources);

    // Input Available Resources
    printf("Enter the number of available resources for each resource type:\n");
    for (int i = 0; i < num_resources; i++)
    {
        printf("Resource %d: ", i + 1);
        scanf("%d", &available[i]);
    }

    // Input Maximum Resource Needs
    printf("\nEnter Maximum Resource Need Matrix:\n");
    for (int i = 0; i < num_processes; i++)
    {
        printf("Process %d:\n", i + 1);
        for (int j = 0; j < num_resources; j++)
        {
            printf("  Resource %d max: ", j + 1);
            scanf("%d", &max[i][j]);
        }
    }

    // Input Current Allocation
    printf("\nEnter Current Allocation Matrix:\n");
    for (int i = 0; i < num_processes; i++)
    {
        printf("Process %d:\n", i + 1);
        for (int j = 0; j < num_resources; j++)
        {
            printf("  Resource %d allocated: ", j + 1);
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate Need Matrix
    printf("\nCalculating Need Matrix...\n");
    for (int i = 0; i < num_processes; i++)
    {
        for (int j = 0; j < num_resources; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Function to display matrices
void display_matrices()
{
    int i, j;

    // Allocation Matrix
    printf("\nAllocation Matrix:\n");
    for (i = 0; i < num_processes; i++)
    {
        for (j = 0; j < num_resources; j++)
        {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    // Max Matrix
    printf("\nMax Matrix:\n");
    for (i = 0; i < num_processes; i++)
    {
        for (j = 0; j < num_resources; j++)
        {
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }

    // Need Matrix
    printf("\nNeed Matrix:\n");
    for (i = 0; i < num_processes; i++)
    {
        for (j = 0; j < num_resources; j++)
        {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Available Resources
    printf("\nAvailable Resources:\n");
    for (j = 0; j < num_resources; j++)
    {
        printf("%d ", available[j]);
    }
    printf("\n");
}

// Safety Algorithm to check if system is in safe state
bool is_safe_state()
{
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safe_sequence[MAX_PROCESSES];
    int count = 0;

    // Initialize work with available resources
    for (int i = 0; i < num_resources; i++)
    {
        work[i] = available[i];
    }

    // Find a safe sequence
    while (count < num_processes)
    {
        bool found = false;

        for (int i = 0; i < num_processes; i++)
        {
            // Check if process is not finished
            if (!finish[i])
            {
                bool can_allocate = true;

                // Check if need is less than or equal to work
                for (int j = 0; j < num_resources; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        can_allocate = false;
                        break;
                    }
                }

                // If can allocate, add to safe sequence
                if (can_allocate)
                {
                    // Add resources back to work
                    for (int j = 0; j < num_resources; j++)
                    {
                        work[j] += allocation[i][j];
                    }

                    // Mark process as finished
                    finish[i] = true;
                    safe_sequence[count++] = i;
                    found = true;
                }
            }
        }

        // If no process can be allocated, system is in unsafe state
        if (!found)
        {
            return false;
        }
    }

    // Print safe sequence
    printf("\nSafe Sequence: ");
    for (int i = 0; i < num_processes; i++)
    {
        printf("P%d ", safe_sequence[i]);
    }
    printf("\n");

    return true;
}

// Request resource function
bool request_resources(int process_num, int request[])
{
    // Check if request exceeds need
    for (int i = 0; i < num_resources; i++)
    {
        if (request[i] > need[process_num][i])
        {
            printf("Error: Request exceeds process's maximum need!\n");
            return false;
        }
    }

    // Check if request exceeds available
    for (int i = 0; i < num_resources; i++)
    {
        if (request[i] > available[i])
        {
            printf("Error: Insufficient resources available!\n");
            return false;
        }
    }

    // Simulate allocation
    for (int i = 0; i < num_resources; i++)
    {
        available[i] -= request[i];
        allocation[process_num][i] += request[i];
        need[process_num][i] -= request[i];
    }

    // Check if the system remains in a safe state
    if (is_safe_state())
    {
        printf("Resource request for Process P%d is granted.\n", process_num);
        return true;
    }
    else
    {
        // Rollback if system becomes unsafe
        for (int i = 0; i < num_resources; i++)
        {
            available[i] += request[i];
            allocation[process_num][i] -= request[i];
            need[process_num][i] += request[i];
        }
        printf("Resource request for Process P%d is denied to prevent potential deadlock.\n", process_num);
        return false;
    }
}

// Main function
int main()
{
    // Input resource data
    input_resource_data();

    // Display input matrices
    display_matrices();

    // Check initial system state
    printf("\nChecking Initial System State:\n");
    if (is_safe_state())
    {
        printf("Initial system state is SAFE.\n");
    }
    else
    {
        printf("Initial system state is UNSAFE.\n");
    }

    // Demonstration of resource request
    int example_request[MAX_RESOURCES];
    printf("\nDemonstrating Resource Request:\n");
    printf("Enter a process number (0 to %d): ", num_processes - 1);
    int process_num;
    scanf("%d", &process_num);

    printf("Enter resource request for Process P%d:\n", process_num);
    for (int i = 0; i < num_resources; i++)
    {
        printf("  Resource %d request: ", i + 1);
        scanf("%d", &example_request[i]);
    }

    // Request resources
    request_resources(process_num, example_request);

    return 0;
}