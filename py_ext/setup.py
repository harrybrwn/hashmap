from distutils.core import setup, Extension

hashmap = Extension(
	'hashmap',
	# libraries=['hashmap'],
	# library_dirs=['.'],
	extra_objects=['hashmap.o'],
	sources=['py_hashmap.c'],
)

setup(
	name='py_hashmap',
	version='1.0',
	description='a hashmap module',
	ext_modules=[hashmap]
)