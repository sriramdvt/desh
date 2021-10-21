#include "ls.h"

void ls(struct command cmd) {

    extern int optind;
    optind = 0;

    int opt;
    int flag_l = 0;
    int flag_a = 0;

    // get if the flags -l and -a are passed
    while((opt = getopt(cmd.argc, cmd.argv, "la")) != -1) 
    {
        switch(opt) 
        { 
            case 'l':
                flag_l = 1;
                break;
            case 'a':
                flag_a = 1;
                break;
            default:
                return;
        } 
    }

    char* cwd = {'\0'};
    cwd = getcwd(NULL, 0);
    
    if (cmd.argc == optind) {
        // if the path isn't given, ls the current working directory
        char ls_fpath[PATH_MAX] = {'\0'};
        strcpy(ls_fpath, cwd);
        ls_path(ls_fpath, flag_l, flag_a);
    }

    for (int arg_idx = optind; arg_idx < cmd.argc; arg_idx++) {
        // iterate through each of the paths given

        char ls_fpath[2*PATH_MAX] = {'\0'};

        // get the path to ls in ls_fpath
        if (cmd.argv[arg_idx][0] - '~' == 0) {
            sprintf(ls_fpath, "%s%s", prompt_home_dir, cmd.argv[arg_idx]+1);
        }
        else if (strcmp(cmd.argv[arg_idx], ".") == 0) {
            sprintf(ls_fpath, "%s", cwd);
        }
        else if (strcmp(cmd.argv[arg_idx], "..") == 0) {
            sprintf(ls_fpath, "%s", cwd);
            dirname(ls_fpath);
        }
        else{
            sprintf(ls_fpath, "%s", cmd.argv[arg_idx]);
        }

        // if there are multiple paths
        if (cmd.argc - optind != 0) {
            char dir_name[PATH_MAX] = {'\0'};
            sprintf(dir_name, "%s:\n", cmd.argv[arg_idx]);
            print(dir_name);
        }

        ls_path(ls_fpath, flag_l, flag_a);

        if (cmd.argc - optind != 0) print("\n");
    }

}

int filter_flag_a(const struct dirent* f_path) {
    return (f_path->d_name[0] != '.');
}

int lengthsort (const struct dirent **d1, const struct dirent **d2) {
    return strlen((*d1)->d_name) > strlen((*d2)->d_name);
}

void print_info (struct stat f_st) {

    print(S_ISDIR(f_st.st_mode) ? "d" : "-");
    print(f_st.st_mode & S_IRUSR ? "r" : "-");
    print(f_st.st_mode & S_IWUSR ? "w" : "-");
    print(f_st.st_mode & S_IXUSR ? "x" : "-");
    print(f_st.st_mode & S_IRGRP ? "r" : "-");
    print(f_st.st_mode & S_IWGRP ? "w" : "-");
    print(f_st.st_mode & S_IXGRP ? "x" : "-");
    print(f_st.st_mode & S_IROTH ? "r" : "-");
    print(f_st.st_mode & S_IWOTH ? "w" : "-");
    print(f_st.st_mode & S_IXOTH ? "x" : "-");
    print(" ");

    char hardlinks[4] = {'\0'};
    sprintf(hardlinks, "%ju ", f_st.st_nlink);
    print(hardlinks);

    char owner[LOGIN_NAME_MAX] = {'\0'};
    sprintf(owner, "%s ", getpwuid(f_st.st_uid)->pw_name);
    print(owner);
    
    char group[LOGIN_NAME_MAX] = {'\0'};
    sprintf(group, "%s ", getgrgid(f_st.st_gid)->gr_name);
    print(group);

    char size[64] = {'\0'};
    sprintf(size, "%ld ", f_st.st_size);
    print(size);

    char time_output[256];
    time_t t = f_st.st_mtime, present_time;
    struct tm *tmp, *present_time_info;

    time(&present_time);
    present_time_info = localtime(&present_time);

    tmp = localtime(&t);   

    if(tmp == NULL || present_time_info == NULL) {
        perror("Localtime Error"); 
        return;
    } 

    if (difftime(present_time, t) < 15780000){
        strftime(time_output, sizeof(time_output), "%b %d %H:%M ", tmp);
    }
    else{
        strftime(time_output, sizeof(time_output), "%b %d %Y ", tmp);
    }
    print(time_output);

}

void ls_path(char* ls_pathname, int flag_l, int flag_a) {

    struct stat path_arg = {};
    if((stat(ls_pathname, &path_arg)) == -1) {
        perror("Cannot stat path");
        return;
    }

    if(S_ISREG(path_arg.st_mode)) {

        if (flag_l) print_info(path_arg);
        char name[PATH_MAX] = {'\0'};
        sprintf(name, "%s\n", basename(ls_pathname));
        print(name);

    }
    else if(S_ISDIR(path_arg.st_mode)) {

        struct dirent **namelist;
        int n_files;

        n_files = scandir(ls_pathname, &namelist, flag_a ? NULL : filter_flag_a, lengthsort);
        if (n_files < 0) {
            perror("Unable to scandir");
            return;
        }

        char blocks[64] = {'\0'};
        if (flag_l) sprintf(blocks, "total %ld\n", path_arg.st_blocks);
        print(blocks);

        for (int file_id = 0; file_id < n_files; file_id++) {
            if (flag_l){

                struct stat f_st = {0};

                char buf[PATH_MAX + MAX_ARG_LEN] = {'\0'};
                sprintf(buf, "%s/%s", ls_pathname, namelist[file_id]->d_name);

                if (stat(buf, &f_st) < 0) {
                    perror("Unable to stat file");
                    continue;
                }
                print_info(f_st);
            }
            char name[PATH_MAX] = {'\0'};
            sprintf(name, "%s\n", namelist[file_id]->d_name);
            print(name);

            free(namelist[file_id]);
        }
        free(namelist);
    }
}
