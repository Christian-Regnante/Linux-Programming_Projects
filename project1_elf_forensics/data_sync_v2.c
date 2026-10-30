#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

static const char *LOG_FILE = "sync_audit.log";
static int audit_enabled = 1;

static uint32_t compute_djb2_hash(const char *text)
{
    uint32_t hash = 5381U;
    int character;

    while ((character = (unsigned char)*text++) != '\0') {
        hash = ((hash << 5) + hash) + (uint32_t)character;
    }
    return hash;
}

static void log_action(const char *action, const char *path, off_t size,
                       uint32_t hash)
{
    FILE *file;
    time_t now;

    if (!audit_enabled) {
        return;
    }
    file = fopen(LOG_FILE, "a");
    if (file == NULL) {
        return;
    }
    now = time(NULL);
    (void)fprintf(file, "[%ld] ACTION=%s PATH=%s SIZE=%ld HASH=0x%08X\n",
                  (long)now, action, path, (long)size, hash);
    (void)fclose(file);
}

static int inspect_file_metadata(const char *path)
{
    struct stat status;
    uint32_t name_hash;

    if (stat(path, &status) == -1) {
        perror("[data_sync_v2] stat failed");
        return -1;
    }
    name_hash = compute_djb2_hash(path);
    log_action("METADATA_CHECK", path, status.st_size, name_hash);
    printf("[data_sync_v2] Scanned: %s (Size: %ld bytes, Hash: 0x%08X)\n",
           path, (long)status.st_size, name_hash);
    return 0;
}

int main(int argc, char **argv)
{
    const char *target = (argc > 1) ? argv[1] : ".";
    DIR *directory = opendir(target);
    struct dirent *entry;
    int result = EXIT_SUCCESS;

    if (directory == NULL) {
        perror("[data_sync_v2] opendir failed");
        return EXIT_FAILURE;
    }
    while ((entry = readdir(directory)) != NULL) {
        char path[4096];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (snprintf(path, sizeof(path), "%s/%s", target, entry->d_name)
            >= (int)sizeof(path)) {
            fprintf(stderr, "[data_sync_v2] path too long: %s\n", entry->d_name);
            result = EXIT_FAILURE;
            continue;
        }
        if (inspect_file_metadata(path) != 0) {
            result = EXIT_FAILURE;
        }
    }
    if (closedir(directory) != 0) {
        perror("[data_sync_v2] closedir failed");
        result = EXIT_FAILURE;
    }
    return result;
}
