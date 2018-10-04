#objs: test.o ebd_sys_util.o

run : test.o ebd_sys_util.o
	gcc -o run test.o ebd_sys_util.o

test.o : ebd_sys_util.h ebd_interface.h ebd_list.h
ebd_sys_util.o : ebd_sys_util.h

.PHONY: clean
clean:
	rm run *.o
