build: hashmap.o
	python setup.py build_ext --inplace
	# python setup.py build -cmingw32
	# @echo 'from . import hashmap' > hashmap/__init__.py

.PHONY: build

hashmap.o: ../hashmap.c
	gcc -Wall -I.. -c -fPIC -o $@ ../hashmap.c

clean:
	rm -rf build hashmap.o

.PHONY: clean