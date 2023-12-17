major_version = 1
minor_version = 1
patch_version = 0

lib_version = $(major_version).$(minor_version).$(patch_version)

all:
	make clean
	mkdir -p ./build
	gcc -Wall -Wextra -fpic -c basicvector.c -o build/libbasicvector.$(lib_version).o
	gcc -Wall -Wextra -shared -o build/libbasicvector.$(lib_version).so build/libbasicvector.$(lib_version).o
	cp basicvector.h build/basicvector.h

test:
	make clean
	mkdir -p ./build
	gcc -Wall -Wextra -lgdalloc -lcriterion main.c basicvector.c -o build/test
	./build/test


install:
	rm -rf /usr/lib64/libbasicvector.$(lib_version).so /usr/lib64/libbasicvector.$(major_version).so /usr/lib64/libbasicvector.so
	rm -rf /usr/include/basicvector.h
	cp build/libbasicvector.$(lib_version).so /usr/lib64/libbasicvector.$(lib_version).so
	ln -s /usr/lib64/libbasicvector.$(lib_version).so /usr/lib64/libbasicvector.$(major_version).so
	ln -s /usr/lib64/libbasicvector.$(lib_version).so /usr/lib64/libbasicvector.so
	cp build/basicvector.h /usr/include/basicvector.h

uninstall:
	rm -rf /usr/lib64/libbasicvector.*
	rm -rf /usr/include/basicvector.h

clean:
	rm -rf ./build


