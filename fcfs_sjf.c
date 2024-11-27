#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

// Process structure
typedef struct Process
{
    int process_id;      // Process ID
    int arrival_time;    // Arrival time
    int burst_time;      // Total CPU burst time
    int waiting_time;    // Waiting time
    int turnaround_time; // Turnaround time
    int completion_time; // Completion time
} Process;

// Function prototypes
void fcfs_scheduling(Process processes[], int n);
void sjf_scheduling(Process processes[], int n);
void display_process_details(Process processes[], int n);
void sort_by_arrival_time(Process processes[], int n);
void sort_by_burst_time(Process processes[], int n);

// First-Come, First-Served (FCFS) Scheduling
void fcfs_scheduling(Process processes[], int n)
{
    int total_waiting_time = 0, total_turnaround_time = 0;

    // Sort processes by arrival time
    sort_by_arrival_time(processes, n);

    // Calculate completion times
    int current_time = 0;
    for (int i = 0; i < n; i++)
    {
        // Wait if process arrives later
        if (current_time < processes[i].arrival_time)
        {
            current_time = processes[i].arrival_time;
        }

        // Calculate completion time
        processes[i].completion_time = current_time + processes[i].burst_time;

        // Calculate waiting time and turnaround time
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;

        // Update current time
        current_time = processes[i].completion_time;

        // Accumulate total times
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    // Print FCFS results
    printf("\n--- First-Come, First-Served (FCFS) Scheduling Results ---\n");
    display_process_details(processes, n);

    printf("\nFCFS Scheduling Metrics:\n");
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

// Shortest Job First (SJF) Scheduling
void sjf_scheduling(Process processes[], int n)
{
    int total_waiting_time = 0, total_turnaround_time = 0;
    Process temp_processes[MAX_PROCESSES];

    // Create a copy of original processes
    for (int i = 0; i < n; i++)
    {
        temp_processes[i] = processes[i];
    }

    // Sort processes by arrival time
    sort_by_arrival_time(temp_processes, n);

    // Track completed processes
    bool completed[MAX_PROCESSES] = {false};
    int current_time = 0;
    int completed_count = 0;

    // SJF Scheduling
    while (completed_count < n)
    {
        int shortest_job_index = -1;
        int shortest_burst_time = INT_MAX;

        // Find shortest job among arrived processes
        for (int i = 0; i < n; i++)
        {
            if (!completed[i] &&
                temp_processes[i].arrival_time <= current_time &&
                temp_processes[i].burst_time < shortest_burst_time)
            {
                shortest_job_index = i;
                shortest_burst_time = temp_processes[i].burst_time;
            }
        }

        // If no process is available, increment time
        if (shortest_job_index == -1)
        {
            current_time++;
            continue;
        }

        // Process the shortest job
        int i = shortest_job_index;

        // Calculate completion time
        current_time += temp_processes[i].burst_time;
        temp_processes[i].completion_time = current_time;

        // Calculate waiting time and turnaround time
        temp_processes[i].waiting_time =
            current_time -
            temp_processes[i].arrival_time -
            temp_processes[i].burst_time;

        temp_processes[i].turnaround_time =
            temp_processes[i].waiting_time +
            temp_processes[i].burst_time;

        // Accumulate total times
        total_waiting_time += temp_processes[i].waiting_time;
        total_turnaround_time += temp_processes[i].turnaround_time;

        // Mark process as completed
        completed[i] = true;
        completed_count++;
    }

    // Copy back to original processes
    for (int i = 0; i < n; i++)
    {
        processes[i] = temp_processes[i];
    }

    // Print SJF results
    printf("\n--- Shortest Job First (SJF) Scheduling Results ---\n");
    display_process_details(processes, n);

    printf("\nSJF Scheduling Metrics:\n");
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

// Sort processes by arrival time
void sort_by_arrival_time(Process processes[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].arrival_time > processes[j + 1].arrival_time)
            {
                // Swap processes
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Sort processes by burst time
void sort_by_burst_time(Process processes[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].burst_time > processes[j + 1].burst_time)
            {
                // Swap processes
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Display process details
void display_process_details(Process processes[], int n)
{
    printf("PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].process_id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }
}

// Main function to demonstrate scheduling
int main()
{
    Process fcfs_processes[MAX_PROCESSES];
    Process sjf_processes[MAX_PROCESSES];
    int n;

    // Input process details
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input process information
    for (int i = 0; i < n; i++)
    {
        printf("\nProcess %d Details:\n", i + 1);

        // Set process ID
        fcfs_processes[i].process_id = sjf_processes[i].process_id = i + 1;

        // Input arrival time
        printf("Arrival Time: ");
        scanf("%d", &fcfs_processes[i].arrival_time);
        sjf_processes[i].arrival_time = fcfs_processes[i].arrival_time;

        // Input burst time
        printf("Burst Time: ");
        scanf("%d", &fcfs_processes[i].burst_time);
        sjf_processes[i].burst_time = fcfs_processes[i].burst_time;
    }

    // Perform FCFS Scheduling
    fcfs_scheduling(fcfs_processes, n);

    // Perform SJF Scheduling
    sjf_scheduling(sjf_processes, n);

    return 0;
}