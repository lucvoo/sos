#ifndef	_DIAG_H_
#define	_DIAG_H_

void _os_diag_write_char(unsigned int c);
void _os_diag_write_string(const char* str);
void _os_diag_write_buf(const char* str, unsigned int n);

void _os_diag_hexval(unsigned long val);
void _os_diag_hexdump(const char *msg, const void *buff, unsigned int len, int ascii);

void _os_diag_printf(const char *fmt, ...);


static void _os_diag_nl(void) { _os_diag_write_char('\n'); }

#endif
