#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int is_white_space(char c)
{
	return c == '\n' || c == '\r' || c == '\t';
}


void read_file(void)
{
	int fd;
	char buf[1];
	char address[1024];
	char name[1280];
	int j, k;
	enum t_state {out, addr, sym_name, type} state;
	char *endp;
	long final_addr = 0;


	fd = open("/proc/kallsyms", O_RDONLY);

	state = out;
	j = 0;
	k = 0;

	while(read(fd, buf, 1) > 0)
	{
		char c = buf[0];

		if((state == out  || state == addr) && ((c >= '0' && c <= '9' ) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
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
				final_addr = strtol(address, &endp, 16);
				goto out;
			}
		}
	}
out:

	printf("Found address: %lu\n", final_addr);
	close(fd);
}


int main(int argc, char ** argv)
{
	read_file();

	return 0;
}


