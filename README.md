# Energy-Aware EDF Scheduler

This project implements an **Energy-Aware Earliest Deadline First (EDF)** CPU scheduling algorithm in C. It simulates the execution of tasks in an embedded system where energy consumption is a critical factor.

## Overview

The scheduler prioritizes tasks based on the **Earliest Deadline First (EDF)** algorithm. In addition to standard scheduling metrics (Waiting Time, Turnaround Time), this simulator incorporates energy awareness by allowing frequency scaling for active tasks and implementing a low-power sleep mode during processor idle times.

## Features

- **EDF Scheduling (Earliest Deadline First)**: The scheduler dynamically selects the available task with the closest deadline to execute next.
- **Dynamic Voltage and Frequency Scaling (DVFS) Simulation**: Each task can be assigned a frequency scaling factor (0.1 to 1.0). Power consumption scales cubically with frequency (\(P \propto f^3\)), allowing simulation of running tasks at lower frequencies to save energy.
- **Optimized Idle (Sleep Mode)**: When no tasks are ready to execute, the processor jumps to the next arrival time rather than busy-waiting. During this period, it enters a low-power sleep mode (5mW).
- **Performance Metrics**: Calculates and reports Waiting Time, Turnaround Time, Total Energy Consumption, and whether each task met or missed its deadline.

## Logic and Working

1. **Input Generation**: The program accepts the total number of tasks. For each task, the user inputs its Arrival Time, Burst Time, Deadline, and a Frequency Scale factor.
2. **Scheduling Loop**: The core scheduling runs until all tasks are completed. At each time step:
   - It checks all tasks that have arrived but haven't completed.
   - It finds the task with the smallest (earliest) deadline.
3. **Task Execution (Active Mode)**:
   - If a valid task is found, it is scheduled to run.
   - Its power consumption is calculated using the formula: `Power (mW) = 100.0 * (Freq_Scale)^3`.
   - Energy consumed by the task is `Power * Burst_Time`.
   - The time is advanced by the task's burst time (assumed non-preemptive simulation for simplicity in the current implementation per burst step).
   - Metrics like Waiting Time and Turnaround Time are calculated upon completion.
4. **Processor Idle (Sleep Mode)**:
   - If no tasks are currently ready (i.e., the current time is less than the earlist arrival time of pending tasks), the processor goes into an idle state.
   - The system calculates the time until the next task arrives and advances the clock directly to that time.
   - Energy during this idle period is calculated at a sleep power level of `5mW`.
5. **Final Report**: Once all tasks complete, the program generates a table showing each task's performance, along with average waiting/turnaround times and the total energy consumed across the entire schedule.

## How to Run

1.  ### Compile the C Program
    Open a terminal and navigate to the project directory. Compile the code using `gcc`:
    ```bash
    gcc maincode.c -o scheduler -lm
    ```
    *(Note: `-lm` links the math library required for the `pow()` function)*

2.  ### Execute the Program
    Run the compiled executable:
    ```bash
    ./scheduler
    ```

3.  ### Provide Inputs
    Follow the on-screen prompts to enter the number of tasks, followed by the Arrival Time, Burst Time, Deadline, and Frequency Scale for each task.

## Example

```text
=== Energy-Aware EDF Scheduler ===
Enter number of tasks: 2

--- Task P1 ---
Arrival, Burst, Deadline: 0 4 10
Freq Scale (0.1 to 1.0): 1.0

--- Task P2 ---
Arrival, Burst, Deadline: 2 3 7
Freq Scale (0.1 to 1.0): 0.8
```

The output will detail the execution timeline showing Active and Sleep modes with corresponding energy consumption, and finish with a comprehensive metrics report.
