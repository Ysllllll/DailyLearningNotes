CFLAGS += -Wall -g -lstdc++ -lpthread -D_FILE_OFFSET_BITS=64 -D_XOPEN_SOURCE -D_GUN_SOURCE -O2
LDFLAGS += lcrypt
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
