#ifndef	_STRING_H_
#define	_STRING_H_

void *memchr(const void *s, int c, unsigned int n);
int memcmp(const void *a, const void *b, unsigned int n);
void memcpy(void *dst, const void *src, unsigned int n);
void memset(void *dst, int c, unsigned int n);
int strcmp(const char *a, const char *b);
unsigned int strlen(const char *str);
void strncpy(char *dst, const char *src, unsigned int max);
int strtoul(const char *str, unsigned int *val);

#endif
