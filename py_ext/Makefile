build:
	python setup.py build

install: hashmap.o
	python setup.py install

clean:
	rm -rf build hashmap.o hashmap *.so py_hashmap *.linux*

.PHONY: build install clean