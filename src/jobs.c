#include "jobs.h"

int num_bg_processes = 0;
struct bg_process *bg_processes;

int bg_cmp_alpha(const void * a, const void * b){

    struct bg_process *bg_a = (struct bg_process *)a;
    struct bg_process *bg_b = (struct bg_process *)b;

    return strcmp(bg_a->proc_name, bg_b->proc_name);
}

void add_bg_job(int proc_id, char *proc_name, char *p_status) {

    num_bg_processes++;

    bg_processes = (struct bg_process *) realloc(bg_processes, sizeof(struct bg_process) * num_bg_processes);

    struct bg_process new_bg = {0};
    new_bg.job_number = num_bg_processes - 1;
    new_bg.proc_id = proc_id;
    strcpy(new_bg.proc_name, proc_name);
    strcpy(new_bg.status, p_status == NULL ? "Run" : p_status);

    *(bg_processes + new_bg.job_number) = new_bg;

    qsort(bg_processes, num_bg_processes, sizeof(struct bg_process), bg_cmp_alpha);
    
    //print_bg_jobs();
}

void print_bg_jobs(){

    for (int bg_id = 0; bg_id < num_bg_processes; bg_id++){

        char bg_job_print[2*MAX_CMD_LEN] = {'\0'};
        sprintf(bg_job_print, "[%d] Running %s [%d]\n",   (bg_processes+bg_id)->job_number+1,
                                                        (bg_processes+bg_id)->proc_name,
                                                        (bg_processes+bg_id)->proc_id);
        print(bg_job_print);

    }

}

void jobs(struct command cmd) {

    extern int optind;
    optind = 0;

    int opt;
    int flag_r = 0;
    int flag_s = 0;

    if (cmd.argc == 1){
        flag_r = 1;
        flag_s = 1;
    }

    // get if the flags -l and -a are passed
    while((opt = getopt(cmd.argc, cmd.argv, "rs")) != -1) 
    {
        switch(opt) 
        { 
            case 'r':
                flag_r = 1;
                break;
            case 's':
                flag_s = 1;
                break;
            default:
                return;
        } 
    }

    for (int bg_id = 0; bg_id < num_bg_processes; bg_id++){
        // check if the processes needs to have their status updated

        // terminated processes won't have /proc/pid/stat file
        if (strcmp((bg_processes+bg_id)->status, "Not") == 0) continue;

        char proc_stat[PATH_MAX] = {'\0'};
        sprintf(proc_stat, "/proc/%d/stat", (bg_processes+bg_id)->proc_id);
        FILE *p_stat = fopen(proc_stat, "r");
        if (p_stat != NULL){
            // The process is either running or stopped
            int pid;
            char state;
            fscanf(p_stat, "%d %*s %c", &pid, &state);

            if (state == 'R' || state == 'S') {
                strcpy((bg_processes+bg_id)->status, "Run");
            }
            else if (state == 'T' || state == 't'){
                strcpy((bg_processes+bg_id)->status, "Stp");
            }
        }
        fclose(p_stat);
    }

    for (int bg_id = 0; bg_id < num_bg_processes; bg_id++){

        if (!flag_r && (strcmp((bg_processes+bg_id)->status, "Run") == 0)) continue;
        if (!flag_s && (strcmp((bg_processes+bg_id)->status, "Stp") == 0)) continue;

        char proc_status[20] = {'\0'};
        if (strcmp((bg_processes+bg_id)->status, "Run") == 0) strcpy(proc_status, "Running");
        else if (strcmp((bg_processes+bg_id)->status, "Stp") == 0) strcpy(proc_status, "Stopped");
        else continue;

        char bg_job_print[2*MAX_CMD_LEN] = {'\0'};
        sprintf(bg_job_print, "[%d] %s %s [%d]\n", (bg_processes+bg_id)->job_number+1,
                                                    proc_status,
                                                    (bg_processes+bg_id)->proc_name,
                                                    (bg_processes+bg_id)->proc_id);
        print(bg_job_print);

    }

}

