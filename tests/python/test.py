import hashmap
import unittest
import sys

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
		self.map.size = 10_000
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

		self.assertEqual(self.map.size, 10_000)
		self.map.size = 33

	def test_Errors(self):
		self.assertRaises(TypeError, self.map.put, 1, "no")
		self.assertRaises(KeyError, self.map.delete, "non-existant-key")

	def test_dunders(self):
		data = 'the key should be "testing"'
		self.map.put('testing', data)
		self.map['test_val'] = 'this is a test'
		self.assertEqual(data, self.map['testing'])

	def test_iter(self):
		keys = ['one', 'two', 'three', 'five', 'seven', 'eleven', 'thirteen']
		for k in keys:
			self.map.put(k, 'prime')
		
		for key in self.map:
			self.assertTrue(key in keys)

	def test_refcounting(self):
		m = hashmap.HashMap()
		one = 1
		refc = sys.getrefcount(one)
		m.put('one', one)
		self.assertEqual(sys.getrefcount(one), refc + 1)

		m.delete('one')
		self.assertEqual(sys.getrefcount(one), refc)

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

	def test_init(self):
		m = hashmap.HashMap(99)
		self.assertEqual(m.size, 99)

	def test_keys(self):
		keys = ['one', 'two', 'three', 'four', 'five', 'six']
		for k in keys:
			self.map[k] = 'what?'
		self.assertEqual(set(keys), set(self.map.keys()))

	def test_cyclic_gc(self):
		self.map.put('self.map', self.map)
		m = self.map.get('self.map')
		self.assertEqual(id(self.map), id(m))
	
	def test_contains(self):
		self.map['athing'] = 'some thing'
		self.assertTrue('athing' in self.map)
		self.assertFalse('nothere' in self.map)
		try:
			5 in self.map
		except TypeError:
			pass
		else:
			self.assertTrue(False, msg="should have been a type error here")


if __name__ == '__main__':
	unittest.main()