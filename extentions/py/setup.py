from distutils.core import setup, Extension

hashmap = Extension(
	'hashmap',
	sources=['../hashmap.c', 'py_hashmap.c']
)

setup(
	name='hashmap',
	version='1.0',
	description='a hashmap module',
	ext_modules=[hashmap]
)