#ifndef	_DIAG_H_
#define	_DIAG_H_

void _os_diag_write_char(unsigned int c);
void _os_diag_write_string(const char* str);
void _os_diag_write_buf(const char* str, unsigned int n);

void _os_diag_printf(const char *fmt, ...);

#endif
