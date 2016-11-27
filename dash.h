#ifndef DASH_H
#define DASH_H

#include <stddef.h>

#define DASH_SUCCESS 0
#define DASH_ERROR -1

#define ETHER_ADDR_LEN 6

struct button {
    const char *name;
    const unsigned char addr[ETHER_ADDR_LEN];
};

typedef void (*callback)(const struct button *button);

int dash_init(const struct button buttons[], size_t len, callback cb);
int dash_free(void);

#endif
