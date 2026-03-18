#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>

#define MAX_LINE 4096

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
Set CHMOD, CHOWN, SETUID, SETGID;
Set NETWORK_CONNECT, NETWORK_BIND, MOUNT, PRIVILEGED;

// -------------------- Regex Helpers --------------------
int regex_match(const char *pattern, const char *str, regmatch_t *matches, int nmatch) {
    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED))
        return 0;

    int ret = regexec(&regex, str, nmatch, matches, 0);
    regfree(&regex);

    return ret == 0;
}

void extract_group(const char *src, regmatch_t match, char *dest) {
    int len = match.rm_eo - match.rm_so;
    strncpy(dest, src + match.rm_so, len);
    dest[len] = '\0';
}

// -------------------- Logic --------------------
void classify_file_open(const char *flags, const char *file) {
    if (strstr(flags, "O_CREAT") ||
        strstr(flags, "O_WRONLY") ||
        strstr(flags, "O_RDWR") ||
        strstr(flags, "O_TRUNC")) {
        set_add(&WRITE, file);
    }

    if (strstr(flags, "O_CREAT")) {
        set_add(&CREATE, file);
    }

    if (strstr(flags, "O_RDONLY")) {
        set_add(&READ, file);
    }
}

void parse_line(char *line) {
    regmatch_t m[3];
    char file1[1024], file2[1024], flags[256];

    // open/openat
    if (regex_match("openat?\\([^\"].*\"([^\"]+)\",[[:space:]]*([A-Z_|]+)", line, m, 3)) {
        extract_group(line, m[1], file1);
        extract_group(line, m[2], flags);
        classify_file_open(flags, file1);
        return;
    }

    // stat family
    if (regex_match("(newfstatat|fstatat|statx|stat|lstat)\\([^\"].*\"([^\"]+)\"", line, m, 3)) {
        extract_group(line, m[2], file1);
        set_add(&READ, file1);
        return;
    }

    // mkdir
    if (regex_match("mkdirat?\\([^\"].*\"([^\"]+)\"", line, m, 2)) {
        extract_group(line, m[1], file1);
        set_add(&MKDIR, file1);
        return;
    }

    // unlink
    if (regex_match("unlinkat?\\([^\"].*\"([^\"]+)\"", line, m, 2)) {
        extract_group(line, m[1], file1);
        set_add(&DELETE, file1);
        return;
    }

    // rename
    if (regex_match("renameat?\\([^\"].*\"([^\"]+)\",[[:space:]]*\"([^\"]+)\"", line, m, 3)) {
        extract_group(line, m[1], file1);
        extract_group(line, m[2], file2);

        char buffer[4096];
        snprintf(buffer, sizeof(buffer), "%s -> %s", file1, file2);
        set_add(&RENAME, buffer);
        return;
    }

    // chmod
    if (regex_match("chmodat?\\([^\"].*\"([^\"]+)\"", line, m, 2)) {
        extract_group(line, m[1], file1);
        set_add(&CHMOD, file1);
        return;
    }

    // chown
    if (regex_match("fchownat?\\([^\"].*\"([^\"]+)\"", line, m, 2)) {
        extract_group(line, m[1], file1);
        set_add(&CHOWN, file1);
        return;
    }

    // setuid
    if (regex_match("setuid\\(([0-9]+)\\)", line, m, 2)) {
        extract_group(line, m[1], file1);
        set_add(&SETUID, file1);
        return;
    }

    // setgid
    if (regex_match("setgid\\(([0-9]+)\\)", line, m, 2)) {
        extract_group(line, m[1], file1);
        set_add(&SETGID, file1);
        return;
    }

    // network
    if (strstr(line, "socket(")) set_add(&NETWORK_CONNECT, "socket");
    if (strstr(line, "connect(")) set_add(&NETWORK_CONNECT, "connect");
    if (strstr(line, "bind(")) set_add(&NETWORK_BIND, "bind");

    // mount
    if (strstr(line, "mount(")) set_add(&MOUNT, "mount");

    // privileged
    if (strstr(line, "ptrace(") ||
        strstr(line, "capset(") ||
        strstr(line, "reboot(") ||
        strstr(line, "swapon(")) {

        char syscall[256];
        sscanf(line, "%[^ (]", syscall);
        set_add(&PRIVILEGED, syscall);
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
    set_init(&CHMOD);
    set_init(&CHOWN);
    set_init(&SETUID);
    set_init(&SETGID);
    set_init(&NETWORK_CONNECT);
    set_init(&NETWORK_BIND);
    set_init(&MOUNT);
    set_init(&PRIVILEGED);

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
    print_set("CHMOD OPERATIONS", &CHMOD);
    print_set("CHOWN OPERATIONS", &CHOWN);
    print_set("SETUID CALLS", &SETUID);
    print_set("SETGID CALLS", &SETGID);
    print_set("NETWORK CONNECT", &NETWORK_CONNECT);
    print_set("NETWORK BIND", &NETWORK_BIND);
    print_set("MOUNT CALLS", &MOUNT);
    print_set("PRIVILEGED SYSCALLS", &PRIVILEGED);

    return 0;
}