#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define MAX_FDS 4096

// -------------------- Simple Set --------------------
typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} Set;

void set_init(Set *s) {
    s->items = NULL;
    s->count = 0;
    s->capacity = 0;
}

int set_contains(Set *s, const char *value) {
    for (size_t i = 0; i < s->count; i++) {
        if (strcmp(s->items[i], value) == 0)
            return 1;
    }
    return 0;
}

void set_add(Set *s, const char *value) {
    if (set_contains(s, value))
        return;

    if (s->count == s->capacity) {
        s->capacity = s->capacity == 0 ? 16 : s->capacity * 2;
        s->items = realloc(s->items, s->capacity * sizeof(char *));
    }

    s->items[s->count++] = strdup(value);
}

int cmp_str(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

void print_set(const char *title, Set *s) {
    printf("\n==== %s ====\n\n", title);

    qsort(s->items, s->count, sizeof(char *), cmp_str);

    for (size_t i = 0; i < s->count; i++) {
        printf("%s\n", s->items[i]);
    }
}

// -------------------- Global Sets --------------------
Set READ, WRITE, CREATE, MKDIR, DELETE, RENAME;
Set METADATA, EXEC, PROCESS;
Set NETWORK_CONNECT, NETWORK_BIND;
Set IPC, MOUNT, PRIVILEGED;
Set UNKNOWN;

// -------------------- FD Tracking --------------------
typedef struct {
    int used;
    int pid;
    int fd;
    char path[1024];
} FDEntry;

FDEntry fd_table[MAX_FDS * 4]; // more space
int fd_count = 0;

void fd_set_val(int pid, int fd, const char *path) {
    if (fd < 0)
        return;

    for (int i = 0; i < fd_count; i++) {
        if (fd_table[i].used && fd_table[i].pid == pid && fd_table[i].fd == fd) {
            strncpy(fd_table[i].path, path, sizeof(fd_table[i].path)-1);
            return;
        }
    }

    fd_table[fd_count++] = (FDEntry){1, pid, fd, {0}};
    strncpy(fd_table[fd_count-1].path, path, sizeof(fd_table[fd_count-1].path)-1);
}

char* fd_get_val(int pid, int fd) {
    for (int i = 0; i < fd_count; i++) {
        if (fd_table[i].used && fd_table[i].pid == pid && fd_table[i].fd == fd)
            return fd_table[i].path;
    }

    return NULL;
}

void fd_close_val(int pid, int fd) {
    for (int i = 0; i < fd_count; i++) {
        if (fd_table[i].used && fd_table[i].pid == pid && fd_table[i].fd == fd) {
            fd_table[i].used = 0;
            return;
        }
    }
}

// -------------------- Helpers --------------------
int extract_pid(const char *line, const char **rest) {
    int pid;
    if (sscanf(line, "%d %n", &pid, (int[]){0}) == 1) {
        *rest = strchr(line, ' ');
        if (*rest) (*rest)++;
        return pid;
    }
    *rest = line;
    return 0; // fallback
}

int extract_fd_return(const char *line) {
    int fd;
    if (sscanf(line, "%*[^=]= %d", &fd) == 1 && fd >= 0)
        return fd;
    return -1;
}

int extract_fd_arg(const char *line, int *fd) {
    return sscanf(line, "%*[^ (](%d,", fd) == 1;
}

int extract_two_fds(const char *line, int *fd1, int *fd2) {
    return sscanf(line, "%*[^ (](%d, %d", fd1, fd2) == 2;
}

// Extract two FDs from pipe([x,y])
void extract_pipe_fds(const char *line, int *fd1, int *fd2) {
    sscanf(line, "%*[^[][%d,%d]", fd1, fd2);
}

void extract_path(const char *line, char *out) {
    char *start = strchr(line, '"');
    if (!start) 
        return;
    
    char *end = strchr(start + 1, '"');
    if (!end)
        return;

    size_t len = end - start - 1;
    strncpy(out, start + 1, len);
    out[len] = '\0';
}

void extract_socket_addr(const char *line, char *out) {
    char ip[128] = {0};
    int port = 0;

    char *ip_start = strstr(line, "inet_addr(\"");
    if (ip_start) {
        sscanf(ip_start, "inet_addr(\"%127[^\"]\")", ip);
    }

    char *port_start = strstr(line, "htons(");
    if (port_start) {
        sscanf(port_start, "htons(%d)", &port);
    }

    if (strlen(ip) && port > 0) {
        snprintf(out, 256, "tcp://%s:%d", ip, port);
    } else {
        strcpy(out, "socket");
    }
}

// -------------------- Logic --------------------
void handle_open(const char *line, int pid) {
    char path[1024]={0};
    extract_path(line, path);

    int fd = extract_fd_return(line);
    if (fd >= 0)
        fd_set_val(pid, fd, path);

    if (strstr(line, "O_CREAT"))
        set_add(&CREATE, path);
    if (strstr(line, "O_WRONLY") || strstr(line, "O_RDWR") || strstr(line, "O_TRUNC"))
        set_add(&WRITE, path);
    if (strstr(line, "O_RDONLY"))
        set_add(&READ, path);
}

void handle_fd_read(const char *line, int pid) {
    int fd;
    if (extract_fd_arg(line, &fd)) {
        char *path = fd_get_val(pid, fd);
        if (path) set_add(&READ, path);
    }
}

void handle_fd_write(const char *line, int pid) {
    int fd;
    if (extract_fd_arg(line, &fd)) {
        char *path = fd_get_val(pid, fd);
        if (path) set_add(&WRITE, path);
    }
}

void handle_close(const char *line, int pid) {
    int fd;
    if (sscanf(line, "close(%d)", &fd) == 1) {
        fd_close_val(pid, fd);
    }
}

void parse_line(char *line) {

    const char *rest;
    int pid = extract_pid(line, &rest);

    char syscall[64] = {0};
    sscanf(rest, "%63[^ (]", syscall);

    // ---- OPEN ----
    if (!strcmp(syscall, "open") || !strcmp(syscall, "openat")) {
        handle_open(rest, pid);
    }

    // ---- FD READ ----
    else if (!strcmp(syscall, "read") || !strcmp(syscall, "pread64") ||
             !strcmp(syscall, "readv")) {
        handle_fd_read(rest, pid);
    }

    // ---- FD WRITE ----
    else if (!strcmp(syscall, "write") || !strcmp(syscall, "pwrite64") ||
             !strcmp(syscall, "writev")) {
        handle_fd_write(rest, pid);
    }

    // ---- CLOSE ----
    else if (!strcmp(syscall, "close")) {
        handle_close(rest, pid);
    }

    // ---- DIRECTORY READ ----
    else if (!strcmp(syscall, "getdents64")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char *path = fd_get_val(pid, fd);
            if (path) set_add(&READ, path);
        }
    }

    // ---- METADATA (PATH) ----
    else if (!strcmp(syscall, "stat") || !strcmp(syscall, "lstat") ||
             !strcmp(syscall, "statx") || !strcmp(syscall, "newfstatat") ||
             !strcmp(syscall, "fstatat")) {

        char path[1024] = {0};
        extract_path(rest, path);
        if (strlen(path)) set_add(&METADATA, path);
    }

    else if (!strcmp(syscall, "uname")) {
        set_add(&METADATA, "system_info");
    }

    else if (!strcmp(syscall, "lseek")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char *path = fd_get_val(pid, fd);
            if (path) set_add(&METADATA, path);
        }
    }

    else if (!strcmp(syscall, "getcwd")) {
        char path[1024]={0};
        extract_path(rest, path);
        if (strlen(path)) set_add(&METADATA, path);
    }

    // ---- METADATA (PERM) ----
    else if (!strcmp(syscall, "chmod") || !strcmp(syscall, "fchmodat")) {
        char path[1024]={0};
        extract_path(rest, path);
        if (strlen(path)) set_add(&METADATA, path);
    }

    else if (!strcmp(syscall, "chown") || !strcmp(syscall, "fchownat")) {
        char path[1024]={0};
        extract_path(rest, path);
        if (strlen(path)) set_add(&METADATA, path);
    }

    // ---- METADATA (FD) ----
    else if (!strcmp(syscall, "fstat")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char *path = fd_get_val(pid, fd);
            if (path) set_add(&METADATA, path);
        }
    }

    // ---- ACCESS ----
    else if (!strcmp(syscall, "access")) {
        char path[1024] = {0};
        extract_path(rest, path);
        if (strlen(path)) set_add(&READ, path);
    }

    // ---- READLINK ----
    else if (!strcmp(syscall, "readlink")) {
        char path[1024] = {0};
        extract_path(rest, path);
        if (strlen(path)) set_add(&READ, path);
    }

    // ---- MKDIR ----
    else if (!strcmp(syscall, "mkdir") || !strcmp(syscall, "mkdirat")) {
        char path[1024] = {0};
        extract_path(rest, path);
        set_add(&MKDIR, path);
    }

    // ---- CREATE ----
    else if (!strcmp(syscall, "link") || !strcmp(syscall, "linkat") ||
             !strcmp(syscall, "symlink") || !strcmp(syscall, "symlinkat") ||
             !strcmp(syscall, "creat")) {

        char path[1024] = {0};
        extract_path(rest, path);
        set_add(&CREATE, path);
    }

    // ---- DELETE ----
    else if (!strcmp(syscall, "unlink") || !strcmp(syscall, "unlinkat") ||
             !strcmp(syscall, "rmdir")) {

        char path[1024] = {0};
        extract_path(rest, path);
        set_add(&DELETE, path);
    }

    // ---- RENAME ----
    else if (!strcmp(syscall, "rename") || !strcmp(syscall, "renameat") || !strcmp(syscall, "renameat2")) {
        char p1[1024]={0}, p2[1024]={0};
        char *first = strchr(rest, '"');
        if (first) {
            char *second = strchr(first+1, '"');
            char *third = second ? strchr(second+1, '"') : NULL;
            char *fourth = third ? strchr(third+1, '"') : NULL;

            if (second && third && fourth) {
                snprintf(p1, sizeof(p1), "%.*s", (int)(second-first-1), first+1);
                snprintf(p2, sizeof(p2), "%.*s", (int)(fourth-third-1), third+1);

                char buf[4096];
                snprintf(buf, sizeof(buf), "%s -> %s", p1, p2);
                set_add(&RENAME, buf);
            }
        }
    }

    // ---- EXEC ----
    else if (!strcmp(syscall, "execve") || !strcmp(syscall, "execveat")) {
        char path[1024]={0};
        extract_path(rest, path);
        if (strlen(path)) {
            set_add(&EXEC, path);
            set_add(&READ, path);
        }
    }

    // ---- PROCESS ----
    else if (!strcmp(syscall, "fork") || !strcmp(syscall, "vfork") ||
             !strcmp(syscall, "clone") || !strcmp(syscall, "clone3")) {
        set_add(&PROCESS, syscall);
    }

    // ---- SOCKET ----
    else if (!strcmp(syscall, "socket")) {
        int fd = extract_fd_return(rest);
        if (fd >= 0) fd_set_val(pid, fd, "socket");
    }

    else if (!strcmp(syscall, "connect")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char addr[256]={0};
            extract_socket_addr(rest, addr);

            fd_set_val(pid, fd, addr);
            set_add(&NETWORK_CONNECT, addr);
        }
    }

    else if (!strcmp(syscall, "recvfrom") || !strcmp(syscall, "recvmsg")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char *path = fd_get_val(pid, fd);
            if (path)
                set_add(&NETWORK_CONNECT, path);
        }
    }

    else if (!strcmp(syscall, "sendto")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char *path = fd_get_val(pid, fd);
            if (path)
                set_add(&NETWORK_CONNECT, path);
        }
    }

    else if (!strcmp(syscall, "getsockname") || !strcmp(syscall, "getpeername")) {
        int fd;
        if (extract_fd_arg(rest, &fd)) {
            char *path = fd_get_val(pid, fd);
            if (path)
                set_add(&NETWORK_CONNECT, path);
        }
    }

    else if (!strcmp(syscall, "bind") || !strcmp(syscall, "listen") ||
             !strcmp(syscall, "accept") || !strcmp(syscall, "accept4") || !strcmp(syscall, "setsockopt")) {
        set_add(&NETWORK_BIND, syscall);
    }

    // ---- PIPE ----
    else if (!strcmp(syscall, "pipe") || !strcmp(syscall, "pipe2")) {
        int fd1 = -1, fd2 = -1;
        extract_pipe_fds(rest, &fd1, &fd2);
        if (fd1 >= 0) fd_set_val(pid, fd1, "pipe_read");
        if (fd2 >= 0) fd_set_val(pid, fd2, "pipe_write");
        set_add(&IPC, syscall);
    }

    // ---- DUP ----
    else if (!strcmp(syscall, "dup") || !strcmp(syscall, "dup2") || !strcmp(syscall, "dup3")) {
        int oldfd, newfd;
        if (extract_two_fds(rest, &oldfd, &newfd)) {
            char *path = fd_get_val(pid, oldfd);
            if (path) fd_set_val(pid, newfd, path);
        }
        set_add(&IPC, syscall);
    }

    // ---- MMAP ----
    else if (!strcmp(syscall, "mmap")) {
        int fd;
        if (sscanf(rest, "%*[^,], %*[^,], %*[^,], %*[^,], %d", &fd) == 1) {
            if (fd >= 0) {
                char *path = fd_get_val(pid, fd);
                if (path) set_add(&READ, path);
            }
        }
        set_add(&IPC, "mmap");
    }

    else if (!strcmp(syscall, "munmap")) {
        set_add(&IPC, "munmap");
    }

    // ---- IOCTL ----
    else if (!strcmp(syscall, "ioctl")) {
        set_add(&IPC, "ioctl");
    }

    // ---- FCNTL ----
    else if (!strcmp(syscall, "fcntl")) {
        set_add(&IPC, "fcntl");
    }


    else if (!strcmp(syscall, "epoll_create1") ||
            !strcmp(syscall, "poll") ||
            !strcmp(syscall, "futex")) {
        set_add(&IPC, syscall);
    }

    // ---- MOUNT ----
    else if (!strcmp(syscall, "mount") || !strcmp(syscall, "umount") || !strcmp(syscall, "umount2")) {
        set_add(&MOUNT, syscall);
    }

    // ---- PRIVILEGED ----
    else if (!strcmp(syscall, "ptrace") || !strcmp(syscall, "capset") ||
             !strcmp(syscall, "reboot") || !strcmp(syscall, "swapon") ||
             !strcmp(syscall, "setuid") || !strcmp(syscall, "setgid")) {
        set_add(&PRIVILEGED, syscall);
    }

    // ignore syscalls not relevant for resource usage
    else if (!strcmp(syscall, "arch_prctl") ||
            !strcmp(syscall, "brk") ||
            !strcmp(syscall, "rseq") ||
            !strcmp(syscall, "rt_sigaction") ||
            !strcmp(syscall, "set_tid_address") ||
            !strcmp(syscall, "gettid") ||
            !strcmp(syscall, "getuid") ||
            !strcmp(syscall, "geteuid") ||
            !strcmp(syscall, "getgid") ||
            !strcmp(syscall, "getegid") ||
            !strcmp(syscall, "exit_group") ||
            !strcmp(syscall, "getrandom")) {
        // ignore
    }

    // ---- UNKNOWN (TODO) ----
    else {
        set_add(&UNKNOWN, syscall);
    }
}

