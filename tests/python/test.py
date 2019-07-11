import hashmap
import unittest

import string
import random

LETTERS = string.ascii_lowercase + string.ascii_uppercase + string.digits

def _rand_keys(n):
	s = []
	for _ in range(n):
		s.append(''.join(random.choices(LETTERS, k=random.randint(5, 15))))
	return s


class HashMapTest(unittest.TestCase):
	def setUp(self):
		self.map = hashmap.HashMap()
		self.map.size = 100_000
		self.keys = _rand_keys(self.map.size)

	def test_putgetdelete(self):
		for i in range(self.map.size):
			self.map.put(self.keys[i], i)
		for i in range(self.map.size):
			self.assertEqual(self.map.get(self.keys[i]), i)
		for i in range(self.map.size):
			self.map.delete(self.keys[i])
		for k in self.keys:
			self.assertRaises(KeyError, self.map.get, k)

		self.assertEqual(self.map.size, 100_000)
		self.map.size = 33

	def test_Errors(self):
		self.assertRaises(TypeError, self.map.put, 1, "no")
		self.assertRaises(KeyError, self.map.delete, "non-existant-key")

	def test_dunders(self):
		data = 'the key should be "testing"'
		self.map.put('testing', data)
		self.map['test_val'] = 'this is a test'

		self.assertEqual(data, self.map['testing'])

	def test_clear(self):
		keys = ['one', 'two', 'three', 'four']
		for k in keys:
			self.map[k] = k
		self.assertEqual(len(self.map), len(keys))

		self.map.clear()
		self.assertEqual(len(self.map), 0)
		self.assertEqual(len(self.map.keys()), 0)
		for key in keys:
			try:
				self.map[key]
			except KeyError:
				pass
			else:
				self.assertTrue(False, "should have raised an error here")
	
	def test_keys(self):
		keys = ['one', 'two', 'three', 'four']
		for k in keys:
			self.map[k] = k
		self.assertEqual(keys, self.map.keys())

	def test_cyclic_gc(self):
		self.map.put('self.map', self.map)
		m = self.map.get('self.map')
		self.assertEqual(id(self.map), id(m))


if __name__ == '__main__':
	unittest.main()