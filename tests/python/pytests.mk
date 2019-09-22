PY=python
PyLib=$(PyTestDir)/hashmap.cpython-37m-x86_64-linux-gnu.so

py-test: $(PyLib)
	@$(PY) $(PyTestDir)/test.py

$(PyLib): $(ExtDir)/py/py_hashmap.c
	$(PY) ./$(PyTestDir)/setup.py build --build-lib $(PyTestDir) -b $(PyTestDir)