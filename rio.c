#include "rio.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>

ssize_t rio_readn(int fd, void *buf, size_t n) {
	size_t left;
	ssize_t nread;
	char *bufp;

	left = n;
	bufp = buf;
	while (left > 0) {
		if ((nread = read(fd, bufp, left)) < 0) {
			if (errno == EINTR)
				continue;
			else
				return -1;
		} else if (nread == 0)
			break;
		left -= nread;
		bufp += nread;
	}
	return (n - left);
}

ssize_t rio_writen(int fd, const void *buf, size_t n) {
	size_t left;
	ssize_t nwritten;
	const char *bufp;

	left = n;
	bufp = buf;
	while (left > 0) {
		if ((nwritten = write(fd, bufp, left)) < 0) {
			if (errno == EINTR)
				continue;
			else
				return -1;
		}
		left -= nwritten;
		bufp += nwritten;
	}
	return n;
}

static ssize_t rio_read(rio_t *rp, char *buf, size_t n) {
	size_t cnt;

	while (rp->cnt <= 0) {
		rp->cnt = read(rp->fd, rp->buf, sizeof(rp->buf));
		if (rp->cnt < 0) {
			if (errno != EINTR)
				return -1;
		} else if (rp->cnt == 0)
			return 0;
		else
			rp->bufp = rp->buf;
	}
	cnt = n;
	if (rp->cnt < n)
		cnt = rp->cnt;
	memcpy(buf, rp->bufp, cnt);
	rp->bufp += cnt;
	rp->cnt -= cnt;
	return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *buf, size_t maxlen) {
	size_t n;
	ssize_t nread;
	char c, *bufp;

	bufp = buf;
	for (n = 1; n < maxlen; n++) {
		if ((nread = rio_read(rp, &c, 1) == 1)) {
			*bufp++ = c;
			if (c == '\n') {
				n++;
				break;
			}
		} else if (nread == 0) {
			if (n == 1)
				return 0;
			else
				break;
		} else
			return -1;
	}
	*bufp = 0;
	return (n - 1);
}

ssize_t rio_readnb(rio_t *rp, void *buf, size_t n) {
	size_t left;
	ssize_t nread;
	char *bufp;

	left = n;
	bufp = buf;
	while (left > 0) {
		if ((nread = rio_read(rp, bufp, left) < 0))
			return -1;
		else if (nread == 0)
			break;
		left -= nread;
		bufp += nread;
	}
	return (n - left);
}
