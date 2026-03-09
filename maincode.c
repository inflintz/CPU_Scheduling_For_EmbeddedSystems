#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Task {
    int id;
    int arrival_time;
    int burst_time;
    int deadline;
    int waiting_time;
    int turnaround_time;
    float freq_scale; 
    float energy_mwh;
    int completed;
};

int main() {
    int n, completed_tasks = 0, currentTime = 0;
    float total_wt = 0, total_tat = 0, total_energy = 0;

    printf("=== Energy-Aware EDF Scheduler ===\n");
    printf("Enter number of tasks: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;

    struct Task *t = (struct Task*)malloc(n * sizeof(struct Task));
    if(!t) return 1;

    for (int i = 0; i < n; i++) {
        t[i].id = i + 1;
        printf("\n--- Task P%d ---\n", t[i].id);
        printf("Arrival, Burst, Deadline: ");
        scanf("%d %d %d", &t[i].arrival_time, &t[i].burst_time, &t[i].deadline);
        printf("Freq Scale (0.1 to 1.0): ");
        scanf("%f", &t[i].freq_scale);
        t[i].completed = 0;
    }

    printf("\nExecution Log:\nTime\tEvent\t\tPower Status\n--------------------------------------------\n");

    while (completed_tasks < n) {
        int idx = -1;
        int min_deadline = 1e9;

        // EDF Logic: Find arrived task with earliest deadline [cite: 23, 24]
        for (int i = 0; i < n; i++) {
            if (t[i].arrival_time <= currentTime && !t[i].completed) {
                if (t[i].deadline < min_deadline) {
                    min_deadline = t[i].deadline;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            t[idx].waiting_time = currentTime - t[idx].arrival_time;
            
            // Power Calculation based on frequency scaling [cite: 25, 29]
            float power_mw = 100.0 * pow(t[idx].freq_scale, 3);
            t[idx].energy_mwh = t[idx].burst_time * power_mw;

            printf("%d\tRunning P%d\tActive Mode (%.2fmW)\n", currentTime, t[idx].id, power_mw);

            currentTime += t[idx].burst_time;
            t[idx].turnaround_time = t[idx].waiting_time + t[idx].burst_time;
            t[idx].completed = 1;
            completed_tasks++;

            total_wt += t[idx].waiting_time;
            total_tat += t[idx].turnaround_time;
            total_energy += t[idx].energy_mwh;
        } else {
            // OPTIMIZED IDLE: Find the next arrival time to jump to 
            int next_arrival = 1e9;
            for(int i = 0; i < n; i++) {
                if(!t[i].completed && t[i].arrival_time < next_arrival)
                    next_arrival = t[i].arrival_time;
            }
            
            if(next_arrival != 1e9) {
                int idle_time = next_arrival - currentTime;
                total_energy += (idle_time * 5.0); // Sleep power 
                printf("%d\tIDLE\t\tSleep Mode (5mW) for %d units\n", currentTime, idle_time);
                currentTime = next_arrival;
            }
        }
    }

    // Output Performance Metrics [cite: 18, 31]
    printf("\n================ FINAL REPORT ================\n");
    printf("ID\tWait\tTAT\tEnergy\tDeadline Status\n----------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%.2f\t%s\n", 
               t[i].id, t[i].waiting_time, t[i].turnaround_time, 
               t[i].energy_mwh, 
               (t[i].arrival_time + t[i].turnaround_time <= t[i].deadline) ? "MET" : "MISSED");
    }

    printf("\nAvg Wait: %.2f | Avg TAT: %.2f\n", total_wt / n, total_tat / n);
    printf("Total Energy: %.2f mWh\n", total_energy);

    free(t);
    return 0;
}