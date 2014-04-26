CONTRIB_DIR = ..
FIXEDARRAYLIST_DIR = $(CONTRIB_DIR)/CFixedArraylist
HASHMAP_DIR = $(CONTRIB_DIR)/CHashMapViaLinkedList
LLQUEUE_DIR = $(CONTRIB_DIR)/CLinkedListQueue
CBUFFER_DIR = $(CONTRIB_DIR)/CCircularBuffer

GCOV_OUTPUT = *.gcda *.gcno *.gcov 
GCOV_CCFLAGS = -fprofile-arcs -ftest-coverage
SHELL  = /bin/bash
CC     = gcc
CCFLAGS = -g -O2 -Wall -Werror -W -fno-omit-frame-pointer -fno-common -fsigned-char $(GCOV_CCFLAGS) -I$(HASHMAP_DIR) -I$(CBUFFER_DIR)


all: tests

fixedarraylist:
	mkdir -p $(FIXEDARRAYLIST_DIR)/.git
	git --git-dir=$(FIXEDARRAYLIST_DIR)/.git init 
	pushd $(FIXEDARRAYLIST_DIR); git pull git@github.com:willemt/CFixedArraylist.git; popd

chashmap:
	mkdir -p $(HASHMAP_DIR)/.git
	git --git-dir=$(HASHMAP_DIR)/.git init 
	pushd $(HASHMAP_DIR); git pull git@github.com:willemt/CHashMapViaLinkedList.git; popd

cllqueue:
	mkdir -p $(LLQUEUE_DIR)/.git
	git --git-dir=$(LLQUEUE_DIR)/.git init 
	pushd $(LLQUEUE_DIR); git pull git@github.com:willemt/CLinkedListQueue.git; popd

ccircularbuffer:
	mkdir -p $(CBUFFER_DIR)/.git
	git --git-dir=$(CBUFFER_DIR)/.git init 
	pushd $(CBUFFER_DIR); git pull git@github.com:willemt/CCircularBuffer.git; popd


download-contrib: chashmap fixedarraylist ccircularbuffer



main.c:
	sh make-tests.sh > main.c

tests: main.c component.c component_context.c component_manager.c component_type.c test_component.c CuTest.c $(HASHMAP_DIR)/linked_list_hashmap.c $(CBUFFER_DIR)/cbuffer.c component_messenger.c
	#$(CC) -g -I$(HASHMAP_DIR) -I$(CBUFFER_DIR) -o $@ $^
	$(CC) $(CCFLAGS) -o $@ $^
	./tests
	gcov main.c test_component.c component_context.c component_manager.c component_type.c component_messenger.c
	./tests

#texture_atlas.o: texture_atlas.c 
#	$(CC) -g -c -Ichashmap_via_linked_list -Ifixedarraylist -o $@ $^

clean:
	rm -f main.c tests

