MODULE_TEST = module_test
COMPILED = compiled

C_FILE_LIST = $(wildcard *.c)
D_FILE_LIST = $(patsubst %.c, %.d, $(C_FILE_LIST))
O_FILE_LIST = $(patsubst %.c, %.o, $(C_FILE_LIST))
DEBUG_FILE_LIST = $(patsubst %.c, %.debug, $(C_FILE_LIST))

APPLICATION = server
APPLICATION_DEBUG = server_debug

$(APPLICATION):$(O_FILE_LIST)
	gcc -pthread $^ -o $@

$(APPLICATION_DEBUG):$(DEBUG_FILE_LIST)
	gcc -pthread $^ -o $@

event_test:event_test.o HashTable.o Link.o Event.o
	gcc $^ -o $@

link_test:link_test.o Link.o
	gcc $^ -o $@

event_test_debug:event_test.debug HashTable.debug Link.debug Event.debug
	gcc -g $^ -o $@

%.o:%.c
	gcc -MMD -c $< -o $@

%.debug:%.c
	gcc -MMD -c -g $< -o $@

%.o:$(MODULE_TEST)/%.c
	gcc -MMD -c $< -o $@

%.debug:$(MODULE_TEST)/%.c
	gcc -MMD -c -g $< -o $@

-include $(D_FILE_LIST)

clean:
	rm *.d *.o *.pthread *.i *.out *.debug *test *debug
	rm -rf $(COMPILED)/*
