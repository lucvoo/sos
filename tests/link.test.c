#include <version.h>
#include <diag.h>

void kapi_start(void)
{
	_os_diag_write_string(os_version);
}
