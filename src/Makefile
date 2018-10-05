#objs: test.o ebd_sys_util.o

run : test.o ebd_sys_util.o ebd_interface.o ebd_block.o ebd_util.o
	gcc -o run test.o ebd_sys_util.o ebd_interface.o ebd_block.o ebd_util.o

test.o : ebd_sys_util.h ebd_interface.h ebd_list.h ebd_util.h
ebd_sys_util.o : ebd_sys_util.h
ebd_interface.o : ebd_interface.h ebd_sys_util.h
ebd_block.o : ebd_block.h ebd_sys_util.h ebd_util.h
ebd_util.o : ebd_util.h ebd_sys_util.h ebd_block.h

.PHONY: clean
clean:
	rm run *.o
