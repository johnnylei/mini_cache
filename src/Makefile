MODULE_TEST = module_test
COMPILED = compiled

C_FILE_LIST = $(wildcard *.c)
D_FILE_LIST = $(patsubst %.c, %.d, $(C_FILE_LIST))
O_FILE_LIST = $(patsubst %.c, %.o, $(C_FILE_LIST))
DEBUG_FILE_LIST = $(patsubst %.c, %.debug, $(C_FILE_LIST))
TEST_SERVER_DEPENDCE = $(patsubst main.debug, $(MODULE_TEST)/server_test.debug, $(DEBUG_FILE_LIST))

APPLICATION = server
APPLICATION_DEBUG = server_debug
APPLICATION_TEST = server_test

$(APPLICATION):$(O_FILE_LIST)
	gcc -pthread $^ -o $@

$(APPLICATION_DEBUG):$(DEBUG_FILE_LIST)
	gcc -pthread $^ -o $@

$(APPLICATION_TEST):$(TEST_SERVER_DEPENDCE)
	gcc -pthread $^ -o $@

event_test:event_test.debug HashTable.debug Link.debug Event.debug
	gcc -g $^ -o $@

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
	rm *.d *.o *.pthread *.i *.out *.debug *test *debug $(APPLICATION)
	rm $(MODULE_TEST)/*.d $(COMPILED)/*.o *$(COMPILED)/.pthread *$(COMPILED)/.i *$(COMPILED)/.out *$(COMPILED)/.debug *$(COMPILED)/test *$(COMPILED)/debug
	rm -rf $(COMPILED)/*
