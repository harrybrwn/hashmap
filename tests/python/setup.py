from distutils.core import setup, Extension

# this script will be exectuted by the main Makefile

hashmap = Extension(
	'hashmap',
	include_dirs=['.', 'inc'],
	sources=['src/hashmap.c', 'extentions/py/py_hashmap.c'],
)

setup(
	name='hashmap',
	ext_modules=[hashmap],
)