#ifndef RIO_H
#define RIO_H

#include <sys/types.h>

#ifndef RIO_BUFSIZE
#define RIO_BUFSIZE 8192
#elif RIO_BUFSIZE <= 0
#error RIO_BUFSIZE must be a positive integer
#endif

typedef struct rio {
	int fd;
	size_t cnt;
	char *bufp;
	char buf[RIO_BUFSIZE];
} rio_t;

ssize_t rio_readn(int fd, void *buf, size_t n);
ssize_t rio_writen(int fd, const void *buf, size_t n);

void rio_open(rio_t *rp, int fd);

ssize_t rio_readlineb(rio_t *rp, void *buf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *buf, size_t n);

#endif
