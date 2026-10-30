#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum {
    STATE_NORMAL,
    STATE_MAINTENANCE
} service_state_t;

static volatile sig_atomic_t sig_usr1_received;
static volatile sig_atomic_t sig_usr2_received;
static volatile sig_atomic_t sig_int_received;
static volatile sig_atomic_t sig_term_received;

static void signal_handler(int signum)
{
    if (signum == SIGUSR1) {
        sig_usr1_received = 1;
    } else if (signum == SIGUSR2) {
        sig_usr2_received = 1;
    } else if (signum == SIGINT) {
        sig_int_received = 1;
    } else if (signum == SIGTERM) {
        sig_term_received = 1;
    }
}

static int setup_signal_handlers(void)
{
    struct sigaction action;
    int signals[] = {SIGUSR1, SIGUSR2, SIGINT, SIGTERM};
    size_t index;

    (void)memset(&action, 0, sizeof(action));
    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    for (index = 0; index < sizeof(signals) / sizeof(signals[0]); ++index) {
        if (sigaction(signals[index], &action, NULL) != 0) {
            return -1;
        }
    }
    return 0;
}

static const char *state_name(service_state_t state)
{
    return state == STATE_NORMAL ? "NORMAL" : "MAINTENANCE";
}

int main(void)
{
    service_state_t state = STATE_NORMAL;
    pid_t process_id = getpid();

    if (setup_signal_handlers() != 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    printf("[Server Controller] Service started with PID: %ld\n", (long)process_id);
    fflush(stdout);
    for (;;) {
        if (sig_term_received != 0) {
            const char message[] =
                "[Server Controller] EMERGENCY TERMINATION (SIGTERM). Halting immediately.\n";
            (void)write(STDERR_FILENO, message, sizeof(message) - 1);
            return EXIT_FAILURE;
        }
        if (sig_usr2_received != 0) {
            sig_usr2_received = 0;
            state = state == STATE_NORMAL ? STATE_MAINTENANCE : STATE_NORMAL;
            printf("[Server Controller] State switched to: %s\n", state_name(state));
            fflush(stdout);
        }
        if (sig_usr1_received != 0) {
            sig_usr1_received = 0;
            printf("[Server Controller] STATUS REPORT: PID=%ld, State=%s\n",
                   (long)process_id, state_name(state));
            fflush(stdout);
        }
        if (sig_int_received != 0) {
            printf("[Server Controller] SIGINT received. Performing graceful shutdown...\n");
            printf("[Server Controller] Cleanup completed. Exiting safely.\n");
            fflush(stdout);
            return EXIT_SUCCESS;
        }
        printf("[Server Controller] Service running in %s mode...\n", state_name(state));
        fflush(stdout);
        (void)sleep(5);
    }
}
