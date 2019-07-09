from distutils.core import setup, Extension

hashmap = Extension(
	'hashmap',
	include_dirs=['.'],
	sources=['hashmap.c', 'extentions/py/py_hashmap.c'],
)

setup(
	name='hashmap',
	ext_modules=[hashmap],
)