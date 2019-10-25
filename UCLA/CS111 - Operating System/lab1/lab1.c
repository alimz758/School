#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/time.h>

#define APPEND  O_APPEND
#define CLOEXEC O_CLOEXEC
#define CREAT   O_CREAT
#define DIRECTORY   O_DIRECTORY
#define DSYNC   O_DSYNC
#define EXCL    O_EXCL
#define NOFOLLOW    O_NOFOLLOW
#define NONBLOCK    O_NONBLOCK
#define SYNC    O_SYNC
#define TRUNC   O_TRUNC

static void sig_handler (int num) {
    fprintf(stderr, "%d caught\n", num);
    exit(num);
}

struct Command {
    char ** args;
    int num_args;
    bool err;
    pid_t id;
};

static int * file_table;
static struct Command * command_table;

int total_commands;
int total_files;

bool option_error = false;

struct Command parse_args(int argc, char * argv[]) {
    struct Command cmd;
    cmd.err = false;
    
    int num_args = 0;
    int index = optind - 1;
    int curr_index = optind;
    
    // Check for -- to signify end of command args
    while (index < argc && strncmp(argv[index], "--", 2)) {
        int fd = atoi(argv[index]);
        if ((fd < 0 || fd >= total_files) && num_args < 3) {
            fprintf(stderr, "Error: incorrect file descriptor number: %d\n", fd);
            cmd.err = true;
        }
        num_args++;
        index++;
    }
    // move index to after args for getopr
    optind = index;
    
    if (num_args < 4) {
        fprintf(stderr, "Error: Need at least 4 arguments for --command option.\n");
        cmd.err = true;
    }
    
    cmd.args = malloc(sizeof(char *) * (num_args + 1));
    cmd.args[0] = optarg;
    
    int i = 1;
    for (; i < num_args; i++) {
        cmd.args[i] = argv[curr_index + i - 1];
    }
    
    // Last needs to be set to null
    cmd.args[num_args] = NULL;
    cmd.num_args = num_args;
    
    // Check for closed fd
    i = 0;
    for (; i < 3; i++) {
        int file_index = atoi(cmd.args[i]);
        if (file_table[file_index] == -1) {
            fprintf(stderr, "Error: cannot open a closed file.\n");
            cmd.err = true;
        }
    }
    
    return cmd;
}

void execute(bool verbose) {
    
    if (verbose) {
        fprintf(stdout, "--command");
        int i = 0;
        for (; i < command_table[total_commands].num_args; i++) {
            fprintf(stdout, " ");
            fprintf(stdout, "%s", command_table[total_commands].args[i]);
        }
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    
    pid_t cpid = fork();
    if (cpid == 0) {
        // child process
        char ** args = command_table[total_commands].args;
        command_table[total_commands].id = getpid();
        
        int i = 0;
        for (; i < 3; i++) {
            int file_index = atoi(args[i]);
            
            if(close(i) < 0) {
                // error
                fprintf(stderr, "Error: Error closing file descriptor: %d.\n", i);
                option_error = true;
            }
            if(dup2(file_table[file_index], i) < 0) {
                // error
                fprintf(stderr, "Error: Invalid file descriptor: %d.\n", i);
                option_error = true;
            }
        }
    
        // Close unused fd
        int j = 0;
        for (; j < total_files; j++)
        {
            if (file_table[j] == -1)
                continue;
            close(file_table[j]);
        }
        
        int execvp_return = execvp(args[3], &args[3]);
        if (execvp_return == -1) {
            fprintf(stderr, "Error using execvp.\n");
        }
    }
    else if (cpid < 0) {
        // error
        fprintf(stderr, "Error: fork failed.");
        option_error = true;
    }
    else {
        // Parent
        command_table[total_commands].id = cpid;
    }
}

static struct rusage profile(struct rusage * usage) {
    int ret = 0;
    if((ret = getrusage(RUSAGE_SELF, usage)) < 0) {
        fprintf(stderr, "Error using getrusage: %d.\n", errno);
    }
    return *usage;
}

static struct rusage child_profile(struct rusage * usage) {
    int ret = 0;
    if((ret = getrusage(RUSAGE_CHILDREN, usage)) < 0) {
        fprintf(stderr, "Error using getrusage: %d.\n", errno);
    }
    return *usage;
}
// Credit to GNU Elapsed Time for following function
// https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html

static int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }
    
    /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;
    
    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

