from distutils.core import setup, Extension

hashmap = Extension(
	'hashmap',
	include_dirs=['../../'],
	sources=['../../hashmap.c', 'py_hashmap.c']
)

setup(
	name='hashmap',
	version='1.0',
	author='Harry Brown',
	description='a hashmap module',
	ext_modules=[hashmap]
)