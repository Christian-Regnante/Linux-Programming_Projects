#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 4096

static int write_log(const char *message)
{
    char buffer[256];
    int fd;
    int length;
    time_t now = time(NULL);

    fd = open("backup_dir/backup.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        return -1;
    }
    length = snprintf(buffer, sizeof(buffer), "[%ld] %s\n", (long)now, message);
    if (length < 0 || write(fd, buffer, (size_t)length) != length) {
        (void)close(fd);
        return -1;
    }
    return close(fd);
}

static int copy_file_worker(const char *source, const char *destination)
{
    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    int source_fd = open(source, O_RDONLY);
    int destination_fd;

    if (source_fd < 0) {
        return 1;
    }
    destination_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd < 0) {
        (void)close(source_fd);
        return 2;
    }
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(destination_fd, buffer, (size_t)bytes_read);
        if (bytes_written != bytes_read) {
            (void)close(source_fd);
            (void)close(destination_fd);
            return 3;
        }
    }
    if (bytes_read < 0) {
        (void)close(source_fd);
        (void)close(destination_fd);
        return 4;
    }
    if (close(source_fd) != 0 || close(destination_fd) != 0) {
        return 5;
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *source = (argc > 1) ? argv[1] : "source.txt";
    const char *destination = "backup_dir/destination.txt";
    int status;
    pid_t child;

    if (mkdir("backup_dir", 0755) == -1 && errno != EEXIST) {
        perror("mkdir failed");
        return EXIT_FAILURE;
    }
    if (write_log("Backup directory initialized") != 0) {
        perror("backup log failed");
        return EXIT_FAILURE;
    }
    child = fork();
    if (child < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }
    if (child == 0) {
        _exit(copy_file_worker(source, destination));
    }
    if (waitpid(child, &status, 0) < 0) {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        if (write_log("Backup completed successfully") != 0) {
            perror("backup success log failed");
            return EXIT_FAILURE;
        }
        printf("[backup_tool] Backup SUCCESS\n");
        return EXIT_SUCCESS;
    }
    if (write_log("Backup failed during file copy") != 0) {
        perror("backup failure log failed");
    }
    fprintf(stderr, "[backup_tool] Backup FAILED (child exit=%d)\n",
            WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    return EXIT_FAILURE;
}
