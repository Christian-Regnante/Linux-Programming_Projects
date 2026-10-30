#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void compare_and_report(const char *filename)
{
    if (strcmp(filename, "core") == 0 || strcmp(filename, "sync.lock") == 0) {
        printf("[data_sync] Ignored special entry: %s\n", filename);
        return;
    }
    printf("[data_sync] Candidate for sync: %s\n", filename);
}

static int scan_directory(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    struct dirent *entry;

    if (dir == NULL) {
        perror("[data_sync] opendir failed");
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        compare_and_report(entry->d_name);
    }
    if (closedir(dir) != 0) {
        perror("[data_sync] closedir failed");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *target = (argc > 1) ? argv[1] : ".";

    return (scan_directory(target) == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