static struct option long_options[] = {
    {"rdonly",      required_argument,  NULL,   'r'},
    {"wronly",      required_argument,  NULL,   'w'},
    {"rdwr",      required_argument,  NULL,   'd'},
    {"pipe",      no_argument,  NULL,   'p'},
    {"command",     required_argument,  NULL,   'c'},
    {"wait",     no_argument,  NULL,   't'},
    {"verbose",     no_argument,        NULL,   'v'},
    {"close",     required_argument,        NULL,   'l'},
    {"abort",     no_argument,        NULL,   'a'},
    {"catch",     required_argument,        NULL,   'h'},
    {"ignore",     required_argument,        NULL,   'i'},
    {"default",     required_argument,        NULL,   'e'},
    {"pause",     no_argument,        NULL,   'u'},
	{"chdir", required_argument, NULL, 'x'},
    {"append",      no_argument,        NULL,   APPEND},
    {"cloexec",     no_argument,        NULL,   CLOEXEC},
    {"creat",       no_argument,        NULL,   CREAT},
    {"directory",   no_argument,        NULL,   DIRECTORY},
    {"dsync",       no_argument,        NULL,   DSYNC},
    {"excl",        no_argument,        NULL,   EXCL},
    {"nofollow",    no_argument,        NULL,   NOFOLLOW},
    {"nonblock",    no_argument,        NULL,   NONBLOCK},
    {"rsync",       no_argument,        NULL,   SYNC},
    {"sync",        no_argument,        NULL,   SYNC},
    {"trunc",       no_argument,        NULL,   TRUNC},
    {"profile",       no_argument,        NULL,   'f'},
    {0, 0, 0, 0}
};

