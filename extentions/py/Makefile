PY=python3.7

build:
	@# $(PY) setup.py build_ext --inplace
	$(PY) setup.py build

test:
	@$(PY) setup.py build_ext --inplace
	$(PY) test.py

install:
	$(PY) setup.py install

clean:
	rm -rf build hashmap.o hashmap *.so py_hashmap *.linux*

.PHONY: build install clean