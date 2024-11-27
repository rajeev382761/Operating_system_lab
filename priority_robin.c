#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 100

// Process structure
typedef struct Process
{
    int process_id;      // Process ID
    int arrival_time;    // Arrival time
    int burst_time;      // Total CPU burst time
    int remaining_time;  // Remaining burst time
    int priority;        // Priority value (lower number = higher priority)
    int waiting_time;    // Waiting time
    int turnaround_time; // Turnaround time
} Process;

// Function prototypes
void priority_scheduling(Process processes[], int n);
void round_robin_scheduling(Process processes[], int n, int time_quantum);
void display_process_details(Process processes[], int n);

// Priority Scheduling Algorithm
void priority_scheduling(Process processes[], int n)
{
    Process temp;
    int total_waiting_time = 0, total_turnaround_time = 0;

    // Sort processes based on priority (lower number = higher priority)
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].priority > processes[j + 1].priority)
            {
                // Swap processes
                temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Calculate waiting and turnaround times
    processes[0].waiting_time = 0;
    processes[0].turnaround_time = processes[0].burst_time;

    for (int i = 1; i < n; i++)
    {
        processes[i].waiting_time =
            processes[i - 1].waiting_time +
            processes[i - 1].burst_time;

        processes[i].turnaround_time =
            processes[i].waiting_time +
            processes[i].burst_time;

        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    // Print results
    printf("\n--- Priority Scheduling Results ---\n");
    display_process_details(processes, n);

    printf("\nAverage Waiting Time: %.2f\n",
           (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n",
           (float)total_turnaround_time / n);
}

// Round Robin Scheduling Algorithm
void round_robin_scheduling(Process processes[], int n, int time_quantum)
{
    int total_waiting_time = 0, total_turnaround_time = 0;
    int current_time = 0;
    int completed_processes = 0;

    // Create a copy of processes to manipulate
    Process queue[MAX_PROCESSES];
    for (int i = 0; i < n; i++)
    {
        queue[i] = processes[i];
        queue[i].remaining_time = queue[i].burst_time;
    }

    // Round Robin scheduling simulation
    while (completed_processes < n)
    {
        for (int i = 0; i < n; i++)
        {
            // Skip already completed processes
            if (queue[i].remaining_time <= 0)
                continue;

            // Process for time quantum or remaining time
            if (queue[i].remaining_time > time_quantum)
            {
                current_time += time_quantum;
                queue[i].remaining_time -= time_quantum;
            }
            else
            {
                current_time += queue[i].remaining_time;
                queue[i].remaining_time = 0;

                // Calculate waiting and turnaround times
                queue[i].turnaround_time = current_time;
                queue[i].waiting_time =
                    queue[i].turnaround_time - queue[i].burst_time;

                total_waiting_time += queue[i].waiting_time;
                total_turnaround_time += queue[i].turnaround_time;

                completed_processes++;
            }
        }
    }

    // Replace original processes with modified queue
    for (int i = 0; i < n; i++)
    {
        processes[i] = queue[i];
    }

    // Print results
    printf("\n--- Round Robin Scheduling Results ---\n");
    printf("Time Quantum: %d\n", time_quantum);
    display_process_details(processes, n);

    printf("\nAverage Waiting Time: %.2f\n",
           (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n",
           (float)total_turnaround_time / n);
}

// Display process details
void display_process_details(Process processes[], int n)
{
    printf("PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].process_id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }
}

// Main function to demonstrate scheduling
int main()
{
    Process priority_processes[MAX_PROCESSES];
    Process rr_processes[MAX_PROCESSES];
    int n, time_quantum;

    // Input process details
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter Time Quantum for Round Robin: ");
    scanf("%d", &time_quantum);

    // Input process information
    for (int i = 0; i < n; i++)
    {
        printf("\nProcess %d Details:\n", i + 1);

        // Set process ID
        priority_processes[i].process_id = rr_processes[i].process_id = i + 1;

        // Input arrival time
        printf("Arrival Time: ");
        scanf("%d", &priority_processes[i].arrival_time);
        rr_processes[i].arrival_time = priority_processes[i].arrival_time;

        // Input burst time
        printf("Burst Time: ");
        scanf("%d", &priority_processes[i].burst_time);
        rr_processes[i].burst_time = priority_processes[i].burst_time;

        // Input priority
        printf("Priority (lower number = higher priority): ");
        scanf("%d", &priority_processes[i].priority);
        rr_processes[i].priority = priority_processes[i].priority;
    }

    // Perform Priority Scheduling
    priority_scheduling(priority_processes, n);

    // Perform Round Robin Scheduling
    round_robin_scheduling(rr_processes, n, time_quantum);

    return 0;
}