int main(int argc, char * argv[]) {
    int option_index = 0, exit_max = 0, signal_max = 0, curr_commands = 0, c = 0, file_flags  = 0;
    bool command_flag = false, verbose_flag = false, wait_flag = false, child_signal = false, profile_flag = false;
    
    // Profile struct to be used
    struct rusage init_usage;
    struct rusage final_usage;
    struct timeval usr_result;
    struct timeval sys_result;
    int ret;
    // 1000 files
    file_table = malloc(1000 * sizeof(int));
    total_files = 0;
    
    // 1000 commands
    command_table = malloc(1000 * sizeof(struct Command));
    total_commands = 0;

    while((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        profile(&init_usage);
        switch (c) {
            // open tiime flags
            case APPEND:    case CLOEXEC:   case CREAT:     case DIRECTORY:
            case DSYNC:     case EXCL:      case NOFOLLOW:  case NONBLOCK:
            case SYNC:      case TRUNC:
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--%s\n", long_options[option_index].name);
                    fflush(stdout);
                }
                file_flags |= c;
                break;
			//chaning directory
			case 'x':
				
				//printf("in chdir %s", optarg);
				ret = chdir (optarg);
				if(verbose_flag){
					printf("--chdir %s\n", optarg);
					fflush(stdout);
				}
				if(ret==-1){
					fprintf(stderr,"Could not change the directory %s: ",strerror(errno));
					option_error=true;
				}
				break;
            case 'r':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--rdonly %s\n", optarg);
                    fflush(stdout);
                }
                file_flags |= O_RDONLY;
                if ((file_table[total_files] = open(optarg, file_flags, 0666)) < 0) {
                    fprintf(stderr, "Error opening input file. %s\n", strerror(errno));
                    option_error = true;
                }
                total_files++;
                file_flags = 0;
                break;
            case 'w':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--wronly %s\n", optarg);
                }
                file_flags |= O_WRONLY;
                if ((file_table[total_files] = open(optarg, file_flags, 0666)) < 0) {
                    fprintf(stderr, "Error opening input file. %s\n", strerror(errno));
                    option_error = true;
                }
                total_files++;
                file_flags = 0;
                break;
            case 'd':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--rdwr %s\n", optarg);
                }
                file_flags |= O_RDWR;
                if ((file_table[total_files] = open(optarg, file_flags, 0666)) < 0) {
                    fprintf(stderr, "Error opening input file. %s\n", strerror(errno));
                    option_error = true;
                }
                total_files++;
                file_flags = 0;
                break;
            case 'c':
                command_flag = (profile_flag ? true : false);
                command_table[total_commands] = parse_args(argc, argv);
                if (!command_table[total_commands].err) {
                    execute(verbose_flag);
                    total_commands++;
                    curr_commands++;
                }
                else {
                    option_error = true;
                }
                
                break;
            case 't':
                wait_flag = true;
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--wait\n");
                }
                
                int wait_status;
                pid_t cpid;
                int j = 0;
                while (j < curr_commands) {
                    int signal_num = 0;
                    int exit_status = 0;
                    
                    if ((cpid = waitpid(-1, &wait_status, 0)) != -1) {
                        if (WIFSIGNALED(wait_status)) {
                            signal_num = WTERMSIG(wait_status);
                            child_signal = true;
                            // Check for max signal
                            if (signal_num > signal_max) {
                                signal_max = signal_num;
                            }
                            fprintf(stdout, "signal %d", signal_num);
                            fflush(stdout);
                        }
                        else {
                            exit_status = WEXITSTATUS(wait_status);
                            // Checm for max exit
                            if (exit_status > exit_max) {
                                exit_max = exit_status;
                            }
                            fprintf(stdout, "exit %d", exit_status);
                            fflush(stdout);
                        }
                    }
                    else {
                        // Error
                        fprintf(stderr, "Error waiting for process: %d", errno);
                    }
                    fflush(stdout);
                    
                    // Print Executed Command
                    int i = 0;
                    for (; i < total_commands; i++) {
                        if (command_table[i].id == cpid) {
                            int k = 3;
                            for (; k < command_table[i].num_args; k++) {
                                fprintf(stdout, " ");
                                fprintf(stdout, "%s", command_table[i].args[k]);
                            }
                            fprintf(stdout, "\n");
                        }
                    }
                    fflush(stdout);
                    j++;
                }
                curr_commands = 0;
                break;
            case 'p':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--pipe\n");
                }
                int pipefd[2];
                if (pipe(pipefd) < 0) {
                    // Error
                    fprintf(stderr, "Error creating pipe. %s\n", strerror(errno));
                }
                
                file_table[total_files] = pipefd[0];
                file_table[total_files+1] = pipefd[1];
                total_files += 2;
                break;
            case 'v':
                verbose_flag = true;
                break;
            case 'l':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--close %s\n", optarg);
                    fflush(stdout);
                }
                int index = atoi(optarg);
                if (index < 0 || index > total_files) {
                    fprintf(stderr, "Not a valid file descriptor: %d\n", index);
                    option_error = true;
                }
                else {
                    close(file_table[index]);
                    file_table[index] = -1;
                }
                
                break;
            case 'a':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--abort\n");
                    fflush(stdout);
                }
                char * bad = NULL;
                *bad = 1;
                break;
            case 'h':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--catch %s\n", optarg);
                    fflush(stdout);
                }
                struct sigaction catch_action;
                int sig_num = atoi(optarg);
                catch_action.sa_handler = sig_handler;
                
                sigaction(sig_num, &catch_action, NULL);
                break;
            case 'i':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--ignore %s\n", optarg);
                    fflush(stdout);
                }
                struct sigaction ignore_action;
                sig_num = atoi(optarg);
                ignore_action.sa_handler = SIG_IGN;
                
                sigaction(sig_num, &ignore_action, NULL);
                
                break;
            case 'e':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--default %s\n", optarg);
                    fflush(stdout);
                }
                struct sigaction default_action;
                sig_num = atoi(optarg);
                default_action.sa_handler = SIG_DFL;
                
                sigaction(sig_num, &default_action, NULL);
                break;
            case 'u':
                if (verbose_flag) {
                    // Print message
                    fprintf(stdout, "--pause\n");
                    fflush(stdout);
                }
                pause();
                break;
            case 'f':
                profile_flag = true;
                break;
            default:
                fprintf(stderr, "Not a valid option.\n");
                option_error = true;
                break;
        }
        if (profile_flag && (strcmp(long_options[option_index].name, "profile") != 0)) {
            profile(&final_usage);
            timeval_subtract(&usr_result, &final_usage.ru_utime, &init_usage.ru_utime);
            timeval_subtract(&sys_result, &final_usage.ru_stime, &init_usage.ru_stime);
            fprintf(stdout, "%s -- User time: %ld.%06lds || System time: %ld.%06lds\n",
                    long_options[option_index].name, usr_result.tv_sec, usr_result.tv_usec, sys_result.tv_sec, sys_result.tv_usec);
            fflush(stdout);
        }
    }
    // Get Child Process Times when profile glag is set
    if (profile_flag && command_flag) {
        child_profile(&final_usage);
        fprintf(stdout, "Child processes -- User time: %ld.%06lds || System time: %ld.%06lds\n",
                final_usage.ru_utime.tv_sec, final_usage.ru_utime.tv_usec, final_usage.ru_stime.tv_sec, final_usage.ru_stime.tv_usec);
        fflush(stdout);
    }
    
    if (wait_flag) {
        if (child_signal) {
            signal(signal_max, SIG_DFL);
            fflush(stdout);
            raise(signal_max);
        }
        else {
            exit(exit_max);
        }
    }
    //the exit options
    if (option_error)
            exit(1);
    else
            exit(0);
}
