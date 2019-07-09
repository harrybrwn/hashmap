PY=python
PyLib=$(TestDir)/python/hashmap.cpython-37m-x86_64-linux-gnu.so

py-test: $(PyLib)
	@$(PY) $(TestDir)/python/test.py

$(PyLib):
	$(PY) ./$(TestDir)/python/setup.py build --build-lib $(TestDir)/python

py-clean:
	@for f in `find $(TestDir)/python -name '*.so'`; do rm $$f; done

.PHONY: py-test
