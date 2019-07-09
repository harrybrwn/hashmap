PY=python
PyLib=$(PyTestDir)/hashmap.cpython-37m-x86_64-linux-gnu.so

py-test: $(PyLib)
	@$(PY) $(PyTestDir)/test.py

$(PyLib):
	$(PY) ./$(PyTestDir)/setup.py build --build-lib $(PyTestDir) -b $(PyTestDir)

py-clean:
	@for f in `find $(PyTestDir) -name '*.so'`; do rm $$f; done
	@if [ -d $(PyTestDir)/temp.* ]; then rm -rf $(PyTestDir)/temp.*; fi

.PHONY: py-test
