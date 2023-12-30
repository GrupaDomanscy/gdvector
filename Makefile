major_version = 0
minor_version = 3
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
	gcc -Wall -Wextra main.c basicvector.c -o build/test
	./build/test

install:
	rm -rf /usr/lib/libbasicvector.$(lib_version).so /usr/lib/libbasicvector.$(major_version).so /usr/lib/libbasicvector.so
	rm -rf /usr/include/basicvector.h
	cp build/libbasicvector.$(lib_version).so /usr/lib/libbasicvector.$(lib_version).so
	ln -s /usr/lib/libbasicvector.$(lib_version).so /usr/lib/libbasicvector.$(major_version).so
	ln -s /usr/lib/libbasicvector.$(lib_version).so /usr/lib/libbasicvector.so
	cp build/basicvector.h /usr/include/basicvector.h

uninstall:
	rm -rf /usr/lib/libbasicvector.*
	rm -rf /usr/include/basicvector.h

clean:
	rm -rf ./build

