KVERSION := `uname -r`
KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)

obj-m += bastardo.o
bastardo-objs := proc.o util.o module.o hidemod.o

all:
	@$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules -Wall

clean:
	@-$(RM) *.o .*.o.cmd .*.ko.cmd bastardo.ko Module.symvers *.mod.c *.ko *~
	@-$(RM) -rf .tmp_versions

