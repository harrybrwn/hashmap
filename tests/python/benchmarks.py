from hashmap import HashMap

def bench_HashMap():
	m = HashMap()
	m['test'] = 'test'
	m['test']
	if m['test'] != 'test':
		print('what')

def bench_dict():
	d = dict()
	d['test'] = 'test'
	if d['test'] != 'test':
		print('what')


if __name__ == '__main__':
	from timeit import timeit
	print("HashMap:", timeit("bench_HashMap()", setup='from __main__ import bench_HashMap'))
	print("dict:   ", timeit("bench_dict()", setup='from __main__ import bench_dict'))