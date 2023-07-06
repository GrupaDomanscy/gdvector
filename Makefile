lib_version = 1.0.0

all:
	make clean
	mkdir -p ./build
	gcc -Wall -Wextra -fpic -c gdvector.c -o build/libgdvector.$(lib_version).o
	gcc -Wall -Wextra -shared -o build/libgdvector.$(lib_version).so build/libgdvector.$(lib_version).o
	cp gdvector.h build/gdvector.h

test:
	make clean
	mkdir -p ./build
	gcc -Wall -Wextra -lgdalloc -lcriterion main.c gdvector.c -o build/test
	./build/test


install:
	rm -rf /usr/lib64/libgdvector.$(lib_version).so /usr/lib64/libgdvector.1.so /usr/lib64/libgdvector.so
	rm -rf /usr/include/gdvector.h
	cp build/libgdvector.$(lib_version).so /usr/lib64/libgdvector.$(lib_version).so
	ln -s /usr/lib64/libgdvector.$(lib_version).so /usr/lib64/libgdvector.1.so
	ln -s /usr/lib64/libgdvector.$(lib_version).so /usr/lib64/libgdvector.so
	cp build/gdvector.h /usr/include/gdvector.h

uninstall:
	rm -rf /usr/lib64/libgdvector.*
	rm -rf /usr/include/gdvector.h

clean:
	rm -rf ./build


