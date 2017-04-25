#ifndef LIBNETFILES_H
#define LIBNETFILES_H

int netopen(const char *, int);
int netclose(int);
ssize_t netread(int, void *, size_t);
ssize_t netwrite(int, const void *, size_t);
int netserverinit(char *);

#endif