// -------------------- Stream Parsing --------------------
void parse_stream(FILE *f) {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), f)) {
        parse_line(line);
    }
}

void parse_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        return;
    }

    parse_stream(f);
    fclose(f);
}

int main(int argc, char *argv[]) {
    
    set_init(&READ);
    set_init(&WRITE);
    set_init(&CREATE);
    set_init(&MKDIR);
    set_init(&DELETE);
    set_init(&RENAME);
    set_init(&METADATA);
    set_init(&EXEC);
    set_init(&PROCESS);
    set_init(&NETWORK_CONNECT);
    set_init(&NETWORK_BIND);
    set_init(&IPC);
    set_init(&MOUNT);
    set_init(&PRIVILEGED);
    set_init(&UNKNOWN);

    if (argc < 2) {
        // If no arguments are pass, read from stdin
        parse_stream(stdin);
    } else {
        // Parse all files passed as arguments
        for (int i = 1; i < argc; i++) {
            parse_file(argv[i]);
        }
    }

    // print results
    print_set("READ FILES", &READ);
    print_set("WRITE FILES", &WRITE);
    print_set("CREATED FILES", &CREATE);
    print_set("CREATED DIRECTORIES", &MKDIR);
    print_set("DELETED FILES", &DELETE);
    print_set("RENAMED FILES", &RENAME);
    print_set("METADATA", &METADATA);
    print_set("EXEC", &EXEC);
    print_set("PROCESS", &PROCESS);
    print_set("NETWORK CONNECT", &NETWORK_CONNECT);
    print_set("NETWORK BIND", &NETWORK_BIND);
    print_set("IPC", &IPC);
    print_set("MOUNT", &MOUNT);
    print_set("PRIVILEGED", &PRIVILEGED);
    print_set("UNKNOWN", &UNKNOWN);

    return 0;
}