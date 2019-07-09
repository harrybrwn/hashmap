from distutils.core import setup, Extension

hashmap = Extension(
	'hashmap.hashmap',
	extra_objects=['hashmap.o'],
	sources=['py_hashmap.c'],
)

setup(name='py_hashmap', ext_modules=[hashmap])