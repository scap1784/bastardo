#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/kallsyms.h>
#include "util.h"

static 	unsigned long (*_kallsymptr) (const char *name) = NULL;

/* Our own kallsyms_lookup */
unsigned long my_sym_lookup(const char *name)
{

	if(_kallsymptr == NULL) 
	{
		// Get an address to the proper kallsyms_lookup_name.
		unsigned long addr = get_kallsyms_func();

		WARN_ON((void *) addr == NULL);

		_kallsymptr = (unsigned long (*) (const char *name)) addr;
	}

	return (*_kallsymptr) (name);
}

/* Opens and parses /proc/kallsysms */
unsigned long get_kallsyms_func(void)
{
	int fd;
	char buf[1];
	// 16 chars for 64-bit Hex addresses
	char address[17];


	char name[KSYM_NAME_LEN + 1];
	int j, k;
	enum t_state {out, addr, sym_name, type} state;
	char *endp;
	unsigned long final_addr = 0;

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open("/proc/kallsyms", O_RDONLY, 0);

	//TODO: test for error here and return NULL.

	state = out;
	j = 0;
	k = 0;

	while(sys_read(fd, buf, 1) > 0)
	{
		char c = buf[0];

		if((state == out  || state == addr) && 
				((c >= '0' && c <= '9' ) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
		{
			state = addr;
			address[j++] = c;
		}
		else if(c == ' ' && state == addr)
		{
			state = type;
			name[j] = '\0';
		}
		else if(c == ' ' && state == type)
		{
			state = sym_name;
		}
		else if(state == type)
		{
			continue;
		}
		else if(state == sym_name && c != ' ' && c != '\n' && c != '\r' && c != '\t')
		{
			name[k++] = c;
		}
		else if(is_white_space(c) && state == sym_name)
		{
			state = out;
			name[k] = '\0';
			k = 0;
			j = 0;

			if(strcmp(name, "kallsyms_lookup_name") == 0)
			{
				final_addr = simple_strtoul(address, &endp, 16);
				goto out;
			}
		}

	}

out:
	sys_close(fd);
	set_fs(old_fs);

	return final_addr;
}

int inline is_white_space(char c)
{
	return c == '\n' || c == '\r' || c == '\t';
}


