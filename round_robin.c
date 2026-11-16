#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int burst_time;
    int remaining_time;
    int waiting_time;
    int finish_time;
} Process;

static int count_completed_processes(const Process processes[], int process_count) {
    int completed = 0;
    for (int i = 0; i < process_count; i++) {
        if (processes[i].remaining_time == 0) {
            completed++;
        }
    }
    return completed;
}

static bool all_processes_completed(const Process processes[], int process_count) {
    return count_completed_processes(processes, process_count) == process_count;
}

static void print_execution_step(FILE *output_file, int process_id, int start_time, int end_time) {
    printf("P%d : %d-%d\n", process_id, start_time, end_time);
    fprintf(output_file, "P%d : %d-%d\n", process_id, start_time, end_time);
}

static double calculate_average_waiting_time(const Process processes[], int process_count) {
    int total_waiting_time = 0;
    for (int i = 0; i < process_count; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    return (double)total_waiting_time / process_count;
}

static void print_average_waiting_time(FILE *output_file, double avg_time) {
    printf("AVG TIME : %.2f ms\n", avg_time);
    fprintf(output_file, "AVG TIME : %.2f ms\n", avg_time);
}

void round_robin_schedule(FILE *output_file, Process processes[], int process_count, int quantum) {
    int current_time = 0;
    
    while (!all_processes_completed(processes, process_count)) {
        bool is_last_active_process = (count_completed_processes(processes, process_count) == process_count - 1);
        
        for (int i = 0; i < process_count; i++) {
            if (processes[i].remaining_time == 0) {
                continue;
            }
            
            if (!is_last_active_process) {
                processes[i].waiting_time += current_time - processes[i].finish_time;
            }
            
            int execution_time;
            if (processes[i].remaining_time >= quantum) {
                execution_time = quantum;
            } else {
                execution_time = processes[i].remaining_time;
            }
            
            print_execution_step(output_file, i + 1, current_time, current_time + execution_time);
            
            processes[i].remaining_time -= execution_time;
            current_time += execution_time;
            processes[i].finish_time = current_time;
        }
    }
    
    double avg_waiting_time = calculate_average_waiting_time(processes, process_count);
    print_average_waiting_time(output_file, avg_waiting_time);
}

static Process* allocate_processes(int process_count) {
    Process *processes = (Process*)malloc(sizeof(Process) * process_count);
    if (processes == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return processes;
}

static void initialize_process(Process *process, int burst_time) {
    process->burst_time = burst_time;
    process->remaining_time = burst_time;
    process->waiting_time = 0;
    process->finish_time = 0;
}

static FILE* open_output_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

int main(void) {
    int process_count, quantum;
    
    printf("Process number: ");
    if (scanf("%d", &process_count) != 1 || process_count <= 0) {
        fprintf(stderr, "Error: Invalid process count\n");
        return EXIT_FAILURE;
    }
    
    Process *processes = allocate_processes(process_count);
    
    for (int i = 0; i < process_count; i++) {
        int burst_time;
        printf("P-%d: ", i + 1);
        if (scanf("%d", &burst_time) != 1 || burst_time < 0) {
            fprintf(stderr, "Error: Invalid burst time\n");
            free(processes);
            return EXIT_FAILURE;
        }
        initialize_process(&processes[i], burst_time);
    }
    
    printf("Quantum time: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        fprintf(stderr, "Error: Invalid quantum time\n");
        free(processes);
        return EXIT_FAILURE;
    }
    
    FILE *output_file = open_output_file("cikti.txt");
    round_robin_schedule(output_file, processes, process_count, quantum);
    
    fclose(output_file);
    free(processes);
    
    return EXIT_SUCCESS;
}
