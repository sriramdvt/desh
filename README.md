# Devata Shell (DeSH)

## 2019113007

## Sriram Devata

### Running DeSH
```
make clean
make
./desh
```

### Assumptions
- Assuming that the shell will run on a UNIX system, to get the home directory of the process with `/proc/self/exe`.
- There will not be a ; at the end.
- For ls -l, printing number of 512B blocks for file system blocks.
- Since Desh uses signal over sigaction, assuming that POSIX compliance isn't needed.
- To use regex such as `*.txt` for grep, use will use the flag `-E` for extended regular expressions

### File Structure
- build - temporary directory for object files
- src
    - cd.c - cd functionality
    - desh.c - main .c file
    - dpwd.c - pwd functionality
    - echo.c - echo functionality
    - general_execute.c - execute non-in-built commands
    - handler.c - handle the input command
    - history.c - history functionality
    - ls.c - ls functionality
    - pinfo.c - pinfo functionality
    - utils.c - miscellaneous helper functions
    - bg.c - bg functionality
    - fg.c - fg functionality
    - jobs.c - jobs functionality
    - pipeline.c - handles piping in commands
    - redirection.c - handles the input and output redirection for commands
    - sig.c - sig functionality

- include - corresponding .h files for the files in src
