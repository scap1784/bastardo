#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


/* Handy macros to help getting network and transport headers from the socket. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#       define SKB_NH(a)                        (ip_hdr(a))
#       define SKB_TCP_H(a)                     (tcp_hdr(a))
#else
#       define SKB_NH(a)                        (a->nh.iph)
#       define SKB_TCP_H(a)             :       (a->h.th)
#endif

/* Symbols not exported by the kernel */
extern spinlock_t 	*_proc_dir_spinlock;
extern struct mutex 	*_module_mutex;

extern int is_white_space(char c);
extern unsigned long my_sym_lookup(const char *name);
extern unsigned long get_kallsyms_func(void);
extern void init_symbols(void);


