#include "dash.h"

#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <sys/socket.h>

#define BUF_SIZE 14

static struct state {
    const struct button *buttons;
    size_t len;
    callback cb;
} state;

static int fd;
static pthread_t thread;
static bool running;

static void *run(void *arg);

int dash_init(const struct button buttons[], size_t len, callback cb) {
    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd == -1) {
        return DASH_ERROR;
    }

    state = (struct state) {
        .buttons = buttons,
        .len = len,
        .cb = cb
    };

    if (pthread_create(&thread, NULL, run, NULL) != 0) {
        close(fd);

        return DASH_ERROR;
    }

    return DASH_SUCCESS;
}

int dash_free(void) {
    running = false;

    int result = DASH_SUCCESS;

    if (pthread_join(thread, NULL) != 0) {
        result = DASH_ERROR;
    }

    if (close(fd) != 0) {
        result = DASH_ERROR;
    }

    return result;
}

static void *run(void *arg) {
    (void) arg;

    running = true;

    unsigned char buffer[BUF_SIZE];
    const struct ethhdr *eh = (struct ethhdr *) buffer;

    while (running) {
        if (recv(fd, buffer, BUF_SIZE, 0) == -1) {
            continue;
        }

        if (ntohs(eh->h_proto) != ETH_P_ARP) {
            continue;
        }

        for (size_t i = 0; i < state.len; i++) {
            if (memcmp(eh->h_source, state.buttons[i].addr, ETHER_ADDR_LEN) == 0) {
                state.cb(&state.buttons[i]);
                break;
            }
        }
    }

    return NULL;
}
