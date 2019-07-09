from distutils.core import setup, Extension

hashmap = Extension(
	'hashmap',
	sources=['hashmap.c', 'py_ext/py_hashmap.c'],
)

setup(
	name='hashmap',
	ext_modules=[hashmap],
)