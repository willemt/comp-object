SHELL  = /bin/bash
CC     = gcc


all: tests

fixedarraylist:
	mkdir -p fixedarraylist/.git
	git --git-dir=fixedarraylist/.git init 
	pushd fixedarraylist; git pull git@github.com:willemt/CFixedArraylist.git; popd

chashmap:
	mkdir -p chashmap_via_linked_list/.git
	git --git-dir=chashmap_via_linked_list/.git init 
	pushd chashmap_via_linked_list; git pull git@github.com:willemt/CHashMapViaLinkedList.git; popd

cllqueue:
	mkdir -p clinkedlistqueue/.git
	git --git-dir=clinkedlistqueue/.git init 
	pushd clinkedlistqueue; git pull git@github.com:willemt/CLinkedListQueue.git; popd

ccircularbuffer:
	mkdir -p ccircularbuffer/.git
	git --git-dir=ccircularbuffer/.git init 
	pushd ccircularbuffer; git pull git@github.com:willemt/CCircularBuffer.git; popd


download-contrib: chashmap fixedarraylist 



main.c:
	sh make-tests.sh > main.c

tests: main.c component.c component_context.c component_manager.c component_type.c test_component.c CuTest.c chashmap_via_linked_list/linked_list_hashmap.c ccircularbuffer/cbuffer.c component_messenger.c
	$(CC) -g -Ichashmap_via_linked_list -Iccircularbuffer -o $@ $^
	./tests

#texture_atlas.o: texture_atlas.c 
#	$(CC) -g -c -Ichashmap_via_linked_list -Ifixedarraylist -o $@ $^

clean:
	rm -f main.c tests

